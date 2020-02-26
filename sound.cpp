//sound.cpp

#include "sound.h"
#include "system_timer.h"

Audio Audio::instance;

Audio::Audio()
{
	//�錾�̎d��
	//�錾����K�v�Ȃ�

	//�g�p����Ȃ������ɒǉ����Ă���
	audioName =
	{
		{"paan.wav"}
	};

	//�G���[��������ׂĂɂȂ��Ă����ϐ��A���Ԃ�厖
	HRESULT hr;

	//COM�̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	//XAudio2�̏�����
	//�����ŃG���[�ɂȂ����ۂ�pXAudio2�Ƀ��N�Ȓl�������Ă��Ȃ��̂ŃA�N�Z�X���Ȃ�����
	pXAudio2 = NULL;
	hr = XAudio2Create(&pXAudio2);

	//�}�X�^�[�{�C�X�̎擾
	//PC�������o������łȂ��ꍇ�͂����ŃG���[���o��\������
	pMasteringVoice = NULL;
	pXAudio2->CreateMasteringVoice(&pMasteringVoice);

	//�����ꂽ���̂��{���ɃT�E���h�f�[�^�Ȃ̂��𔻒肵�āA��������΃\�[�X�{�C�X�𐧍삷��
	for (int countAudio = 0; countAudio < AUDIO_COUNT; countAudio++)
			MakeSourceVoice(countAudio,countAudio);
}

Audio::~Audio()
{
	//�錾�̎d��
	//�錾����K�v�Ȃ�

	for (int i = 0; i < pSourceVoice.size(); i++)
	{
		// �\�[�X�{�C�X�̔j��
		pSourceVoice[i]->DestroyVoice();
		pSourceVoice[i] = NULL;

		// �I�[�f�B�I�f�[�^�̊J��

		free(pDataAudio[i]);

		pDataAudio[i] = NULL;
	}

	//�}�X�^�[�{�C�X�̔j��
	pMasteringVoice->DestroyVoice();

	//XAudio2�̔j��
	SAFE_RELEASE(pXAudio2);

	//COM�̔j��
	CoUninitialize();
}

//BGM�𗬂��֐��A�~�߂Ȃ����胋�[�v����
void Audio::PlayBGM(string fName,float vol,float time)
{
	//�錾�̎d��
	//Audio::GetInstance().PlayBGM("�g�������Ȗ�.wav");
	//���̂Ƃ���wav�����Đ��ł��Ȃ��̂Œ���

	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;
	int playFile = NULL;

	//�o�b�t�@�̃N���A
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	//���₵���z����̃\�[�X�{�C�X�������Ă��Ȃ�������폜����
	for (int i = pSourceVoice.size(); i < AUDIO_COUNT; i--)
	{
		//��납�猻�݂̏�Ԃ��擾����
		pSourceVoice[i - 1]->GetState(&xa2state);
		if (xa2state.BuffersQueued == 0)
		{
			//�g���Ă��Ȃ���΍폜����
			pSourceVoice[i - 1]->DestroyVoice();
			pSourceVoice.pop_back();
			fileVol.pop_back();
		}
		else
		{
			break;
		}
	}


	//i�ԖڂɎg�������\�[�X�{�C�X�������Ă���
	//�g�p���Ȃ畡���A�����łȂ���΂������g���ĉ����o��
	for(int i = 0; i < AUDIO_COUNT; i++)
	{
		//�錾���ꂽ�t�@�C�������r
		if(fName.compare(audioName[i]) == 0)
		{
			pSourceVoice[i]->GetState(&xa2state);
			if (xa2state.BuffersQueued == 0)
			{
				//�g�p����Ă��Ȃ��̂ł������g���ċȂ��Đ�
				playFile = i;
			}
			else
			{
				//���Ɏg�p����Ă���̂ŐV�����z��̘g�����A�����ōĐ�
				playFile = pSourceVoice.size();
				MakeSourceVoice(i, playFile);
			}
			break;
		}
	}

	// �o�b�t�@�̒l�ݒ�
	buffer.AudioBytes = SizeAudio[playFile];
	buffer.pAudioData = pDataAudio[playFile];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	buffer.PlayBegin = time * 44100;

	pSourceVoice[playFile]->SubmitSourceBuffer(&buffer);

	//�Z�b�g�{�����[���̉񐔂����炵����
	if (fileVol[playFile] != vol)
	{
		fileVol[playFile] = vol;
		pSourceVoice[playFile]->SetVolume(vol);
	}

	pSourceVoice[playFile]->Start(0);
}

