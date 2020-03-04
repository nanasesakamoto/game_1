#pragma once
#include"texture.h"
#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>

class Cube {
private:
	//サンプルポリゴンの表示自体に関するパラメータ
	D3DXVECTOR3 g_pos;		                      //地面の位置
	D3DXVECTOR3 g_rot;	                         //地面の向き
	D3DXVECTOR3 g_scl;                          //地面の大きさ
	D3DXMATRIX g_mtxWorld;                     //ワールドマトリクス

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

	D3DXVECTOR3 GetForward(); //正面を取得
	D3DXVECTOR3 GetRight();  //右側取得
	D3DXVECTOR3 GetUp();    //上取得
};

class Plane {
private:
	//サンプルポリゴンの表示自体に関するパラメータ
	D3DXVECTOR3 g_pos;		                      //地面の位置
	D3DXVECTOR3 g_rot;	                         //地面の向き
	D3DXVECTOR3 g_scl;                          //地面の大きさ

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
	bool isFWall;
	bool isRWall;
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

	D3DXMATRIX g_mtxWorld;                     //ワールドマトリクス

	void Draw(TextureIndex textureIndex, int Alpha);
	D3DXVECTOR3 GetForward(); //正面を取得
	D3DXVECTOR3 GetRight();  //右側取得
	D3DXVECTOR3 GetUp();    //上取得
};


class Wall {
private:
	//サンプルポリゴンの表示自体に関するパラメータ
	D3DXVECTOR3 g_pos;		                      //地面の位置
	D3DXVECTOR3 g_rot;	                         //地面の向き
	D3DXVECTOR3 g_scl;                          //地面の大きさ
	D3DXMATRIX g_mtxWorld;                     //ワールドマトリクス

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

	D3DXVECTOR3 GetForward(); //正面を取得
	D3DXVECTOR3 GetRight();  //右側取得
	D3DXVECTOR3 GetUp();    //上取得
};
