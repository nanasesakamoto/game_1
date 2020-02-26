

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
//������ǉ��@3D�p�ǉ��R�[�h

//�v���W�F�N�g�p�p�����[�^
#define VIEW_ANGLE (D3DXToRadian(45.0f))//�r���[���ʂ̎���p
#define VIEW_ASPECT ((float)SCREEN_WIDTH/(float)SCREEN_HEIGHT)//�r���[���ʂ̃A�X�y�N�g��
#define VIEW_NEAR_Z (1.0f)//�r���[���ʂ�NearZ�l
#define VIEW_FAR_Z (1000.0f)//�r���[���ʂ�FarZ�l

//�J�����p�p�����[�^
D3DXVECTOR3 g_posCamaraEye;//�J�����̎��_
D3DXVECTOR3 g_posCamaraAt;//�J�����̒����_
D3DXVECTOR3 g_vecCamaraUp;//�J�����̏����

//�v���W�F�N�V�������J�����s��
D3DXMATRIX g_mtxView;//�r���[�}�g���b�N�X
D3DXMATRIX g_mtxProjection;//�v���W�F�N�V�����}�g���b�N�X

//�T���v���|���S���̕\�����̂Ɋւ���p�����[�^
D3DXVECTOR3 g_pos;//�|���S���̈ʒu(�l�p�`�̒������W)
D3DXVECTOR3 g_rot;//�|���S���̌���(��])
D3DXVECTOR3 g_scl;//�|���S���̑傫��(�X�P�[��)

D3DXMATRIX g_mtxWorld;//���[���h�}�g���b�N�X

LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuff; //���_�o�b�t�@
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuff1; //�̒��_�o�b�t�@



//�����܂�3D�p�ǉ��R�[�h
//===============================================



/*------------------------------------------------------------------------------
   �萔��`
------------------------------------------------------------------------------*/
#define CLASS_NAME     "GameWindow"       // �E�C���h�E�N���X�̖��O
#define WINDOW_CAPTION "�Q�[���E�B���h�E" // �E�B���h�E�̖��O
#define FPS_STANTARD 60.0f				  // �t���[�����[�g


float pos_x = 0.0f;
float posadd_x = 0.1f;

float Camerapos_x = 10.0f, Camerapos_y = 25.0f, Camerapos_z = -10.0f;

/*------------------------------------------------------------------------------
   �v���g�^�C�v�錾
------------------------------------------------------------------------------*/
// �E�B���h�E�v���V�[�W��(�R�[���o�b�N�֐�)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// �Q�[���̏������֐�
// �߂�l:�������Ɏ��s�����Ƃ�false
static bool Initialize(void);
// �Q�[���̍X�V�֐�
static void Update(void);
// �Q�[���̕`��֐�
static void Draw(void);
// �Q�[���̏I������
static void Finalize(void);


/*------------------------------------------------------------------------------
   �O���[�o���ϐ��錾
------------------------------------------------------------------------------*/
static HWND g_hWnd;             // �E�B���h�E�n���h��
static float g_Rotation = 0.0f; // �X�v���C�g��]�p�x
static float number = 0;

D3DLIGHT9 g_Light; //���C�g�\����

//�C���^�[�t�F�[�X�̃|�C���^�[�̓��ꕨ��p��
LPDIRECT3DINDEXBUFFER9 g_pD3DidxBuff;

static int g_FrameCount = 0; // �t���[���J�E���^�[
static double g_StaticFrameTime = 0.0f; // �t���[���Œ�p�v������

/*------------------------------------------------------------------------------
   �֐���`
------------------------------------------------------------------------------*/

