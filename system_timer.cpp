#include <Windows.h>
#include "system_timer.h"

//====================================================
// グローバル変数宣言
//====================================================
static bool g_bTimerStopped = true;			// ストップフラグ
static LONGLONG g_TicksPerSec = 0;			// １秒間の計測時間
static LONGLONG g_StopTime;					// ストップした時間
static LONGLONG g_LastElapsedTime;			// 最後に記録した更新時間
static LONGLONG g_BaseTime;					// 基本時間

//====================================================
// プロトタイプ宣言
//====================================================
// 停止していれば停止時間、そうでなければ現在の時間の取得
static LARGE_INTEGER GetAdjustedCurrentTime();
double SystemTimer_GetAbsoluteTime();

//====================================================
// 関数定義
//====================================================
// システムタイマーの初期化
void SystemTimer_Initialize()
{
    g_bTimerStopped = true;
    g_TicksPerSec = 0;
    g_StopTime = 0;
    g_LastElapsedTime = 0;
    g_BaseTime = 0;

    // 高分解能パフォーマンスカウンタ周波数の取得
    LARGE_INTEGER ticksPerSec = { 0 };
    QueryPerformanceFrequency(&ticksPerSec);
    g_TicksPerSec = ticksPerSec.QuadPart;
}

// システムタイマーのリセット
void SystemTimer_Reset()
{
    LARGE_INTEGER time = GetAdjustedCurrentTime();

    g_BaseTime = g_LastElapsedTime = time.QuadPart;
    g_StopTime = 0;
    g_bTimerStopped = false;
}

// システムタイマーのスタート
void SystemTimer_Start()
{
    // 現在の時間を取得
    LARGE_INTEGER time = { 0 };
    QueryPerformanceCounter(&time);

    // 今まで計測がストップしていたら
    if( g_bTimerStopped )
	{
        // 止まっていた時間を差し引いて基本時間を更新
        g_BaseTime += time.QuadPart - g_StopTime;
    }

    g_StopTime = 0;
    g_LastElapsedTime = time.QuadPart;
    g_bTimerStopped = false;
}

// システムタイマーのストップ
void SystemTimer_Stop()
{
    if( g_bTimerStopped ) return;

    LARGE_INTEGER time = { 0 };
    QueryPerformanceCounter(&time);

    g_LastElapsedTime = g_StopTime = time.QuadPart; // 停止時間を記録
    g_bTimerStopped = true;
}

// システムタイマーを0.1秒進める
void SystemTimer_Advance()
{
    g_StopTime += g_TicksPerSec / 10;
}

// 計測時間を取得
double SystemTimer_GetTime()
{
    LARGE_INTEGER time = GetAdjustedCurrentTime();

    return (double)(time.QuadPart - g_BaseTime) / (double)g_TicksPerSec;
}


// 経過時間の取得
float SystemTimer_GetElapsedTime()
{
    LARGE_INTEGER time = GetAdjustedCurrentTime();

    double elapsed_time = (float)((double)(time.QuadPart - g_LastElapsedTime) / (double)g_TicksPerSec);
    g_LastElapsedTime = time.QuadPart;

    // タイマーが正確であることを保証するために、更新時間を０にクランプする。
    // elapsed_timeは、プロセッサが節電モードに入るか、何らかの形で別のプロセッサにシャッフルされると、この範囲外になる可能性がある。
    // よって、メインスレッドはSetThreadAffinityMaskを呼び出して、別のプロセッサにシャッフルされないようにする必要がある。
    // 他のワーカースレッドはSetThreadAffinityMaskを呼び出すべきではなく、メインスレッドから収集されたタイマーデータの共有コピーを使用すること。
    if( elapsed_time < 0.0f )
	{
        elapsed_time = 0.0f;
    }

    return (float)elapsed_time;
}

// システムタイマーが止まっているか？
bool SystemTimer_IsStoped()
{
    return g_bTimerStopped;
}

// 現在のスレッドを1つのプロセッサ（現在のスレッド）に制限
void LimitThreadAffinityToCurrentProc()
{
    HANDLE hCurrentProcess = GetCurrentProcess();

    // Get the processor affinity mask for this process
    DWORD_PTR dwProcessAffinityMask = 0;
    DWORD_PTR dwSystemAffinityMask = 0;

    if( GetProcessAffinityMask(hCurrentProcess, &dwProcessAffinityMask, &dwSystemAffinityMask) != 0 && dwProcessAffinityMask ) {
        // Find the lowest processor that our process is allows to run against
        DWORD_PTR dwAffinityMask = (dwProcessAffinityMask & ((~dwProcessAffinityMask) + 1));

        // Set this as the processor that our thread must always run against
        // This must be a subset of the process affinity mask
        HANDLE hCurrentThread = GetCurrentThread();
        if( INVALID_HANDLE_VALUE != hCurrentThread ) {
            SetThreadAffinityMask(hCurrentThread, dwAffinityMask);
            CloseHandle(hCurrentThread);
        }
    }
    CloseHandle(hCurrentProcess);
}


SystemTimer::SystemTimer()
{
	bTimerStopped = true;
	StopTime = 0;
	LastElapsedTime = 0;
	BaseTime = 0;
	mUse = false;

	// 高分解能パフォーマンスカウンタ周波数の取得
	LARGE_INTEGER ticksPerSec = { 0 };
	QueryPerformanceFrequency(&ticksPerSec);
	TicksPerSec = ticksPerSec.QuadPart;
}

void SystemTimer::StartTimer()
{
	//インスタンスの生成と同時にこの関数を宣言してタイマーをスタートする。

	// 現在の時間を取得
	LARGE_INTEGER time = { 0 };
	QueryPerformanceCounter(&time);

	// 今まで計測がストップしていたら
	if (bTimerStopped)
	{
		// 止まっていた時間を差し引いて基本時間を更新
		BaseTime += time.QuadPart - StopTime;
	}

	mUse = true;
	StopTime = 0;
	LastElapsedTime = time.QuadPart;
	bTimerStopped = false;
}

double SystemTimer::GetTime()
{
	//単位は秒(1.0で1秒)

	LARGE_INTEGER time = GetAdjustedCurrentTime();

	double elapsed_time = (float)((double)(time.QuadPart - LastElapsedTime) / (double)TicksPerSec);
	LastElapsedTime = time.QuadPart;

	// タイマーが正確であることを保証するために、更新時間を０にクランプする。
	// elapsed_timeは、プロセッサが節電モードに入るか、何らかの形で別のプロセッサにシャッフルされると、この範囲外になる可能性がある。
	// よって、メインスレッドはSetThreadAffinityMaskを呼び出して、別のプロセッサにシャッフルされないようにする必要がある。
	// 他のワーカースレッドはSetThreadAffinityMaskを呼び出すべきではなく、メインスレッドから収集されたタイマーデータの共有コピーを使用すること。
	if (elapsed_time < 0.0f)
	{
		elapsed_time = 0.0f;
	}

	return (float)elapsed_time;
}



// 停止していれば停止時間、そうでなければ現在の時間の取得
LARGE_INTEGER GetAdjustedCurrentTime()
{
	LARGE_INTEGER time;
	if (g_StopTime != 0)
	{
		time.QuadPart = g_StopTime;
	}
	else
	{
		QueryPerformanceCounter(&time);
	}
	return time;
}

// 現在の時間を取得
double SystemTimer_GetAbsoluteTime()
{
	LARGE_INTEGER time = { 0 };
	QueryPerformanceCounter(&time);

	return time.QuadPart / (double)g_TicksPerSec;
}
