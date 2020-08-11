//=============================================================================
//!
//!	@file	MouseInput.cpp
//!	@brief	�}�E�X�f�[�^	������
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

#if 0

//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
MouseInputManager::MouseInputManager()
: _buttons (0)
, _triggers(0)
, _releases(0)
, _mousePos(0,0)
, _offsetActive(false)
{
}

//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
MouseInputManager::~MouseInputManager()
{
}

//-----------------------------------------------------------------------------
//!	�X�V
//-----------------------------------------------------------------------------
void MouseInputManager::Update()
{
	//	�{�^������
	u8 buttons = 0;
	u8 oldButtons = _buttons;
	if(GetKeyState(VK_LBUTTON)&0x80) buttons |= MOUSE_L;
	if(GetKeyState(VK_MBUTTON)&0x80) buttons |= MOUSE_M;
	if(GetKeyState(VK_RBUTTON)&0x80) buttons |= MOUSE_R;
	_buttons = buttons;
	_triggers = (buttons^oldButtons)&buttons;
	_releases = (buttons^oldButtons)&oldButtons;


	//	���W����
	//	�n���h���擾
	HWND hWindow = GetActiveWindow();
	POINT pt;
	//�}�E�X�̌��݂̍��W���擾����
	GetCursorPos(&pt);
	//RECT windowPos;
	//GetWindowRect(hWindow,&windowPos);
	//	�N���C�A���g�̈�ł̍��W�ɕϊ�
	ScreenToClient(hWindow,&pt);
	//	�����o�ɓn��
	_mousePos.Set(pt.x,pt.y);

	Vector2i offset(0,0);
	//	����
	if(_mousePos._x < 0)
	{
		offset._x = -_mousePos._x;
		_mousePos._x = 0;
	}
	if(_mousePos._y < 0)
	{
		offset._y = -_mousePos._y;
		_mousePos._y = 0;
	}

	if(_mousePos._x >= SCRN_WIDTH)
	{
		offset._x = SCRN_WIDTH - _mousePos._x;
		_mousePos._x = SCRN_WIDTH-1;
	}
	if(_mousePos._y >= SCRN_HEIGHT)
	{
		offset._y = SCRN_HEIGHT - _mousePos._y;
		_mousePos._y = SCRN_HEIGHT-1;
	}

//	std::cout<<_mousePos._x<<":"<<_mousePos._y<<std::endl;
	if(_offsetActive == true)
	{
		SetCursorPos(pt.x+offset._x,pt.y+offset._y);
	}
}

static MouseInputManager gMouseInput;


#endif
