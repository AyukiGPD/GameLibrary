//=============================================================================
//!
//!	@file	Timer.h
//!	@brief	�^�C�}�[
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//-------------------------------------------------------------------
//!	�^�C�}�[�I�u�W�F�N�g
//-------------------------------------------------------------------
class Timer
{
public:
	Timer()
	:_time(0.0)
	,_active(false)
	{
		memset(&_freq  , 0x00, sizeof _freq);
		memset(&_before, 0x00, sizeof _before);
		memset(&_after , 0x00, sizeof _after);
	}
	virtual ~Timer(){}

	//!	���Z�b�g
	void Reset();
	//!	����J�n
	//!	@retval	true	���萬��
	//!	@retval	false	���莸�s
	bool Start();
	//!	����I��
	//!	@return	���茋��msec
	f64 Stop();

	//!	�v������
	f64 GetTime() const
	{
		return _time;
	}
private:
	LARGE_INTEGER _freq;
	LARGE_INTEGER _before;
	LARGE_INTEGER _after;
	f64 _time;
	bool _active;

};
