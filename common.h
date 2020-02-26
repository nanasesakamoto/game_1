#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>
//==============================================================================*/
#ifndef COMMON_H_
#define COMMON_H_


/*------------------------------------------------------------------------------
   定数定義
------------------------------------------------------------------------------*/
#define SCREEN_WIDTH  (1524) // スクリーン(クライアント領域)の幅
#define SCREEN_HEIGHT  (876) // スクリーン(クライアント領域)の高さ

//3Dポリゴン頂点フォーマット構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;//頂点座標
	D3DXVECTOR3 nor;//法線ベクトル
	D3DCOLOR diffuse;//反射光
	D3DXVECTOR2 tex;//テクスチャ座標
}VERTEX_3D;

//3Dポリゴン頂点フォーマット(頂点座標[3D]／法線／反射光／テクスチャ座標)
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)


#endif // COMMON_H_