//SE�𗬂��֐��A1��Đ����ꎟ�批���~�߂�
void Audio::PlaySE(string fName, float vol)
{
	//�錾�̎d��
	//Audio::GetInstance().PlaySE("�g�������Ȗ�.wav");
	//���̂Ƃ���wav�����Đ��ł��Ȃ��̂Œ���
	
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;
	int playFile = NULL;

	//�o�b�t�@�̃N���A
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));


	//���₵���z����̃\�[�X�{�C�X�������Ă��Ȃ�������폜����
	for (int i = pSourceVoice.size(); i > AUDIO_COUNT; i--)
	{
		//��납�猻�݂̏�Ԃ��擾����
		pSourceVoice[i - 1]->GetState(&xa2state);
		if (xa2state.BuffersQueued == 0)
		{
			//�g���Ă��Ȃ���΍폜����
			pSourceVoice[i - 1]->DestroyVoice();
			pSourceVoice.pop_back();
			fileVol.pop_back();
		}
		else
			break;
	}


	//i�ԖڂɎg�������\�[�X�{�C�X�������Ă���
	//�g�p���Ȃ畡���A�����łȂ���΂������g���ĉ����o��
	for (int i = 0; i < AUDIO_COUNT; i++)
	{
		//�錾���ꂽ�t�@�C�������r
		if (fName.compare(audioName[i]) == 0)
		{
			pSourceVoice[i]->GetState(&xa2state);
			if (xa2state.BuffersQueued == 0)
			{
				//�g�p����Ă��Ȃ��̂ł������g���ċȂ��Đ�
				playFile = i;
			}
			else
			{
				//���Ɏg�p����Ă���̂ŐV�����z��̘g�����A�����ōĐ�
				playFile = pSourceVoice.size();
				MakeSourceVoice(i, playFile);
			}
			break;
		}
	}

	// �o�b�t�@�̒l�ݒ�
	buffer.AudioBytes = SizeAudio[playFile];
	buffer.pAudioData = pDataAudio[playFile];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = 0;

	pSourceVoice[playFile]->SubmitSourceBuffer(&buffer);

	//�Z�b�g�{�����[���̉񐔂����炵����
	//if (fileVol[playFile] != vol)
	//{
	//	fileVol[playFile] = vol;
	//	pSourceVoice[playFile]->SetVolume(vol);
	//}
	pSourceVoice[playFile]->SetVolume(vol);
	pSourceVoice[playFile]->Start(0);
}

//������Ă���Ȃ��~�߂�A�t�F�[�h�@�\�t��
void Audio::StopBGM(float time)
{
	//�X���b�h���₻������
	//�����ŉ��ʂ����炵�Ă����֐����Ăяo��
	//�ꉞfleashSourcebuffer�����Ă���

	//float volume;
	thread fade(&Audio::FadeAudio, this, time);
	fade.detach();
}

//SE�̉��ʂ�0�ɂ���֐�
void Audio::ChangeVolume(string fName,float vol)
{
	//�����̋Ȃ��Đ�����Ă��邩�ǂ����𒲂ׂāA����Ă���Ή��ʂ�ύX����
	for (int i = 0; i < AUDIO_COUNT; i++)
	{
		//�錾���ꂽ�t�@�C�������r
		if (fName.compare(audioName[i]) == 0)
		{
			pSourceVoice[i]->GetState(&xa2state);
			if (xa2state.BuffersQueued != 0)
			{
				//���ݍĐ����̊m�F�͎�ꂽ�̂�vol��ύX����
				pSourceVoice[i]->SetVolume(vol);
			}
			break;
		}
	}

}

//��u�ŉ����~�߂�֐�
void Audio::StopBGM(string fName)
{
	//�����̋Ȃ��Đ�����Ă��邩�ǂ����𒲂ׂāA����Ă���Ή��ʂ�ύX����
	for (int i = 0; i < AUDIO_COUNT; i++)
	{
		//�錾���ꂽ�t�@�C�������r
		if (fName.compare(audioName[i]) == 0)
		{
			pSourceVoice[i]->GetState(&xa2state);
			if (xa2state.BuffersQueued != 0)
			{
				//���ݍĐ����̊m�F�͎�ꂽ�̂Ŏ~�߂�
				pSourceVoice[i]->Stop();
				pSourceVoice[i]->FlushSourceBuffers();
			}
			break;
		}
	}
}

