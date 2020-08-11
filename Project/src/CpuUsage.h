//=============================================================================
//!
//!	@file	CpuUsage.cpp
//!	@brief	CPU�g�p���v���N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once
class CpuUsage
{
public:
	//!	�R���X�g���N�^
	CpuUsage();
	//!	�f�X�g���N�^
	virtual ~CpuUsage();

	//!	�g�p���擾�֐�
	//!	@return	�g�p��
	//!	@retval	-1�͎擾���s
	//!	��x�l������ɕԂ���Ă����玸�s�����Ƃ��͑O��̒l���Ԃ��Ă���
	s16  GetUsage(){ return _usage; }

	//!	�X�V
	void Update();
	//!	�`��
	void Render();

private:
	//!	���Ԃ̍������擾
	u64 SubtractTimes(const FILETIME& ftA, const FILETIME& ftB);

	//!	���Ԃ��o�߂�����
	bool EnoughTimePassed();
	//!	�����N��
	bool IsFirstRun() const { return (_lastRunTime == 0); }

private:
	//system total times
	FILETIME _prevSysKernel;
	FILETIME _prevSysUser;

	//process times
	FILETIME _prevProcKernel;
	FILETIME _prevProcUser;

	s16 _usage;			//!< �g�p��
	u64 _lastRunTime;	//!< �O��擾�����Ƃ��̎���

	volatile s32L _lRunCount;
};

