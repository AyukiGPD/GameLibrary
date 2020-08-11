//=============================================================================
//!
//!	@file	DebugRender.cpp
//!	@brief	�f�o�b�O�`��
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
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
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
DebugManager::~DebugManager()
{
}

//-----------------------------------------------------------------------------
//!	������
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
//!	�X�V
//-----------------------------------------------------------------------------
void DebugManager::Update()
{
	//	�`��t���O
	//	�`�悵�Ȃ��Ă��X�V�͂���
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
//!	�`��
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
		CREATE_WSTRING(L"�X�V��~��", wstr);
		RenderDebugString(wstr, Vector2(0, SCRN_HEIGHT - 80));
	}
#endif
	pRenderer->EnableDepthTest();
}

//-----------------------------------------------------------------------------
//!	�f�o�b�O�`��
//!	@param	[in]	index	�ԍ�
//!	@param	[in]	str		������
//!	@param	[in]	color	�F
//-----------------------------------------------------------------------------
void DebugManager::SetDebugString(u32 index, const std::wstring& str, const Color& color)
{
	_debugWString	 [index] = str;
	_debugStringColor[index] = color;
}

//-----------------------------------------------------------------------------
//!	�`�敶��
//!	@param	[in]	hundler	�����f�[�^
//-----------------------------------------------------------------------------
void DebugManager::SetDebugFontHundler(const FontHundler& hundler)
{
	_debugFont = hundler;
}

//-----------------------------------------------------------------------------
//!	�r�b�g�}�b�v�����`��
//!	@param	[in]	str	������
//!	@param	[in]	x	X���W
//!	@param	[in]	y	Y���W
//-----------------------------------------------------------------------------
void DebugManager::RenderDebugString(const std::wstring& str, f32 x, f32 y)
{
	_debugFont.Render(str, x, y);
}
//-----------------------------------------------------------------------------
//!	�r�b�g�}�b�v�����`��
//!	@param	[in]	str	������
//!	@param	[in]	pos	���W
//-----------------------------------------------------------------------------
void DebugManager::RenderDebugString(const std::wstring& str, const Vector2& pos)
{
	RenderDebugString(str.c_str(), pos.x, pos.y);
}