//�\�[�X�{�C�X���쐬����
void Audio::MakeSourceVoice(int fileplace, int sourceplace)
{
	//�錾�̎d��
	//PlayBGM���Ŏg���Ă��邾���Ȃ̂ŕK�v�Ȃ�

	HRESULT hr;
	HANDLE hFile;
	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	DWORD dwFiletype;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;

	//�\�[�X�{�C�X�z��̘g�쐬
	pSourceVoice.push_back(NULL);
	pDataAudio.push_back(NULL);
	SizeAudio.push_back(NULL);

	//fileVol�̏�����
	fileVol.push_back(0.0);

	//�o�b�t�@�̃N���A
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	// �T�E���h�f�[�^�t�@�C���̐���
	hFile = CreateFile(FitFileName(audioName[fileplace]).c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, "�ꕔ�̉��y�t�@�C���ǂݍ��݂Ɏ��s���܂����B", NULL, MB_OK);	//���b�Z�[�W�\��
		return;
	}
	//(LPCWSTR)FitFileName(audioName[countAudio]).c_str()

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		MessageBox(NULL, "�ꕔ�̉��y�t�@�C���ǂݍ��݂Ɏ��s���܂����B", NULL, MB_OK);	//���b�Z�[�W�\��
		return;
	}

	// WAVE�t�@�C���̃`�F�b�N
	hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�ꕔ�̉��y�t�@�C���ǂݍ��݂Ɏ��s���܂����B", NULL, MB_OK);	//���b�Z�[�W�\��
		return;
	}
	hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�ꕔ�̉��y�t�@�C���ǂݍ��݂Ɏ��s���܂����B", NULL, MB_OK);	//���b�Z�[�W�\��
		return;
	}
	if (dwFiletype != 'EVAW')
	{
		MessageBox(NULL, "�ꕔ�̉��y�t�@�C���ǂݍ��݂Ɏ��s���܂����B", NULL, MB_OK);	//���b�Z�[�W�\��
		return;
	}

	// �t�H�[�}�b�g�`�F�b�N
	hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�ꕔ�̉��y�t�@�C���ǂݍ��݂Ɏ��s���܂����B", NULL, MB_OK);	//���b�Z�[�W�\��
		return;
	}
	hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�ꕔ�̉��y�t�@�C���ǂݍ��݂Ɏ��s���܂����B", NULL, MB_OK);	//���b�Z�[�W�\��
		return;
	}

	// �I�[�f�B�I�f�[�^�ǂݍ���
	hr = CheckChunk(hFile, 'atad', &SizeAudio[sourceplace], &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�ꕔ�̉��y�t�@�C���ǂݍ��݂Ɏ��s���܂����B", NULL, MB_OK);	//���b�Z�[�W�\��
		return;
	}

	pDataAudio[sourceplace] = (BYTE*)malloc(SizeAudio[sourceplace]);

	hr = ReadChunkData(hFile, pDataAudio[sourceplace], SizeAudio[sourceplace], dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�ꕔ�̉��y�t�@�C���ǂݍ��݂Ɏ��s���܂����B", NULL, MB_OK);	//���b�Z�[�W�\��
		return;
	}

	// �\�[�X�{�C�X�̐���
	hr = pXAudio2->CreateSourceVoice(&pSourceVoice[sourceplace], &(wfx.Format));
	if (FAILED(hr))
	{
		MessageBox(NULL, "�\�[�X�{�C�X�̍쐬�Ɏ��s���܂����B", NULL, MB_OK);	//���b�Z�[�W�\��
		return;
	}

}

//�t�@�C���������������ȗ���������
string FitFileName(string fName)
{
	string fileFrontName = ("asset/sound/");

	const string  soundName = fileFrontName += fName;

	return soundName;
}

//�}���`�X���b�h�p�A�����Ńt�F�[�h�A�E�g���Ă���
void Audio::FadeAudio(float time)
{
	XAUDIO2_VOICE_STATE xa2state;

	//�ǂ��̋Ȃ���������T��
	int i;
	for (i = 0; i < pSourceVoice.size() - 1; i++)
	{
		pSourceVoice[i]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{
			//BGM����ɒu���Ă������ƂŐ�ɔ������~�߂Ă��炤�\��(�蔲��)
			break;
		}
	}

	//�Đ����̃\�[�X�{�C�X���������Ȃ������ꍇ�͏I��
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

		//�������S�ɏ����Ă��邱�Ƃ��m�F�����甲����
		if(fileVol[i] < 0.0f)
		{
			pSourceVoice[i]->Stop();
			// �I�[�f�B�I�o�b�t�@�̍폜
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
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
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
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}