//#######################################################################
// ���C���֐�
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // �g�p���Ȃ��ꎞ�ϐ��𖾎�
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // �E�B���h�E�N���X�\���̂̐ݒ�
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;                          // �E�B���h�E�v���V�[�W���̎w��
    wc.lpszClassName = CLASS_NAME;                     // �N���X���̐ݒ�
    wc.hInstance = hInstance;                          // �C���X�^���X�n���h���̎w��
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);          // �}�E�X�J�[�\�����w��
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); // �E�C���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�

    // �N���X�o�^
    RegisterClass(&wc);


    // �E�B���h�E�X�^�C��
    DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);

    // ��{��`���W
    RECT window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    // �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɐV���ȋ�`���W���v�Z
    AdjustWindowRect(&window_rect, window_style, FALSE);

    // �V����Window�̋�`���W���畝�ƍ������Z�o
    int window_width = window_rect.right - window_rect.left;
    int window_height = window_rect.bottom - window_rect.top;

    // �v���C�}�����j�^�[�̉�ʉ𑜓x�擾
    int desktop_width = GetSystemMetrics(SM_CXSCREEN);
    int desktop_height = GetSystemMetrics(SM_CYSCREEN);

    // �f�X�N�g�b�v�̐^�񒆂ɃE�B���h�E�����������悤�ɍ��W���v�Z
    // ��������������A�f�X�N�g�b�v���E�B���h�E���傫���ꍇ�͍���ɕ\��
    int window_x = max((desktop_width - window_width) / 2, 0);
    int window_y = max((desktop_height - window_height) / 2, 0);

    // �E�B���h�E�̐���
    g_hWnd = CreateWindow(
        CLASS_NAME,     // �E�B���h�E�N���X
        WINDOW_CAPTION, // �E�B���h�E�e�L�X�g
        window_style,   // �E�B���h�E�X�^�C��
        window_x,       // �E�B���h�E���Wx
        window_y,       // �E�B���h�E���Wy
        window_width,   // �E�B���h�E�̕�
        window_height,  // �E�B���h�E�̍���
        NULL,           // �e�E�B���h�E�n���h��
        NULL,           // ���j���[�n���h��
        hInstance,      // �C���X�^���X�n���h��
        NULL            // �ǉ��̃A�v���P�[�V�����f�[�^
    );

    if( g_hWnd == NULL ) {
        // �E�B���h�E�n���h�������炩�̗��R�Ő����o���Ȃ�����
        return -1;
    }

    // �w��̃E�B���h�E�n���h���̃E�B���h�E���w��̕��@�ŕ\��
    ShowWindow(g_hWnd, nCmdShow);


	Keyboard_Initialize(hInstance, g_hWnd);

	// Windows�Q�[���p���C�����[�v
	MSG msg = {}; // msg.message == WM_NULL

	// �Q�[���̏�����(Direct3D�̏�����)
	if( !Initialize() ) {
        // �Q�[���̏������Ɏ��s����
		return -1;
	}

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// ���b�Z�[�W������ꍇ�̓��b�Z�[�W������D��
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// ���݂̃V�X�e�����Ԃ��擾
			double time = SystemTimer_GetTime();

			if (time - g_StaticFrameTime < 1.0 / FPS_STANTARD)
			{
				// 1 / 60 �b�o���Ă��Ȃ����������
				Sleep(0);
			}
			else
			{
				// �t���[���Œ�p�̌v�����Ԃ��X�V����
				g_StaticFrameTime += 1.0 / 60;// �Q�[���������s�������Ԃ��L�^
				// �Q�[���̍X�V
				Update();
				// �Q�[���̕`��
				Draw();
			}
		}
	}

	Keyboard_Finalize();
	// �Q�[���̏I������(Direct3D�̏I������)
	Finalize();

    return (int)msg.wParam;
}

