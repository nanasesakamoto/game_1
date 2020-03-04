#include"figure.h"
#include"Stage.h"
#include"Player.h"
#include"Enemy.h"
#include<stdlib.h>
#include"sound.h"
#include"sprite.h"
#include"input.h"

static int Pold = -1;
static int Pnow = -1;
static int Eold = -1;
static bool Goal;

static int SEcount;

Plane awa[awa_MAX];
Plane Fwall[awa_MAX];
Plane Rwall[awa_MAX];
int awaFlag[awa_MAX];
int timer[awa_MAX];

void StageInit() {
	Pold = -1;
	Pnow = -1;
	Eold = -1;
	Goal = false;
	SEcount = 0;

	float height = 0;
	float width = 0;

	for (int i = 0; i < awa_MAX; i++) {
		if (width > Stage_width) {
			width = 0;
			height += 1;
		}

		awa[i].x = width * WIDE;
		awa[i].z = height * WIDE;
		width++;

		awa[i].isUse = true;
		awa[i].isReset = false;
		awa[i].isGoal = false;
		awa[i].isFWall = false;
		awa[i].isRWall = false;
		awaFlag[i] = 0;
		timer[i] = 0;

		awa[i].position.x = awa[i].x;
		awa[i].position.y = 0;
		awa[i].position.z = awa[i].z;
		awa[i].rotation = D3DXVECTOR3(0, 0, 0);
		awa[i].scale = D3DXVECTOR3(2, 0, 2);

		Fwall[i].position.x = awa[i].x;
		Fwall[i].position.y = 0.1f;
		Fwall[i].position.z = awa[i].z + 1.0f;
		Fwall[i].rotation = D3DXVECTOR3(0, 0, 0);
		Fwall[i].scale = D3DXVECTOR3(2, 0, 0.5);

		Rwall[i].position.x = awa[i].x + 1.0f;
		Rwall[i].position.y = 0.1f;
		Rwall[i].position.z = awa[i].z;
		Rwall[i].rotation = D3DXVECTOR3(0, 0, 0);
		Rwall[i].scale = D3DXVECTOR3(0.5, 0, 2);
	}
	//リセットマス
	awa[10].isReset = true;
	//ゴールマス
	awa[89].isGoal = true;
	//縦壁
	awa[10].isFWall = true;
	//横壁
	awa[11].isRWall = true;
}

void StageUpdate() {
	if (Keyboard_IsTrigger(DIK_SPACE)) {
		for (int i = 0; i < awa_MAX; i++) {
			awa[i].isUse = true;
			awaFlag[i] = 0;
		}
	}
	if (Keyboard_IsTrigger(DIK_R)) {
			StageUnInit();
			PlayerUnInit();
			EnemyUnInit();

			StageInit();
			PlayerInit();
			EnemyInit();
	}

	//プレイヤーの現在地
	Pold = GetOldPlayerPos();
	//プレイヤーの一昔前の位置
	Pnow = GetNowPlayerPos();
	//エネミーの現在地
	Eold = GetOldEnemyPos();

	for (int i = 0; i < awa_MAX; i++) {
		//リセットを踏んだ時
		if (Pnow == i && awa[i].isReset == true) {
			Pnow = -1;
			Pold = -1;
			PlaySound(SOUND_SE_RESET);
			for (int j = 0; j < awa_MAX; j++) {
				awa[j].isUse = true;
				awaFlag[j] = 0;
			}
		}
		//ゴールを踏んだ時
		if (Pnow == i && awa[i].isGoal == true && SEcount == 0) {
			Pnow = -1;
			Goal = true;
		}
		//プレイヤーが泡を踏んだ時
		if (Pold != -1 && awaFlag[Pold] == 0 && awa[Pnow].isReset == false)
		{
			awa[Pold].isUse = false;
			awaFlag[Pold] += 1;
			Pold = -1;
			PlaySound(SOUND_SE_PAAN);
		}
		//エネミーが泡を踏んだ時
		if (Eold != -1 && awaFlag[Eold] == 0 && Pnow == i && awa[Pnow].isReset == false)
		{
			awa[Eold].isUse = false;
			awaFlag[Eold] += 1;
			Eold = -1;
			PlaySound(SOUND_SE_PAAN);
		}

	}

	if (Goal == true) {
		if (awa[SEcount].isUse == true) {
			awa[SEcount].isUse = false;
			awaFlag[SEcount] = 1;
			PlaySound(SOUND_SE_PAAN);
		}
		SEcount += 1;

		if (SEcount >= awa_MAX) {
			Goal = false;
		}
	}
}

void StageDraw() {

	for (int i = 0; i < awa_MAX; i++) {
		if (awa[i].isReset == true) {
			if (awa[i].isUse == true) {
				awa[i].Draw(TEXTURE_INDEX_RESET, 255);
			}
		}
		else if (awa[i].isGoal == true) {
			awa[i].Draw(TEXTURE_INDEX_GOAL, 255);
		}
		else if (awa[i].isUse == true) {
			awa[i].Draw(TEXTURE_INDEX_MARU, 255);
		}

		if (awa[i].isUse == false && awaFlag[i] >= 1 && awaFlag[i] < 5) {
			awa[i].Draw(TEXTURE_INDEX_BURST01, 255);
			awaFlag[i]++;
		}
		if (awa[i].isUse == false && awaFlag[i] >= 5 && awaFlag[i] < 10) {
			awa[i].Draw(TEXTURE_INDEX_BURST02, 255);
			awaFlag[i]++;
		}
		if (awa[i].isUse == false && awaFlag[i] >= 10 && awaFlag[i] < 15) {
			awa[i].Draw(TEXTURE_INDEX_BURST03, 255);
			awaFlag[i]++;
		}

		if (awa[i].isFWall == true) {
			Fwall[i].Draw(TEXTURE_INDEX_WALL, 255);
		}
		if (awa[i].isRWall == true) {
			Rwall[i].Draw(TEXTURE_INDEX_WALL, 255);
		}

	}
	
}
void StageUnInit() {

}

Plane GetStagePos(int i) {
	return awa[i];
}

bool GetawaFlag(int i)
{
	return awa[i].isUse;
}
bool GetawaResetFlag(int i) {
	return awa[i].isReset;
}
bool GetFWallFlag(int i) {
	return awa[i].isFWall;
}
bool GetRWallFlag(int i) {
	return awa[i].isRWall;
}