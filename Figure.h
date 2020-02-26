#pragma once
#include"texture.h"
#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>

class Cube {
private:
	//�T���v���|���S���̕\�����̂Ɋւ���p�����[�^
	D3DXVECTOR3 g_pos;		                      //�n�ʂ̈ʒu
	D3DXVECTOR3 g_rot;	                         //�n�ʂ̌���
	D3DXVECTOR3 g_scl;                          //�n�ʂ̑傫��
	D3DXMATRIX g_mtxWorld;                     //���[���h�}�g���N�X

public:
	D3DXVECTOR3 position{0,0,0};
	D3DXVECTOR3 rotation{0,0,0};
	D3DXVECTOR3 scale{1,1,1};
	bool isDraw = false;
	Cube(){isDraw = false; }
	Cube(D3DXVECTOR3 pos) {
		position = pos;
		isDraw = false;
	}
	void Draw(TextureIndex textureIndex, D3DXCOLOR color={1,1,1,1});

	D3DXVECTOR3 GetForward(); //���ʂ��擾
	D3DXVECTOR3 GetRight();  //�E���擾
	D3DXVECTOR3 GetUp();    //��擾
};

class Plane {
private:
	//�T���v���|���S���̕\�����̂Ɋւ���p�����[�^
	D3DXVECTOR3 g_pos;		                      //�n�ʂ̈ʒu
	D3DXVECTOR3 g_rot;	                         //�n�ʂ̌���
	D3DXVECTOR3 g_scl;                          //�n�ʂ̑傫��

public:
	D3DXVECTOR3 position{ 0,0,0 };
	D3DXVECTOR3 rotation{ 0,0,0 };
	D3DXVECTOR3 scale{ 1,1,1 };
	bool isDraw = false;
	float x;
	float z;
	bool isUse;
	bool isReset;
	bool isGoal;
	Plane(){ isDraw = false; }
	Plane(D3DXVECTOR3 pos, D3DXVECTOR3 size) {
		position = pos;
		scale = size;
		isDraw = false;
	}
	Plane(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size) {
		position = pos;
		scale = size;
		rotation = rot;
		isDraw = false;
	}

	D3DXMATRIX g_mtxWorld;                     //���[���h�}�g���N�X

	void Draw(TextureIndex textureIndex, int Alpha);
	D3DXVECTOR3 GetForward(); //���ʂ��擾
	D3DXVECTOR3 GetRight();  //�E���擾
	D3DXVECTOR3 GetUp();    //��擾
};


class Wall {
private:
	//�T���v���|���S���̕\�����̂Ɋւ���p�����[�^
	D3DXVECTOR3 g_pos;		                      //�n�ʂ̈ʒu
	D3DXVECTOR3 g_rot;	                         //�n�ʂ̌���
	D3DXVECTOR3 g_scl;                          //�n�ʂ̑傫��
	D3DXMATRIX g_mtxWorld;                     //���[���h�}�g���N�X

public:
	D3DXVECTOR3 position{ 0,0,0 };
	D3DXVECTOR3 rotation{ 0,0,0 };
	D3DXVECTOR3 scale{ 1,1,1 };
	bool isDraw = false;
	Wall() { isDraw = false; }
	Wall(D3DXVECTOR3 pos) {
		position = pos;
		isDraw = false;
	}
	void Draw(TextureIndex textureIndex, D3DXCOLOR color = { 1,1,1,1 });

	D3DXVECTOR3 GetForward(); //���ʂ��擾
	D3DXVECTOR3 GetRight();  //�E���擾
	D3DXVECTOR3 GetUp();    //��擾
};