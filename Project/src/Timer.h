//=============================================================================
//!
//!	@file	Timer.h
//!	@brief	タイマー
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//-------------------------------------------------------------------
//!	タイマーオブジェクト
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

	//!	リセット
	void Reset();
	//!	測定開始
	//!	@retval	true	測定成功
	//!	@retval	false	測定失敗
	bool Start();
	//!	測定終了
	//!	@return	測定結果msec
	f64 Stop();

	//!	計測時間
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
