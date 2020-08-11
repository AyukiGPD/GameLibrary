//=============================================================================
//!
//!	@file	SystemMain.h
//!	@brief	メインシステム
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//-------------------------------------------------------------------
//!	メイン
//-------------------------------------------------------------------
class SystemMain : public SingletonInherit<SystemMain>
{
public:
	//!	コンストラクタ
	SystemMain();
	//!	デストラクタ
	virtual ~SystemMain();

	//! メインループ
	//! @param  [in]    titleName     タイトル名
	//! @param  [in]    className     クラス名
	//! @param  [in]    screenW       画面の幅
	//! @param  [in]    screenH       画面の高さ
	//! @param  [in]    isWindowed    true：ウィンドウ, false：フルスクリーン
	//! @return 
	s32					MainLoop(
		LPCWSTR titleName,
		LPCWSTR className,
		bool    isWindowed = true
		);


	//! ウィンドウハンドルを取得
	//! @return ウィンドウハンドル
	MY_FORCEINLINE
	HWND				GetHwnd() const
	{
		return _hwnd;
	}

private:

	//! 初期化
	//! @retval true    初期化成功
	//! @retval false   初期化失敗
	bool				Init();

	//! 1フレーム更新
	//! @retval なし
	void				UpdateFrame();

	//! 解放
	//! @retval なし
	void				Clear();

private:
	HWND				_hwnd;					//!< ウィンドウハンドル
	bool				_isExit;				//!< 終了フラグ
	bool				_isRunning;

	SystemWork* _pWork;								//!< メインクラス

	//! ウィンドウプロシージャ
	static LRESULT CALLBACK	WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
};

//!	ウィンドウのアクティブ状態の取得
//!	@retval	true アクティブ : false 非アクティブ
bool IsWindowActive();

