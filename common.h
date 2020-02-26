#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>
//==============================================================================*/
#ifndef COMMON_H_
#define COMMON_H_


/*------------------------------------------------------------------------------
   �萔��`
------------------------------------------------------------------------------*/
#define SCREEN_WIDTH  (1524) // �X�N���[��(�N���C�A���g�̈�)�̕�
#define SCREEN_HEIGHT  (876) // �X�N���[��(�N���C�A���g�̈�)�̍���

//3D�|���S�����_�t�H�[�}�b�g�\���̂��`
typedef struct
{
	D3DXVECTOR3 vtx;//���_���W
	D3DXVECTOR3 nor;//�@���x�N�g��
	D3DCOLOR diffuse;//���ˌ�
	D3DXVECTOR2 tex;//�e�N�X�`�����W
}VERTEX_3D;

//3D�|���S�����_�t�H�[�}�b�g(���_���W[3D]�^�@���^���ˌ��^�e�N�X�`�����W)
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)


#endif // COMMON_H_





