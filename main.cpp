

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

#include <Windows.h>
#include "common.h"
#include "mydirect3d.h"
#include "texture.h"
#include "sprite.h"
#include"input.h"
#include "Field.h"
#include"Stage.h"
#include"Player.h"
#include"Enemy.h"
#include <math.h>
#include <d3dx9.h>
#include "system_timer.h"
#include "sound.h"


//===============================================
//ここを追加　3D用追加コード

//プロジェクト用パラメータ
#define VIEW_ANGLE (D3DXToRadian(45.0f))//ビュー平面の視野角
#define VIEW_ASPECT ((float)SCREEN_WIDTH/(float)SCREEN_HEIGHT)//ビュー平面のアスペクト比
#define VIEW_NEAR_Z (1.0f)//ビュー平面のNearZ値
#define VIEW_FAR_Z (1000.0f)//ビュー平面のFarZ値

//カメラ用パラメータ
D3DXVECTOR3 g_posCamaraEye;//カメラの視点
D3DXVECTOR3 g_posCamaraAt;//カメラの注視点
D3DXVECTOR3 g_vecCamaraUp;//カメラの上方向

//プロジェクション＆カメラ行列
D3DXMATRIX g_mtxView;//ビューマトリックス
D3DXMATRIX g_mtxProjection;//プロジェクションマトリックス

//サンプルポリゴンの表示自体に関するパラメータ
D3DXVECTOR3 g_pos;//ポリゴンの位置(四角形の中央座標)
D3DXVECTOR3 g_rot;//ポリゴンの向き(回転)
D3DXVECTOR3 g_scl;//ポリゴンの大きさ(スケール)

D3DXMATRIX g_mtxWorld;//ワールドマトリックス

LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuff; //頂点バッファ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuff1; //板の頂点バッファ



//ここまで3D用追加コード
//===============================================



/*------------------------------------------------------------------------------
   定数定義
------------------------------------------------------------------------------*/
#define CLASS_NAME     "GameWindow"       // ウインドウクラスの名前
#define WINDOW_CAPTION "ゲームウィンドウ" // ウィンドウの名前
#define FPS_STANTARD 60.0f				  // フレームレート


float pos_x = 0.0f;
float posadd_x = 0.1f;

float Camerapos_x = 10.0f, Camerapos_y = 25.0f, Camerapos_z = -10.0f;

/*------------------------------------------------------------------------------
   プロトタイプ宣言
------------------------------------------------------------------------------*/
// ウィンドウプロシージャ(コールバック関数)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ゲームの初期化関数
// 戻り値:初期化に失敗したときfalse
static bool Initialize(void);
// ゲームの更新関数
static void Update(void);
// ゲームの描画関数
static void Draw(void);
// ゲームの終了処理
static void Finalize(void);


/*------------------------------------------------------------------------------
   グローバル変数宣言
------------------------------------------------------------------------------*/
static HWND g_hWnd;             // ウィンドウハンドル
static float g_Rotation = 0.0f; // スプライト回転角度
static float number = 0;

D3DLIGHT9 g_Light; //ライト構造体

//インターフェースのポインターの入れ物を用意
LPDIRECT3DINDEXBUFFER9 g_pD3DidxBuff;

static int g_FrameCount = 0; // フレームカウンター
static double g_StaticFrameTime = 0.0f; // フレーム固定用計測時間

/*------------------------------------------------------------------------------
   関数定義
------------------------------------------------------------------------------*/

//#######################################################################
// メイン関数
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 使用しない一時変数を明示
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // ウィンドウクラス構造体の設定
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;                          // ウィンドウプロシージャの指定
    wc.lpszClassName = CLASS_NAME;                     // クラス名の設定
    wc.hInstance = hInstance;                          // インスタンスハンドルの指定
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);          // マウスカーソルを指定
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); // ウインドウのクライアント領域の背景色を設定

    // クラス登録
    RegisterClass(&wc);


    // ウィンドウスタイル
    DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);

    // 基本矩形座標
    RECT window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    // 指定したクライアント領域を確保するために新たな矩形座標を計算
    AdjustWindowRect(&window_rect, window_style, FALSE);

    // 新たなWindowの矩形座標から幅と高さを算出
    int window_width = window_rect.right - window_rect.left;
    int window_height = window_rect.bottom - window_rect.top;

    // プライマリモニターの画面解像度取得
    int desktop_width = GetSystemMetrics(SM_CXSCREEN);
    int desktop_height = GetSystemMetrics(SM_CYSCREEN);

    // デスクトップの真ん中にウィンドウが生成されるように座標を計算
    // ※ただし万が一、デスクトップよりウィンドウが大きい場合は左上に表示
    int window_x = max((desktop_width - window_width) / 2, 0);
    int window_y = max((desktop_height - window_height) / 2, 0);

    // ウィンドウの生成
    g_hWnd = CreateWindow(
        CLASS_NAME,     // ウィンドウクラス
        WINDOW_CAPTION, // ウィンドウテキスト
        window_style,   // ウィンドウスタイル
        window_x,       // ウィンドウ座標x
        window_y,       // ウィンドウ座標y
        window_width,   // ウィンドウの幅
        window_height,  // ウィンドウの高さ
        NULL,           // 親ウィンドウハンドル
        NULL,           // メニューハンドル
        hInstance,      // インスタンスハンドル
        NULL            // 追加のアプリケーションデータ
    );

    if( g_hWnd == NULL ) {
        // ウィンドウハンドルが何らかの理由で生成出来なかった
        return -1;
    }

    // 指定のウィンドウハンドルのウィンドウを指定の方法で表示
    ShowWindow(g_hWnd, nCmdShow);


	Keyboard_Initialize(hInstance, g_hWnd);

	// Windowsゲーム用メインループ
	MSG msg = {}; // msg.message == WM_NULL

	// ゲームの初期化(Direct3Dの初期化)
	if( !Initialize() ) {
        // ゲームの初期化に失敗した
		return -1;
	}

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// メッセージがある場合はメッセージ処理を優先
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// 現在のシステム時間を取得
			double time = SystemTimer_GetTime();

			if (time - g_StaticFrameTime < 1.0 / FPS_STANTARD)
			{
				// 1 / 60 秒経っていなかったら空回り
				Sleep(0);
			}
			else
			{
				// フレーム固定用の計測時間を更新する
				g_StaticFrameTime += 1.0 / 60;// ゲーム処理を行った時間を記録
				// ゲームの更新
				Update();
				// ゲームの描画
				Draw();
			}
		}
	}

	Keyboard_Finalize();
	// ゲームの終了処理(Direct3Dの終了処理)
	Finalize();

    return (int)msg.wParam;
}

