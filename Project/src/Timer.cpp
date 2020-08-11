//=============================================================================
//!
//!	@file	Timer.cpp
//!	@brief	�^�C�}�[
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
//!	���Z�b�g
//-----------------------------------------------------------------------------
void Timer::Reset()
{
	//�ϐ��̏�����
	memset(&_freq, 0x00, sizeof _freq);
	memset(&_before, 0x00, sizeof _before);
	memset(&_after, 0x00, sizeof _after);
	_time = 0;
	_active = false;
}
//-----------------------------------------------------------------------------
//!	����J�n
//!	@retval	true	���萬��
//!	@retval	false	���莸�s
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
//!	����I��
//!	@return	���茋��msec
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


