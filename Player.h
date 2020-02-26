#pragma once
#include"Figure.h"

void PlayerInit();
void PlayerUpdate();
void PlayerDraw();
void PlayerUnInit();

D3DXVECTOR3 GetPlayerPos();
int GetOldPlayerPos();
int GetNowPlayerPos();