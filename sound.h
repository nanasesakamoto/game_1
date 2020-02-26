#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "system_timer.h"
#include <thread>

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

#define SAFE_RELEASE(p)		{ if(p!=NULL) { (p)->Release(); (p) = NULL; } }	//zeromemoryみたいなもん
#define AUDIO_COUNT			(1)		//曲の数
#define FADE_COUNT			(6)			//フェードアウトするために呼び出すSetVolumeの回数

using  std::string;
using  std::vector;
using  std::thread;

class Audio
{
private:
	Audio();
	~Audio();

	vector<string> audioName;

	IXAudio2* pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* pMasteringVoice;			// マスターボイス
	vector<IXAudio2SourceVoice*> pSourceVoice;			// ソースボイス
	vector<BYTE*> pDataAudio;							// オーディオデータ
	vector<DWORD> SizeAudio;							// オーディオデータサイズ

	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	vector<float> fileVol;

	static Audio instance;
	void MakeSourceVoice(int fileplace, int sourceplace);
	void FadeAudio(float time = 3.0f);

public:
	void PlayBGM(string fName, float vol = 1.0f,float time = 0.0f);
	void PlaySE(string fName, float vol = 1.0f);
	void StopBGM(float time = 2.0f);
	void StopBGM(string fName);
	void ChangeVolume(string fName, float vol);

	static Audio& GetInstance() { return instance; }
};

string FitFileName(string fName);

HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);

HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

