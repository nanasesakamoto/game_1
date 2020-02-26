#pragma once

//====================================================
// �֐��̃v���g�^�C�v�錾
//====================================================
void SystemTimer_Initialize(void); // �V�X�e���^�C�}�[�̏�����
void SystemTimer_Reset(void); // �V�X�e���^�C�}�[�̃��Z�b�g
void SystemTimer_Start(void); // �V�X�e���^�C�}�[�̃X�^�[�g
void SystemTimer_Stop(void); // �V�X�e���^�C�}�[�̃X�g�b�v
void SystemTimer_Advance(void); // �V�X�e���^�C�}�[��0.1�b�i�߂�
double SystemTimer_GetTime(void); // �v�����Ԃ̎擾
double SystemTimer_GetAbsoluteTime(void); // ���݂̎��Ԃ��擾
float SystemTimer_GetElapsedTime(void); // �o�ߎ��Ԃ̎擾
bool SystemTimer_IsStoped(void); // �V�X�e���^�C�}�[���~�܂��Ă��邩�H
void LimitThreadAffinityToCurrentProc(void); // ���݂̃X���b�h��1�̃v���Z�b�T�i���݂̃X���b�h�j�ɐ���


//�N���X�����܂��傤
class SystemTimer
{
private:
	LONGLONG TicksPerSec;					// �P�b�Ԃ̌v������
	bool bTimerStopped = true;				// �X�g�b�v�t���O
	LONGLONG StopTime;						// �X�g�b�v��������
	LONGLONG LastElapsedTime;				// �Ō�ɋL�^�����X�V����
	LONGLONG BaseTime;						// ��{����

public:
	SystemTimer();
	void StartTimer();
	double GetTime();
	bool mUse;
};
