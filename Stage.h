#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include"Figure.h"
const int Stage_width = 9;
const float WIDE = 2;
const int awa_MAX = 90;

void StageInit();
void StageUpdate();
void StageDraw();
void StageUnInit();

Plane GetStagePos(int i);
bool GetawaFlag(int i);
bool GetawaResetFlag(int i);
bool GetFWallFlag(int i);
bool GetRWallFlag(int i);