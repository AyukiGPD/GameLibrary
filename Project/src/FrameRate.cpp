//=============================================================================
//!
//!	@file	FrameRate.cpp
//!	@brief	フレーム計測クラス
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

const s32 FRAME_COUNT = 30;

//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
FrameRate::FrameRate()
: _nowTime(timeGetTime())	//	時間の取得
, _count(0)
, _frameRate(0.0f)
, _frame(nullptr)
, _deltaTime(0)
{
	_frame = new s32[FRAME_COUNT];	//	フレーム分の配列の確保
	assert(_frame != nullptr);

	for(s32 i=0; i<FRAME_COUNT; ++i)
	{
		_frame[i] = 0;
	}
	_oldTime = _nowTime;

}
//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
FrameRate::~FrameRate()
{
	SafeDeleteArray(_frame);
}

//-----------------------------------------------------------------------------
//!	更新
//-----------------------------------------------------------------------------
void FrameRate::Update()
{

	s32 nowCount = _count % FRAME_COUNT;

	_nowTime         = timeGetTime();		//	時間取得

	s32 offset		 = _nowTime - _oldTime;	//	差分を格納
	_frame[nowCount] = offset;				//	差分を保存していく

	_deltaTime		 = (f32)offset * 0.001f;//	秒単位に変換
	_oldTime         = _nowTime;			//	前の時間を保存

	f32 average = 0;
	//	フレームが一周したら
	if( nowCount == (FRAME_COUNT-1) )
	{
		//	１フレームにかかった時間をすべて足す
		for(s32 i=0; i<FRAME_COUNT; ++i) 
		{
			average += _frame[i];
		}

		if( average != 0 ) 
		{
			//	平均を出す
			average /= FRAME_COUNT;
		}
	}

	_count = (_count + 1) % FRAME_COUNT;	//	カウント



	//u32 time = (1000.0f / (f32)FRAME_COUNT);
	//_sleepTime = time - (_deltaTime * 1000.0f);
	//Sleep((_sleepTime <= 0 ? 0 : _sleepTime));

	if( average == 0 ) return;

	_frameRate = (1000.0f/average);		//	フレームレート
	

}

//-----------------------------------------------------------------------------
//!	描画
//-----------------------------------------------------------------------------
void FrameRate::Render()
{
	SetColor(255);

	f32 total = 0;
	for(s32 i=0; i<FRAME_COUNT; ++i)
	{
		total += (f32)_frame[i];
	}
	SetColor(255);

	const f32 div = 1000.0f;
	const f32 percent = (total / div) * 0.5f;

	Render2D::Rect(0, (f32)SCRN_HEIGHT, SCRN_WIDTH, SCRN_HEIGHT - 2, Color(0));

	f32 frame = ((total / FRAME_COUNT) / div ) * div;	//	１フレームの平均時間
	
	std::wstringstream wsstr;
	wsstr	<< "1Frame:" << (s32)frame 
			<< " FPS:" << std::fixed << _frameRate 
			<< " DeltaTime:" << std::fixed << _deltaTime;
//			<< " SleepTime" << _sleepTime;
	if (percent > 0.55)
	{
		// 赤
		SetColor(255,0,0,255);


		Render2D::Rect(0, (f32)SCRN_HEIGHT - 4, SCRN_WIDTH * percent, 4, Color(255, 0, 0));
		Manager::Debug()->RenderDebugString(wsstr.str(), Vector2(0, SCRN_HEIGHT - 20));
	}
	else
	{
		// 緑
		SetColor(0,255,0,255);
		Render2D::Rect(0, (f32)SCRN_HEIGHT - 4, SCRN_WIDTH * percent, 4, Color(0, 255, 0));
		Manager::Debug()->RenderDebugString(wsstr.str(), Vector2(0, SCRN_HEIGHT - 20));
	}


}


//-----------------------------------------------------------------------------
//!	デルタタイムの取得
//!	@return	デルタタイム
//-----------------------------------------------------------------------------
f32 GetDeltaTime()
{
	FrameRate* p = Singleton<FrameRate>::GetInstance();
	if (p == nullptr)
	{
		//	実体がないから取得できない
		ERROR_LOG_W(L"FrameRateの実体がない");
		return 0.f;
	}

	return p->GetDeltaTime();
}


f32 GetFrameRate()
{
	FrameRate* p = Singleton<FrameRate>::GetInstance();
	if (p == nullptr)
	{
		//	実体がないから取得できない
		ERROR_LOG_W(L"FrameRateの実体がない");
		return 0.f;
	}

	return p->GetFrameRate();
}



//!	デルタタイムを掛けた値の作成
//!	@return	デルタタイムを掛けた値
f32 GetDeltaValue(f32 val)
{
	return val * GetDeltaTime();
}

//!	デルタタイムを掛けた値の作成
//!	@return	デルタタイムを掛けた値
Vector2 GetDeltaValue(const Vector2& val)
{
	return val * GetDeltaTime();
}

//!	デルタタイムを掛けた値の作成
//!	@return	デルタタイムを掛けた値
Vector3 GetDeltaValue(const Vector3& val)
{
	return val * GetDeltaTime();
}

//!	デルタタイムを掛けた値の作成
//!	@return	デルタタイムを掛けた値
Vector4 GetDeltaValue(const Vector4& val)
{
	return val * GetDeltaTime();
}




