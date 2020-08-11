//=============================================================================
//!
//!	@file	CpuUsage.cpp
//!	@brief	CPU�g�p���v���N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"


//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
CpuUsage::CpuUsage()
: _usage(-1)
, _lastRunTime(0)
, _lRunCount(0)
{
	ZeroMemory(&_prevSysKernel, sizeof(FILETIME));
	ZeroMemory(&_prevSysUser, sizeof(FILETIME));

	ZeroMemory(&_prevProcKernel, sizeof(FILETIME));
	ZeroMemory(&_prevProcUser, sizeof(FILETIME));

}

//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
CpuUsage::~CpuUsage()
{
}

u64 CpuUsage::SubtractTimes(const FILETIME& ftA, const FILETIME& ftB)
{
	LARGE_INTEGER a, b;
	a.LowPart = ftA.dwLowDateTime;
	a.HighPart = ftA.dwHighDateTime;

	b.LowPart = ftB.dwLowDateTime;
	b.HighPart = ftB.dwHighDateTime;

	return a.QuadPart - b.QuadPart;
}

//-----------------------------------------------------------------------------
//!	���Ԃ��o�߂�����
//-----------------------------------------------------------------------------
bool CpuUsage::EnoughTimePassed()
{
	const s32 minElapsedMS = 250;//milliseconds

	u64 currentTickCount = GetTickCount64();
	return (currentTickCount - _lastRunTime) > minElapsedMS;
}


//-----------------------------------------------------------------------------
//!	�X�V
//-----------------------------------------------------------------------------
void CpuUsage::Update()
{
	//	�O��̎g�p����ێ����邽�߂ɃR�s�[
	if (::InterlockedIncrement(&_lRunCount) == 1)
	{
		//	�Ă΂��Ԋu���Z������ƑO�̏�ԂŕԂ�
		if (!EnoughTimePassed())
		{
			::InterlockedDecrement(&_lRunCount);
			return;
		}

		FILETIME sysIdle, sysKernel, sysUser;		//	�V�X�e��
		FILETIME procCreation, procExit, procKernel, procUser;	//	�v���Z�X
		//	�V�X�e���A�v���Z�X�̎��Ԏ擾
		if (!GetSystemTimes(&sysIdle, &sysKernel, &sysUser) ||
			!GetProcessTimes(GetCurrentProcess(), &procCreation, &procExit, &procKernel, &procUser))
		{
			::InterlockedDecrement(&_lRunCount);
			return;
		}

		//	��x�ł��Ă΂�Ă��邩
		if (!IsFirstRun())
		{
			u64 sysKernelDiff = SubtractTimes(sysKernel, _prevSysKernel);
			u64 sysUserDiff = SubtractTimes(sysUser, _prevSysUser);

			u64 procKernelDiff = SubtractTimes(procKernel, _prevProcKernel);
			u64 procUserDiff = SubtractTimes(procUser, _prevProcUser);

			u64 totalSys = sysKernelDiff + sysUserDiff;
			u64 totalProc = procKernelDiff + procUserDiff;

			if (totalSys > 0)
			{
				_usage = (s16)((100.0f * totalProc) / totalSys);
			}
		}

		_prevSysKernel = sysKernel;
		_prevSysUser = sysUser;
		_prevProcKernel = procKernel;
		_prevProcUser = procUser;

		//	�l���擾�������Ԃ�ۑ����Ă���
		_lastRunTime = GetTickCount64();
	}

	::InterlockedDecrement(&_lRunCount);
}
//-----------------------------------------------------------------------------
//!	�`��
//-----------------------------------------------------------------------------
void CpuUsage::Render()
{
	SetColor(255);

	
	std::wstringstream wsstr;

	wsstr << L"CPU:" << _usage << L"%";

	f32 percent = (f32)_usage*0.01f;
	Render2D::Rect(0, (f32)SCRN_HEIGHT - 12, SCRN_WIDTH * percent, 4, Color(255, 255, 0));
	Manager::Debug()->RenderDebugString(wsstr.str(), Vector2(0, SCRN_HEIGHT - 60));
}