//#######################################################################
// �E�B���h�E�v���V�[�W��(�R�[���o�b�N�֐�)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch( uMsg ) {
        case WM_KEYDOWN:
            if( wParam == VK_ESCAPE ) {
                SendMessage(hWnd, WM_CLOSE, 0, 0); // WM_CLOSE���b�Z�[�W�̑��M
            }
            break;

        case WM_CLOSE:
                DestroyWindow(hWnd); // �w��̃E�B���h�E��WM_DESTROY���b�Z�[�W�𑗂�
            return 0; // DefWindowProc�֐��Ƀ��b�Z�[�W�𗬂����I�����邱�Ƃɂ���ĉ����Ȃ��������Ƃɂ���

        case WM_DESTROY: // �E�B���h�E�̔j�����b�Z�[�W
            PostQuitMessage(0); // WM_QUIT���b�Z�[�W�̑��M
            return 0;
    };

    // �ʏ탁�b�Z�[�W�����͂��̊֐��ɔC����
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//#######################################################################
// �Q�[���̏������֐�
bool Initialize(void)
{
	//���C�g�̐ݒ�
	ZeroMemory(&g_Light, sizeof(D3DLIGHT9));//�\���̂��O�ŃN���A(�K�{)
	g_Light.Type = D3DLIGHT_DIRECTIONAL;//���C�g�̎��
	g_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//���̐FRGBA
	g_Light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�����̐F
	D3DXVECTOR3 vecDir = D3DXVECTOR3(0.0f, -0.6f, 1.0f);//���̎w�������x�N�g��
	D3DXVec3Normalize(&vecDir, &vecDir);
	g_Light.Direction = vecDir;//���K�����ăZ�b�g


	// Direct3D���b�p�[���W���[���̏�����
	if (!MyDirect3D_Initialize(g_hWnd)) {
		return false;
	}

	//�J����
	g_posCamaraEye = D3DXVECTOR3(Camerapos_x, Camerapos_y, Camerapos_z);
	g_posCamaraAt = D3DXVECTOR3(10, 0, 5);
	g_vecCamaraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	if (Texture_Load() > 0) {
		MessageBox(g_hWnd, "�������ǂݍ��߂Ȃ������e�N�X�`���t�@�C��������܂�", "�G���[", MB_OK);
	}

	FieldInit();
	StageInit();
	PlayerInit();
	EnemyInit();

    return true;
}

//#######################################################################
// �Q�[���̍X�V�֐�
void Update(void)
{
	Keyboard_Update();
	StageUpdate();
	PlayerUpdate();
	EnemyUpdate();
}

//#######################################################################
// �Q�[���̕`��֐�
void Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
    if( !pDevice ) return;
     
    // ��ʂ̃N���A
    pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(10, 40, 20, 255), 1.0f, 0);

    // �`��o�b�`���߂̊J�n
    pDevice->BeginScene();

	//���C�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);//����������ON

	//�r���[�s��̍쐬
	D3DXMatrixLookAtLH(&g_mtxView,
		&g_posCamaraEye,//�J�����̈ʒu
		&g_posCamaraAt, //�J�����̒����_
		&g_vecCamaraUp);//�J�����̏����

	//�r���[�s��̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);

	D3DXMatrixPerspectiveFovLH(&g_mtxProjection,
		VIEW_ANGLE, //�r���[���ʂ̎���p
		VIEW_ASPECT,//�r���[���ʂ̃A�X�y�N�g��
		VIEW_NEAR_Z,//�r���[���ʂ�NearZ�l
		VIEW_FAR_Z);//�r���[���ʂ�FarZ�l

	//�v���W�F�N�V�����s��̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxProjection);	
	
	FieldDraw();
	StageDraw();
	PlayerDraw();
	EnemyDraw();


    // �`��o�b�`���߂̏I��
    pDevice->EndScene();


	
    // �o�b�N�o�b�t�@���t���b�v�i�^�C�~���O��D3DPRESENT_PARAMETERS�̐ݒ�ɂ��j
    pDevice->Present(NULL, NULL, NULL, NULL);
}

//#######################################################################
// �Q�[���̏I������
void Finalize(void)
{
    // �e�N�X�`���̉��
    Texture_Release();
    // Direct3D���b�p�[���W���[���̏I������
    MyDirect3D_Finalize();

	if (g_pD3DidxBuff != NULL) {
		g_pD3DidxBuff->Release();//�C���^�[�t�F�[�X���	
		g_pD3DidxBuff = NULL;
	}


}