//#######################################################################
// ウィンドウプロシージャ(コールバック関数)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch( uMsg ) {
        case WM_KEYDOWN:
            if( wParam == VK_ESCAPE ) {
                SendMessage(hWnd, WM_CLOSE, 0, 0); // WM_CLOSEメッセージの送信
            }
            break;

        case WM_CLOSE:
                DestroyWindow(hWnd); // 指定のウィンドウにWM_DESTROYメッセージを送る
            return 0; // DefWindowProc関数にメッセージを流さず終了することによって何もなかったことにする

        case WM_DESTROY: // ウィンドウの破棄メッセージ
            PostQuitMessage(0); // WM_QUITメッセージの送信
            return 0;
    };

    // 通常メッセージ処理はこの関数に任せる
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//#######################################################################
// ゲームの初期化関数
bool Initialize(void)
{
	//ライトの設定
	ZeroMemory(&g_Light, sizeof(D3DLIGHT9));//構造体を０でクリア(必須)
	g_Light.Type = D3DLIGHT_DIRECTIONAL;//ライトの種類
	g_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//光の色RGBA
	g_Light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//環境光の色
	D3DXVECTOR3 vecDir = D3DXVECTOR3(0.0f, -0.6f, 1.0f);//光の指す方向ベクトル
	D3DXVec3Normalize(&vecDir, &vecDir);
	g_Light.Direction = vecDir;//正規化してセット


	// Direct3Dラッパーモジュールの初期化
	if (!MyDirect3D_Initialize(g_hWnd)) {
		return false;
	}

	//カメラ
	g_posCamaraEye = D3DXVECTOR3(Camerapos_x, Camerapos_y, Camerapos_z);
	g_posCamaraAt = D3DXVECTOR3(10, 0, 5);
	g_vecCamaraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// テクスチャの読み込み
	if (Texture_Load() > 0) {
		MessageBox(g_hWnd, "いくつか読み込めなかったテクスチャファイルがあります", "エラー", MB_OK);
	}

	FieldInit();
	StageInit();
	PlayerInit();
	EnemyInit();

    return true;
}

//#######################################################################
// ゲームの更新関数
void Update(void)
{
	Keyboard_Update();
	StageUpdate();
	PlayerUpdate();
	EnemyUpdate();
}

//#######################################################################
// ゲームの描画関数
void Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
    if( !pDevice ) return;
     
    // 画面のクリア
    pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(10, 40, 20, 255), 1.0f, 0);

    // 描画バッチ命令の開始
    pDevice->BeginScene();

	//ライトの設定
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);//光源処理をON

	//ビュー行列の作成
	D3DXMatrixLookAtLH(&g_mtxView,
		&g_posCamaraEye,//カメラの位置
		&g_posCamaraAt, //カメラの注視点
		&g_vecCamaraUp);//カメラの上方向

	//ビュー行列の設定
	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);

	D3DXMatrixPerspectiveFovLH(&g_mtxProjection,
		VIEW_ANGLE, //ビュー平面の視野角
		VIEW_ASPECT,//ビュー平面のアスペクト比
		VIEW_NEAR_Z,//ビュー平面のNearZ値
		VIEW_FAR_Z);//ビュー平面のFarZ値

	//プロジェクション行列の設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxProjection);	
	
	FieldDraw();
	StageDraw();
	PlayerDraw();
	EnemyDraw();


    // 描画バッチ命令の終了
    pDevice->EndScene();


	
    // バックバッファをフリップ（タイミングはD3DPRESENT_PARAMETERSの設定による）
    pDevice->Present(NULL, NULL, NULL, NULL);
}

//#######################################################################
// ゲームの終了処理
void Finalize(void)
{
    // テクスチャの解放
    Texture_Release();
    // Direct3Dラッパーモジュールの終了処理
    MyDirect3D_Finalize();

	if (g_pD3DidxBuff != NULL) {
		g_pD3DidxBuff->Release();//インターフェース解放	
		g_pD3DidxBuff = NULL;
	}


}
