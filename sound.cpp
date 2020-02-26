//sound.cpp

#include "sound.h"
#include "system_timer.h"

Audio Audio::instance;

Audio::Audio()
{
	//宣言の仕方
	//宣言する必要なし

	//使用する曲をここに追加していく
	audioName =
	{
		{"paan.wav"}
	};

	//エラー判定をすべてになってくれる変数、たぶん大事
	HRESULT hr;

	//COMの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	//XAudio2の初期化
	//ここでエラーになった際はpXAudio2にロクな値が入っていないのでアクセスしないこと
	pXAudio2 = NULL;
	hr = XAudio2Create(&pXAudio2);

	//マスターボイスの取得
	//PCが音を出せる環境でない場合はここでエラーが出る可能性あり
	pMasteringVoice = NULL;
	pXAudio2->CreateMasteringVoice(&pMasteringVoice);

	//入れられたものが本当にサウンドデータなのかを判定して、正しければソースボイスを制作する
	for (int countAudio = 0; countAudio < AUDIO_COUNT; countAudio++)
			MakeSourceVoice(countAudio,countAudio);
}

Audio::~Audio()
{
	//宣言の仕方
	//宣言する必要なし

	for (int i = 0; i < pSourceVoice.size(); i++)
	{
		// ソースボイスの破棄
		pSourceVoice[i]->DestroyVoice();
		pSourceVoice[i] = NULL;

		// オーディオデータの開放

		free(pDataAudio[i]);

		pDataAudio[i] = NULL;
	}

	//マスターボイスの破棄
	pMasteringVoice->DestroyVoice();

	//XAudio2の破棄
	SAFE_RELEASE(pXAudio2);

	//COMの破棄
	CoUninitialize();
}

//BGMを流す関数、止めない限りループする
void Audio::PlayBGM(string fName,float vol,float time)
{
	//宣言の仕方
	//Audio::GetInstance().PlayBGM("使いたい曲名.wav");
	//今のところwavしか再生できないので注意

	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;
	int playFile = NULL;

	//バッファのクリア
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	//増やした配列内のソースボイスが働いていなかったら削除する
	for (int i = pSourceVoice.size(); i < AUDIO_COUNT; i--)
	{
		//後ろから現在の状態を取得する
		pSourceVoice[i - 1]->GetState(&xa2state);
		if (xa2state.BuffersQueued == 0)
		{
			//使われていなければ削除する
			pSourceVoice[i - 1]->DestroyVoice();
			pSourceVoice.pop_back();
			fileVol.pop_back();
		}
		else
		{
			break;
		}
	}


	//i番目に使いたいソースボイスが入っている
	//使用中なら複製、そうでなければここを使って音を出す
	for(int i = 0; i < AUDIO_COUNT; i++)
	{
		//宣言されたファイル名を比較
		if(fName.compare(audioName[i]) == 0)
		{
			pSourceVoice[i]->GetState(&xa2state);
			if (xa2state.BuffersQueued == 0)
			{
				//使用されていないのでここを使って曲を再生
				playFile = i;
			}
			else
			{
				//既に使用されているので新しく配列の枠を作り、そこで再生
				playFile = pSourceVoice.size();
				MakeSourceVoice(i, playFile);
			}
			break;
		}
	}

	// バッファの値設定
	buffer.AudioBytes = SizeAudio[playFile];
	buffer.pAudioData = pDataAudio[playFile];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	buffer.PlayBegin = time * 44100;

	pSourceVoice[playFile]->SubmitSourceBuffer(&buffer);

	//セットボリュームの回数を減らしたい
	if (fileVol[playFile] != vol)
	{
		fileVol[playFile] = vol;
		pSourceVoice[playFile]->SetVolume(vol);
	}

	pSourceVoice[playFile]->Start(0);
}

