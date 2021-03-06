#pragma once

//====================================================
// 関数のプロトタイプ宣言
//====================================================
void SystemTimer_Initialize(void); // システムタイマーの初期化
void SystemTimer_Reset(void); // システムタイマーのリセット
void SystemTimer_Start(void); // システムタイマーのスタート
void SystemTimer_Stop(void); // システムタイマーのストップ
void SystemTimer_Advance(void); // システムタイマーを0.1秒進める
double SystemTimer_GetTime(void); // 計測時間の取得
double SystemTimer_GetAbsoluteTime(void); // 現在の時間を取得
float SystemTimer_GetElapsedTime(void); // 経過時間の取得
bool SystemTimer_IsStoped(void); // システムタイマーが止まっているか？
void LimitThreadAffinityToCurrentProc(void); // 現在のスレッドを1つのプロセッサ（現在のスレッド）に制限


//クラス化しましょう
class SystemTimer
{
private:
	LONGLONG TicksPerSec;					// １秒間の計測時間
	bool bTimerStopped = true;				// ストップフラグ
	LONGLONG StopTime;						// ストップした時間
	LONGLONG LastElapsedTime;				// 最後に記録した更新時間
	LONGLONG BaseTime;						// 基本時間

public:
	SystemTimer();
	void StartTimer();
	double GetTime();
	bool mUse;
};
