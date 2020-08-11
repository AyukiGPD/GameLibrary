//=============================================================================
//!
//!	@file	FrameRate.h
//!	@brief	フレーム計測クラス
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//	フレーム制御未実装

class FrameRate
{
public:
	//!	コンストラクタ
	FrameRate();
	//!	デストラクタ
	virtual ~FrameRate();

	//!	更新
	void Update();

	//!	描画
	void Render();

	//!	デルタタイムの取得
	//!	@return	デルタタイム
	f32 GetDeltaTime()const
	{
		return _deltaTime;
	}

	//!	フレームレートの取得
	//!	@return	フレームレート
	f32 GetFrameRate()const
	{
		return _frameRate;
	}


private:	

	s32  _oldTime;		//!< 過去の時間
	s32  _nowTime;		//!< 現在の時間
	s32  _count;		//!< カウント
	f32	 _frameRate;	//!< フレームレート
	s32* _frame;		//!< フレーム単位の時間
	f32 _deltaTime;		//!< 1フレームにかかった時間
};


//!	デルタタイムの取得
//!	@return	デルタタイム
f32 GetDeltaTime();

//!	フレームレートの取得
//!	@return	フレームレート
f32 GetFrameRate();

//!	デルタタイムを掛けた値の作成
//!	@return	デルタタイムを掛けた値
f32 GetDeltaValue(f32 val);

//!	デルタタイムを掛けた値の作成
//!	@return	デルタタイムを掛けた値
Vector2 GetDeltaValue(const Vector2& val);

//!	デルタタイムを掛けた値の作成
//!	@return	デルタタイムを掛けた値
Vector3 GetDeltaValue(const Vector3& val);

//!	デルタタイムを掛けた値の作成
//!	@return	デルタタイムを掛けた値
Vector4 GetDeltaValue(const Vector4& val);