//SEを流す関数、1回再生され次第音を止める
void Audio::PlaySE(string fName, float vol)
{
	//宣言の仕方
	//Audio::GetInstance().PlaySE("使いたい曲名.wav");
	//今のところwavしか再生できないので注意
	
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;
	int playFile = NULL;

	//バッファのクリア
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));


	//増やした配列内のソースボイスが働いていなかったら削除する
	for (int i = pSourceVoice.size(); i > AUDIO_COUNT; i--)
	{
		//後ろから現在の状態を取得する
		pSourceVoice[i - 1]->GetState(&xa2state);
		if (xa2state.BuffersQueued == 0)
		{
			//使われていなければ削除する
			pSourceVoice[i - 1]->DestroyVoice();
			pSourceVoice.pop_back();
			fileVol.pop_back();
		}
		else
			break;
	}


	//i番目に使いたいソースボイスが入っている
	//使用中なら複製、そうでなければここを使って音を出す
	for (int i = 0; i < AUDIO_COUNT; i++)
	{
		//宣言されたファイル名を比較
		if (fName.compare(audioName[i]) == 0)
		{
			pSourceVoice[i]->GetState(&xa2state);
			if (xa2state.BuffersQueued == 0)
			{
				//使用されていないのでここを使って曲を再生
				playFile = i;
			}
			else
			{
				//既に使用されているので新しく配列の枠を作り、そこで再生
				playFile = pSourceVoice.size();
				MakeSourceVoice(i, playFile);
			}
			break;
		}
	}

	// バッファの値設定
	buffer.AudioBytes = SizeAudio[playFile];
	buffer.pAudioData = pDataAudio[playFile];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = 0;

	pSourceVoice[playFile]->SubmitSourceBuffer(&buffer);

	//セットボリュームの回数を減らしたい
	//if (fileVol[playFile] != vol)
	//{
	//	fileVol[playFile] = vol;
	//	pSourceVoice[playFile]->SetVolume(vol);
	//}
	pSourceVoice[playFile]->SetVolume(vol);
	pSourceVoice[playFile]->Start(0);
}

//今流れている曲を止める、フェード機能付き
void Audio::StopBGM(float time)
{
	//スレッド増やそう大作戦
	//ここで音量を減らしていく関数を呼び出す
	//一応fleashSourcebufferもしておく

	//float volume;
	thread fade(&Audio::FadeAudio, this, time);
	fade.detach();
}

//SEの音量を0にする関数
void Audio::ChangeVolume(string fName,float vol)
{
	//引数の曲が再生されているかどうかを調べて、されていれば音量を変更する
	for (int i = 0; i < AUDIO_COUNT; i++)
	{
		//宣言されたファイル名を比較
		if (fName.compare(audioName[i]) == 0)
		{
			pSourceVoice[i]->GetState(&xa2state);
			if (xa2state.BuffersQueued != 0)
			{
				//現在再生中の確認は取れたのでvolを変更する
				pSourceVoice[i]->SetVolume(vol);
			}
			break;
		}
	}

}

//一瞬で音を止める関数
void Audio::StopBGM(string fName)
{
	//引数の曲が再生されているかどうかを調べて、されていれば音量を変更する
	for (int i = 0; i < AUDIO_COUNT; i++)
	{
		//宣言されたファイル名を比較
		if (fName.compare(audioName[i]) == 0)
		{
			pSourceVoice[i]->GetState(&xa2state);
			if (xa2state.BuffersQueued != 0)
			{
				//現在再生中の確認は取れたので止める
				pSourceVoice[i]->Stop();
				pSourceVoice[i]->FlushSourceBuffers();
			}
			break;
		}
	}
}

