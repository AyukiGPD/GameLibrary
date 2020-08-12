//=============================================================================
//!
//!	@file	KeyInput.cpp
//!	@brief	�L�[�f�[�^
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// @brief  �R���X�g���N�^
//-----------------------------------------------------------------------------
KeyInputManager::KeyInputManager()
{
	for(u32 i = 0; i < 8; ++i)
	{
		_buttons[i] = 0;
		_triggers[i] = 0;
		_releases[i] = 0;
	}
}

//-----------------------------------------------------------------------------
// @brief  �f�X�g���N�^
//-----------------------------------------------------------------------------
KeyInputManager::~KeyInputManager()
{
}

//-----------------------------------------------------------------------------
// @brief  �X�V
//-----------------------------------------------------------------------------
void KeyInputManager::Update()
{
	//	�L�[�f�[�^�擾
	u8 key[256];
	if(GetKeyboardState(key) == FALSE)
	{
		DEBUG_LOG("�L�[�f�[�^�擾���s");
	}

	for(u32 i = 0; i < 8; ++i)
	{
		u32 buttons = 0;
		u32 oldButtons = _buttons[i];
		u32 start = i * 32;
		for(u32 j = 0; j < 32; ++j)
		{
			if(key[j + start] & 0x80)
			{
				buttons |= (1 << j);
			}
		}
		_buttons[i] = buttons;
		_triggers[i] = (buttons ^ oldButtons) & buttons;
		_releases[i] = (buttons ^ oldButtons) & oldButtons;

	}
}

//-----------------------------------------------------------------------------
//  @brief  �L�[�擾
//  @param  [in]    pad     �L�[�R�[�h
//  @param  [in]    mode    �擾���[�h
//-----------------------------------------------------------------------------
bool KeyInputManager::GetButtons(KEYCODE pad, KEY_MODE mode) const
{
	switch (mode)
	{
	case KEY_MODE::BUTTON:
	return GetButtons(pad);
	break;
	case KEY_MODE::TRIGGER:
	return GetTriggers(pad);
	break;
	case KEY_MODE::RELEASE:
	return GetReleases(pad);
	break;
	}
}

//-----------------------------------------------------------------------------
//  @brief  ���L�[�擾
//  @param  [in]    pad �L�[�R�[�h
//-----------------------------------------------------------------------------
bool KeyInputManager::GetButtons(KEYCODE pad) const
{
	u32 padVal = (u32)pad;
	u32 index = padVal == 0 ? 0 : (padVal / 32);
	u32 shift = padVal - (index * 32);
	return (_buttons[index] & (1 << shift)) != 0;
}

//-----------------------------------------------------------------------------
//  @brief  �g���K�[
//  @param  [in]    pad �L�[�R�[�h
//-----------------------------------------------------------------------------
bool KeyInputManager::GetTriggers(KEYCODE pad) const
{
	u32 padVal = (u32)pad;
	u32 index = padVal == 0 ? 0 : (padVal / 32);
	u32 shift = padVal - (index * 32);
	return (_triggers[index] & (1 << shift)) != 0;
}

//-----------------------------------------------------------------------------
//  @brief  �����[�X
//  @param  [in]    pad �L�[�R�[�h
//-----------------------------------------------------------------------------
bool KeyInputManager::GetReleases(KEYCODE pad) const
{
	u32 padVal = (u32)pad;
	u32 index = padVal == 0 ? 0 : (padVal / 32);
	u32 shift = padVal - (index * 32);
	return (_releases[index] & (1 << shift)) != 0;
}

