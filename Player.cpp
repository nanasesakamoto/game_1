#include"Player.h"
#include"Stage.h"
#include"input.h"
#include"texture.h"
#include<stdlib.h>
#include"sound.h"
#include"Enemy.h"


Cube Player;
Plane NL[awa_MAX];

static int nowPos;//現プレイヤー位置
static int oldPos;//旧プレイヤー位置

void PlayerInit() {
	for (int i = 0; i < awa_MAX; i++) {
		NL[i] = GetStagePos(i);
	}
	Player.position.x = NL[3].x;
	Player.position.y =0;
	Player.position.z = NL[3].z;

	nowPos = 3;
	oldPos = -1;
}
void PlayerUpdate() {
	if (nowPos == 10) {
		oldPos = -1;
	}
	if (GetawaFlag(nowPos) == true)
	{
		if (Keyboard_IsTrigger(DIK_LEFT)) {
			if (Player.position.x != 0)
				if (GetawaFlag(nowPos - 1) == true) {
					Player.position.x -= WIDE;
					oldPos = nowPos;
					nowPos -= 1;
					Audio::GetInstance().PlaySE("paan.wav");
					EnemyMove();
				}
		}
		if (Keyboard_IsTrigger(DIK_RIGHT)) {
			if (Player.position.x != WIDE * Stage_width) {
				if (GetawaFlag(nowPos + 1) == true) {
					Player.position.x += WIDE;
					oldPos = nowPos;
					nowPos += 1;
					Audio::GetInstance().PlaySE("paan.wav");
					EnemyMove();
				}
			}
		}
		if (Keyboard_IsTrigger(DIK_UP)) {
			if (Player.position.z != WIDE * Stage_width) {
				if (GetawaFlag(nowPos + Stage_width + 1) == true) {
					Player.position.z += WIDE;
					oldPos = nowPos;
					nowPos += Stage_width + 1;
					Audio::GetInstance().PlaySE("paan.wav");
					EnemyMove();
				}
			}
		}
		if (Keyboard_IsTrigger(DIK_DOWN)) {
			if (Player.position.z != 0) {
				if (GetawaFlag(nowPos - Stage_width - 1) == true) {
					Player.position.z -= WIDE;
					oldPos = nowPos;
					nowPos -= Stage_width + 1;
					Audio::GetInstance().PlaySE("paan.wav");
					EnemyMove();
				}
			}
		}
	}
}

void PlayerDraw() {
	Player.Draw(TEXTURE_INDEX_KI, D3DXCOLOR(255, 255, 255, 255));
}
void PlayerUnInit() {

}

D3DXVECTOR3 GetPlayerPos() {
	return Player.position;
}

int GetOldPlayerPos() {
	return oldPos;
}

int GetNowPlayerPos() {
	return nowPos;
}