//ソースボイスを作成する
void Audio::MakeSourceVoice(int fileplace, int sourceplace)
{
	//宣言の仕方
	//PlayBGM等で使っているだけなので必要なし

	HRESULT hr;
	HANDLE hFile;
	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	DWORD dwFiletype;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;

	//ソースボイス配列の枠作成
	pSourceVoice.push_back(NULL);
	pDataAudio.push_back(NULL);
	SizeAudio.push_back(NULL);

	//fileVolの初期化
	fileVol.push_back(0.0);

	//バッファのクリア
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	// サウンドデータファイルの生成
	hFile = CreateFile(FitFileName(audioName[fileplace]).c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, "一部の音楽ファイル読み込みに失敗しました。", NULL, MB_OK);	//メッセージ表示
		return;
	}
	//(LPCWSTR)FitFileName(audioName[countAudio]).c_str()

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		MessageBox(NULL, "一部の音楽ファイル読み込みに失敗しました。", NULL, MB_OK);	//メッセージ表示
		return;
	}

	// WAVEファイルのチェック
	hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "一部の音楽ファイル読み込みに失敗しました。", NULL, MB_OK);	//メッセージ表示
		return;
	}
	hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "一部の音楽ファイル読み込みに失敗しました。", NULL, MB_OK);	//メッセージ表示
		return;
	}
	if (dwFiletype != 'EVAW')
	{
		MessageBox(NULL, "一部の音楽ファイル読み込みに失敗しました。", NULL, MB_OK);	//メッセージ表示
		return;
	}

	// フォーマットチェック
	hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "一部の音楽ファイル読み込みに失敗しました。", NULL, MB_OK);	//メッセージ表示
		return;
	}
	hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "一部の音楽ファイル読み込みに失敗しました。", NULL, MB_OK);	//メッセージ表示
		return;
	}

	// オーディオデータ読み込み
	hr = CheckChunk(hFile, 'atad', &SizeAudio[sourceplace], &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "一部の音楽ファイル読み込みに失敗しました。", NULL, MB_OK);	//メッセージ表示
		return;
	}

	pDataAudio[sourceplace] = (BYTE*)malloc(SizeAudio[sourceplace]);

	hr = ReadChunkData(hFile, pDataAudio[sourceplace], SizeAudio[sourceplace], dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "一部の音楽ファイル読み込みに失敗しました。", NULL, MB_OK);	//メッセージ表示
		return;
	}

	// ソースボイスの生成
	hr = pXAudio2->CreateSourceVoice(&pSourceVoice[sourceplace], &(wfx.Format));
	if (FAILED(hr))
	{
		MessageBox(NULL, "ソースボイスの作成に失敗しました。", NULL, MB_OK);	//メッセージ表示
		return;
	}

}

//ファイル名を少しだけ簡略化するやつ
string FitFileName(string fName)
{
	string fileFrontName = ("asset/sound/");

	const string  soundName = fileFrontName += fName;

	return soundName;
}

//マルチスレッド用、ここでフェードアウトしていく
void Audio::FadeAudio(float time)
{
	XAUDIO2_VOICE_STATE xa2state;

	//どこの曲を消すかを探す
	int i;
	for (i = 0; i < pSourceVoice.size() - 1; i++)
	{
		pSourceVoice[i]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{
			//BGMを上に置いておくことで先に発見し止めてもらう予定(手抜き)
			break;
		}
	}

	//再生中のソースボイスが見つけられなかった場合は終了
	if (i == pSourceVoice.size())
		return;

	float volume = 1.0f;
	float minusVol = fileVol[i] / FADE_COUNT;

	SystemTimer S;
	float fadeTime = 0.0f;
	S.StartTimer();

	while(1)
	{
		fadeTime += S.GetTime();

		//音が完全に消えていることを確認したら抜ける
		if(fileVol[i] < 0.0f)
		{
			pSourceVoice[i]->Stop();
			// オーディオバッファの削除
			pSourceVoice[i]->FlushSourceBuffers();

			break;
		}
		if (fadeTime > time / 6)
		{
			fileVol[i] -= minusVol;
			fadeTime = 0.0f;
			pSourceVoice[i]->SetVolume(fileVol[i]);
		}
	}
	int t = 8;
}

HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}
