#include"Enemy.h"
#include"Stage.h"
#include"input.h"
#include"texture.h"
#include<stdlib.h>
#include"sound.h"



Cube Enemy;
Plane NL2[awa_MAX];

static int nowPos;//���G�l�~�[�ʒu
static int oldPos;//���G�l�~�[�ʒu
static int numberDigits;//�G�l�~�[�s���p�^�[���̗���
static int enemyMovePattern;

void EnemyInit() {
	for (int i = 0; i < awa_MAX; i++) {
		NL2[i] = GetStagePos(i);
	}
	Enemy.position.x = NL2[30].x;
	Enemy.position.y = 0;
	Enemy.position.z = NL2[30].z;

	nowPos = 30;
	oldPos = -1;

	numberDigits = rand() % 3 + 1;
	enemyMovePattern = 1;
}
void EnemyUpdate() {
	if (Keyboard_IsTrigger(DIK_SPACE)) {
		oldPos = -1;
	}
	if (GetawaFlag(nowPos) == true)
	{
		if (Keyboard_IsTrigger(DIK_A)) {
			if (Enemy.position.x != 0)
				if (GetawaFlag(nowPos - 1) == true) {
					Enemy.position.x -= WIDE;
					oldPos = nowPos;
					nowPos -= 1;
					Audio::GetInstance().PlaySE("paan.wav");
				}
		}
		if (Keyboard_IsTrigger(DIK_D)) {
			if (Enemy.position.x != WIDE * Stage_width) {
				if (GetawaFlag(nowPos + 1) == true) {
					Enemy.position.x += WIDE;
					oldPos = nowPos;
					nowPos += 1;
					Audio::GetInstance().PlaySE("paan.wav");
				}
			}
		}
		if (Keyboard_IsTrigger(DIK_W)) {
			if (Enemy.position.z != WIDE * Stage_width) {
				if (GetawaFlag(nowPos + Stage_width + 1) == true) {
					Enemy.position.z += WIDE;
					oldPos = nowPos;
					nowPos += Stage_width + 1;
					Audio::GetInstance().PlaySE("paan.wav");
				}
			}
		}
		if (Keyboard_IsTrigger(DIK_S)) {
			if (Enemy.position.z != 0) {
				if (GetawaFlag(nowPos - Stage_width - 1) == true) {
					Enemy.position.z -= WIDE;
					oldPos = nowPos;
					nowPos -= Stage_width + 1;
					Audio::GetInstance().PlaySE("paan.wav");
				}
			}
		}
	}
}
void EnemyDraw(){
	Enemy.Draw(TEXTURE_INDEX_KI, D3DXCOLOR(255, 0, 0, 255));
}
void EnemyUnInit(){

}

D3DXVECTOR3 GetEnemyPos() {
	return Enemy.position;
}
int GetOldEnemyPos() {
	return oldPos;
}

void EnemyMove()
{
	switch (numberDigits)
	{
	case 1:
		if(enemyMovePattern % 4 == 0)
		{
			//��Ɉړ�
			if (Enemy.position.z != WIDE * Stage_width) {
				if (GetawaFlag(nowPos + Stage_width + 1) == true) {
					Enemy.position.z += WIDE;
					oldPos = nowPos;
					nowPos += Stage_width + 1;
					Audio::GetInstance().PlaySE("paan.wav");
				}
			}
		}
		else
		{
			//�E�Ɉړ�
			if (Enemy.position.x != WIDE * Stage_width) {
				if (GetawaFlag(nowPos + 1) == true) {
					Enemy.position.x += WIDE;
					oldPos = nowPos;
					nowPos += 1;
					Audio::GetInstance().PlaySE("paan.wav");
				}
			}
		}
		break;
	case 2:
		if (enemyMovePattern % 5 == 0)
		{
			//��Ɉړ�
			if (Enemy.position.z != WIDE * Stage_width) {
				if (GetawaFlag(nowPos + Stage_width + 1) == true) {
					Enemy.position.z += WIDE;
					oldPos = nowPos;
					nowPos += Stage_width + 1;
					Audio::GetInstance().PlaySE("paan.wav");
				}
			}
		}
		else
		{
			//�E�Ɉړ�
			if (Enemy.position.x != WIDE * Stage_width) {
				if (GetawaFlag(nowPos + 1) == true) {
					Enemy.position.x += WIDE;
					oldPos = nowPos;
					nowPos += 1;
					Audio::GetInstance().PlaySE("paan.wav");
				}
			}
		}
		break;
	case 3:
		if (enemyMovePattern % 4 == 0)
		{
			//���Ɉړ�
			if (Enemy.position.z != 0) {
				if (GetawaFlag(nowPos - Stage_width - 1) == true) {
					Enemy.position.z -= WIDE;
					oldPos = nowPos;
					nowPos -= Stage_width + 1;
					Audio::GetInstance().PlaySE("paan.wav");
				}
			}
		}
		else
		{
			//�E�Ɉړ�
			if (Enemy.position.x != WIDE * Stage_width) {
				if (GetawaFlag(nowPos + 1) == true) {
					Enemy.position.x += WIDE;
					oldPos = nowPos;
					nowPos += 1;
					Audio::GetInstance().PlaySE("paan.wav");
				}
			}
		}
		break;
	default:
		break;
	}
	enemyMovePattern++;
}