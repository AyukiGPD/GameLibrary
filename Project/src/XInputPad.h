//=============================================================================
//!
//!	@file	XInputPad.h
//!	@brief	コントローラクラス
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

#include <XInput.h>
#pragma comment(lib,"XInput9_1_0.lib")

enum class XINPUTPAD : u64
{
	//デジタル方向キー
	DPAD_LEFT		=	( static_cast<u64>(1) <<   0 ),		//!< 左
	DPAD_RIGHT		=	( static_cast<u64>(1) <<   1 ),		//!< 右
	DPAD_UP			=	( static_cast<u64>(1) <<   2 ),		//!< 上
	DPAD_DOWN		=	( static_cast<u64>(1) <<   3 ),		//!< 下

	A				=	( static_cast<u64>(1) <<   4 ),		//!< A
	B				=	( static_cast<u64>(1) <<   5 ),		//!< B
	X				=	( static_cast<u64>(1) <<   6 ),		//!< X
	Y				=	( static_cast<u64>(1) <<   7 ),		//!< Y
	START			=	( static_cast<u64>(1) <<   8 ),		//!< START
	BACK			=	( static_cast<u64>(1) <<   9 ),		//!< BACK
	LEFT_THUMB		=	( static_cast<u64>(1) <<  10 ),		//!< 左スティックボタン
	RIGHT_THUMB		=	( static_cast<u64>(1) <<  11 ),		//!< 右スティックボタン
	LEFT_SHOULDER	=	( static_cast<u64>(1) <<  12 ),		//!< Lボタン
	RIGHT_SHOULDER	=	( static_cast<u64>(1) <<  13 ),		//!< Rボタン
	LEFT_TRIGGER	=	( static_cast<u64>(1) <<  14 ),		//!< Lトリガー
	RIGHT_TRIGGER	=	( static_cast<u64>(1) <<  15 ),		//!< Rトリガー

	//アナログ方向キー
	//	左スティック
	LSTICK_LEFT		=	( static_cast<u64>(1) <<  16 ),		//!< 左
	LSTICK_RIGHT	=	( static_cast<u64>(1) <<  17 ),		//!< 右
	LSTICK_UP		=	( static_cast<u64>(1) <<  18 ),		//!< 上
	LSTICK_DOWN		=	( static_cast<u64>(1) <<  19 ),		//!< 下
	//	右スティック
	RSTICK_LEFT		=	( static_cast<u64>(1) <<  20 ),		//!< 左
	RSTICK_RIGHT	=	( static_cast<u64>(1) <<  21 ),		//!< 右
	RSTICK_UP		=	( static_cast<u64>(1) <<  22 ),		//!< 上
	RSTICK_DOWN		=	( static_cast<u64>(1) <<  23 ),		//!< 下

};

class XInputPadManager : public ManagerBase
{
public:

public:
	//!	コンストラクタ
	XInputPadManager();
	//!	デストラクタ
	virtual ~XInputPadManager();

	//!	初期化
	//!	@param	[in]	vibration	バイブレーション有効/無効
	//!	@param	[in]	retry		コントローラーが刺さってない場合のポップアップ表示
	//! @retval true    初期化成功
	//! @retval false   初期化失敗
	bool Init(bool vibration = true,bool retry = true);
	//!	更新
	void Update();

	//!	バイブレーションON
	//!	@param	[in]	time	作動時間
	//!	@param	[in]	left	左モーター
	//!	@param	[in]	right	右モーター
	void VibrateOnTime(f32 time,f32 left = 1.f, f32 right = 1.f);
	//!	バイブレーションON
	//!	@param	[in]	left	左モーター
	//!	@param	[in]	right	右モーター
	void VibrateOn(f32 left = 1.f, f32 right = 1.f);
	//!	バイブレーションOFF
	void VibrateOff();


	//!	ステート取得
	//!	@return	コントローラーステート
	XINPUT_STATE GetState()const{ return _state; }

	//!	生キー
	//!	@param	[in]	pad	キーコード
	bool GetButtons(XINPUTPAD pad) const
	{
		return (_buttons & (u32)pad) != 0;
	}
	//!	トリガー
	//!	@param	[in]	pad	キーコード
	bool GetTriggers(XINPUTPAD pad) const
	{
		return (_triggers & (u32)pad) != 0;
	}
	//!	リリース
	//!	@param	[in]	pad	キーコード
	bool GetReleases(XINPUTPAD pad) const
	{
		return (_releases & (u32)pad) != 0;
	}

	//!	キースケール取得
	//!	@param	[in]	pad	キーコード
	//!	@return	キースケール
	f32 GetScale( XINPUTPAD pad ) const;

	//!	左スティック取得
	//!	@param	[in]	minVal	最低サイズ
	//!	@return	スティックの値
	Vector2 GetStickL( f32 minVal = 0.2f ) const;
	//!	右スティック取得
	//!	@param	[in]	minVal	最低サイズ
	//!	@return	スティックの値
	Vector2 GetStickR( f32 minVal = 0.2f ) const;


	//!	コントローラーの有無
	bool IsController() const { return _isController; }
	//!	バイブレーションON/OFF
	bool IsVibrate()const{ return _isVibration; }


private:
	//!	接続状態の取得
	//!	@return	パッド状態
	DWORD UpdateControllerState();

private:

	XINPUT_STATE	_state;			//!< コントローラーステート
	bool			_isController;	//!< コントローラー接続

	u32				_buttons;		//!< 生キーデータ
	u32				_triggers;		//!< トリガーキーデータ
	u32				_releases;		//!< リリースキーデータ

	bool			_isVibration;		//!< バイブレーション

	bool			_vibrateActivate;	//!< バイブレーション有効/無効

	f32				_vibrateTime;
};

MANAGER_INTERFACE(XInputPadManager, XInput);

