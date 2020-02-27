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

Plane awa[awa_MAX];
int awaFlag[awa_MAX];
int timer[awa_MAX];

void StageInit() {
	Pold = -1;
	Pnow = -1;
	Eold = -1;
	Goal = false;

	int height = 0;
	int width = 0;

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
		awaFlag[i] = 0;
		timer[i] = 0;

		awa[i].position.x = awa[i].x;
		awa[i].position.y = 0;
		awa[i].position.z = awa[i].z;
		awa[i].rotation = D3DXVECTOR3(0, 0, 0);
		awa[i].scale = D3DXVECTOR3(2, 0, 2);
	}
	//リセットマス
	awa[10].isReset = true;
	//ゴールマス
	awa[89].isGoal = true;
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

	Pold = GetOldPlayerPos();
	Pnow = GetNowPlayerPos();
	Eold = GetOldEnemyPos();

	for (int i = 0; i < awa_MAX; i++) {
		if (Pnow == i && awa[i].isReset == true) {
			Pnow = -1;
			Pold = -1;
			for (int i = 0; i < awa_MAX; i++) {
				awa[i].isUse = true;
				awaFlag[i] = 0;
			}
		}
		if (Pnow == i && awa[i].isGoal == true) {
			Pnow = -1;
			Goal = true;
		}

		if (Pold != -1)
		{
			awa[Pold].isUse = false;
			awaFlag[Pold] += 1;
			Pold = -1;
		}
		if (Eold != -1)
		{
			awa[Eold].isUse = false;
			awaFlag[Eold] += 1;
			Eold = -1;
		}

	}

	if (Goal == true) {
		for (int i = 0; i < awa_MAX; i++) {
			if (awa[i].isUse == true) {
				awa[i].isUse = false;
				awaFlag[i] = 1;
			}
		}
		Goal = false;
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