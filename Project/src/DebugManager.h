//=============================================================================
//!
//!	@file	DebugRender.h
//!	@brief	デバッグ描画
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

class DebugManager : public ManagerBase
{
public:
	//!	コンストラクタ
	//!	@param	[in]	render	描画フラグ
	DebugManager();
	//!	デストラクタ
	virtual ~DebugManager();

	//!	初期化
	void Init();
	//!	更新
	void Update();
	//!	描画
	void Render();

	//!	フレームレート
	FrameRate* GetFrameRateObj(){ return _pFrameRate; }
	//!	GPU使用率
	GpuUsage*  GetGpuUsageObj()	{ return _pGpuUsage; }
	//!	CPU使用率
	CpuUsage*  GetCpuUsageObj()	{ return _pCpuUsage; }

	//!	デバッグ描画
	//!	@param	[in]	index	番号
	//!	@param	[in]	str		文字列
	//!	@param	[in]	color	色
	void SetDebugString(u32 index, const std::wstring& str,const Color& Color = Color::WHITE);

	//!	デバッグ停止
	bool IsUpdateActive()const
	{
		return _updateActive || _nextFrame;
	}

	//!	デバッグ描画フラグ設定
	//!	@param	[in]	active	フラグ
	void SetDebugRender(bool active)
	{
		_renderActive = active;
	}

	//!	描画文字
	//!	@param	[in]	hundler	文字データ
	void SetDebugFontHundler(const FontHundler& hundler);

	//!	デバッグ文字描画
	//!	@param	[in]	str	文字列
	//!	@param	[in]	x	X座標
	//!	@param	[in]	y	Y座標
	void RenderDebugString(const std::wstring& str, f32 x, f32 y);
	//!	デバッグ文字描画
	//!	@param	[in]	str	文字列
	//!	@param	[in]	pos	座標
	void RenderDebugString(const std::wstring& str, const Vector2& pos);

private:
	FrameRate* _pFrameRate;	//!< フレームレート計測クラス
	GpuUsage* _pGpuUsage;	//!< GPU使用率計測クラス
	CpuUsage* _pCpuUsage;	//!< CPU使用率計測クラス
	bool _renderActive;		//!< 表示フラグ
	bool _updateActive;		//!< 表示フラグ
	bool _nextFrame;		//!< 表示フラグ
	std::array<std::wstring, 55> _debugWString;
	std::array<Color, 55> _debugStringColor;

	FontHundler _debugFont;	//!< デバッグ文字データ

};

MANAGER_INTERFACE(DebugManager, Debug);

