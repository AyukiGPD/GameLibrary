//=============================================================================
//!
//!	@file	CpuUsage.cpp
//!	@brief	CPU使用率計測クラス
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once
class CpuUsage
{
public:
	//!	コンストラクタ
	CpuUsage();
	//!	デストラクタ
	virtual ~CpuUsage();

	//!	使用率取得関数
	//!	@return	使用率
	//!	@retval	-1は取得失敗
	//!	一度値が正常に返されていたら失敗したときは前回の値が返ってくる
	s16  GetUsage(){ return _usage; }

	//!	更新
	void Update();
	//!	描画
	void Render();

private:
	//!	時間の差分を取得
	u64 SubtractTimes(const FILETIME& ftA, const FILETIME& ftB);

	//!	時間が経過したか
	bool EnoughTimePassed();
	//!	初期起動
	bool IsFirstRun() const { return (_lastRunTime == 0); }

private:
	//system total times
	FILETIME _prevSysKernel;
	FILETIME _prevSysUser;

	//process times
	FILETIME _prevProcKernel;
	FILETIME _prevProcUser;

	s16 _usage;			//!< 使用率
	u64 _lastRunTime;	//!< 前回取得したときの時間

	volatile s32L _lRunCount;
};

