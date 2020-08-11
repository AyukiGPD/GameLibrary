//=============================================================================
//!
//!	@file	CpuUsage.cpp
//!	@brief	CPU使用率計測クラス
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"


//-----------------------------------------------------------------------------
//!	コンストラクタ
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
//!	デストラクタ
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
//!	時間が経過したか
//-----------------------------------------------------------------------------
bool CpuUsage::EnoughTimePassed()
{
	const s32 minElapsedMS = 250;//milliseconds

	u64 currentTickCount = GetTickCount64();
	return (currentTickCount - _lastRunTime) > minElapsedMS;
}


//-----------------------------------------------------------------------------
//!	更新
//-----------------------------------------------------------------------------
void CpuUsage::Update()
{
	//	前回の使用率を保持するためにコピー
	if (::InterlockedIncrement(&_lRunCount) == 1)
	{
		//	呼ばれる間隔が短すぎると前の状態で返す
		if (!EnoughTimePassed())
		{
			::InterlockedDecrement(&_lRunCount);
			return;
		}

		FILETIME sysIdle, sysKernel, sysUser;		//	システム
		FILETIME procCreation, procExit, procKernel, procUser;	//	プロセス
		//	システム、プロセスの時間取得
		if (!GetSystemTimes(&sysIdle, &sysKernel, &sysUser) ||
			!GetProcessTimes(GetCurrentProcess(), &procCreation, &procExit, &procKernel, &procUser))
		{
			::InterlockedDecrement(&_lRunCount);
			return;
		}

		//	一度でも呼ばれているか
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

		//	値を取得した時間を保存しておく
		_lastRunTime = GetTickCount64();
	}

	::InterlockedDecrement(&_lRunCount);
}
//-----------------------------------------------------------------------------
//!	描画
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
