//=============================================================================
//!
//!	@file	Timer.cpp
//!	@brief	タイマー
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
//!	リセット
//-----------------------------------------------------------------------------
void Timer::Reset()
{
	//変数の初期化
	memset(&_freq, 0x00, sizeof _freq);
	memset(&_before, 0x00, sizeof _before);
	memset(&_after, 0x00, sizeof _after);
	_time = 0;
	_active = false;
}
//-----------------------------------------------------------------------------
//!	測定開始
//!	@retval	true	測定成功
//!	@retval	false	測定失敗
//-----------------------------------------------------------------------------
bool Timer::Start()
{
	if(_active)
	{
		return false;
	}

	Reset();
	QueryPerformanceFrequency(&_freq);
	QueryPerformanceCounter(&_before);

	_active = true;

	return true;
}
//-----------------------------------------------------------------------------
//!	測定終了
//!	@return	測定結果msec
//-----------------------------------------------------------------------------
f64 Timer::Stop()
{
	if(!_active)
	{
		return 0.0;
	}
	QueryPerformanceCounter(&_after);
	_time = (f64)((_after.QuadPart - _before.QuadPart) * 1000 / (f64)_freq.QuadPart);
	_active = false;
	return _time;
}


