//=============================================================================
//!
//!	@file	DebugRender.cpp
//!	@brief	デバッグ描画
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
DebugManager::DebugManager()
: _pFrameRate(nullptr)
, _pGpuUsage(nullptr)
, _pCpuUsage(nullptr)
, _renderActive(false)
, _updateActive(true)
, _nextFrame(false)
{
	Init();
}

//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
DebugManager::~DebugManager()
{
}

//-----------------------------------------------------------------------------
//!	初期化
//-----------------------------------------------------------------------------
void DebugManager::Init()
{
	_pFrameRate = Singleton<FrameRate>::GetInstance();
	_pGpuUsage  = Singleton<GpuUsage>::GetInstance();
	_pCpuUsage  = Singleton<CpuUsage>::GetInstance();
	for (auto& wstring : _debugWString)
	{
		wstring = L"";
	}
	for (auto& color : _debugStringColor)
	{
		color = 255;
	}
}
//-----------------------------------------------------------------------------
//!	更新
//-----------------------------------------------------------------------------
void DebugManager::Update()
{
	//	描画フラグ
	//	描画しなくても更新はする
#ifndef RELEASE_FINAL
	if(Manager::KeyInput()->GetTriggers(KEYCODE::PAD_F1))
	{
		_renderActive = !_renderActive;
	}
	if(Manager::KeyInput()->GetTriggers(KEYCODE::PAD_F2))
	{
		_updateActive = !_updateActive;
	}
	if (!_updateActive)
	{
		_nextFrame = false;
		if(Manager::KeyInput()->GetTriggers(KEYCODE::PAD_F3))
		{
			_nextFrame = true;
		}
	}
#endif
	_pFrameRate->Update();
	_pGpuUsage->Update();
	_pCpuUsage->Update();
}
//-----------------------------------------------------------------------------
//!	描画
//-----------------------------------------------------------------------------
void DebugManager::Render()
{
	if (_renderActive == false){ return; }
	RenderManager* pRenderer = Manager::Renderer();
	pRenderer->DisableDepthTest();
	_pFrameRate->Render();
	_pGpuUsage->Render();
	_pCpuUsage->Render();

#ifndef RELEASE_FINAL
	for (u32 i = 0; i < _debugWString.size(); ++i)
	{
		auto& string = _debugWString[i];
		if (string.empty()){ continue; }
		SetColor(_debugStringColor[i]);
		RenderDebugString(string, Vector2(0, f32(16 * i + 16)));
	}
	if (!_updateActive)
	{
		SetColor(255, 0, 0);
		std::wstring wstr;
		CREATE_WSTRING(L"更新停止中", wstr);
		RenderDebugString(wstr, Vector2(0, SCRN_HEIGHT - 80));
	}
#endif
	pRenderer->EnableDepthTest();
}

//-----------------------------------------------------------------------------
//!	デバッグ描画
//!	@param	[in]	index	番号
//!	@param	[in]	str		文字列
//!	@param	[in]	color	色
//-----------------------------------------------------------------------------
void DebugManager::SetDebugString(u32 index, const std::wstring& str, const Color& color)
{
	_debugWString	 [index] = str;
	_debugStringColor[index] = color;
}

//-----------------------------------------------------------------------------
//!	描画文字
//!	@param	[in]	hundler	文字データ
//-----------------------------------------------------------------------------
void DebugManager::SetDebugFontHundler(const FontHundler& hundler)
{
	_debugFont = hundler;
}

//-----------------------------------------------------------------------------
//!	ビットマップ文字描画
//!	@param	[in]	str	文字列
//!	@param	[in]	x	X座標
//!	@param	[in]	y	Y座標
//-----------------------------------------------------------------------------
void DebugManager::RenderDebugString(const std::wstring& str, f32 x, f32 y)
{
	_debugFont.Render(str, x, y);
}
//-----------------------------------------------------------------------------
//!	ビットマップ文字描画
//!	@param	[in]	str	文字列
//!	@param	[in]	pos	座標
//-----------------------------------------------------------------------------
void DebugManager::RenderDebugString(const std::wstring& str, const Vector2& pos)
{
	RenderDebugString(str.c_str(), pos.x, pos.y);
}



