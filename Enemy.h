#pragma once
#include"Figure.h"

void EnemyInit();
void EnemyUpdate();
void EnemyDraw();
void EnemyUnInit();
void EnemyMove();

D3DXVECTOR3 GetEnemyPos();
int GetOldEnemyPos();