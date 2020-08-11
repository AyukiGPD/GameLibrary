//=============================================================================
//!
//!	@file	MouseInput.h
//!	@brief	マウスデータ	未実装
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//	未完成
#if 0
class MouseInputManager
{
public:
	enum MOUSE
	{
		MOUSE_L = (1<<0),
		MOUSE_M = (1<<1),
		MOUSE_R = (1<<2),
	};

	//!	コンストラクタ
	MouseInputManager();
	//!	デストラクタ
	virtual ~MouseInputManager();

	//!	更新
	void	Update();

	u8			GetButtons() const{return _buttons;}
	u8			GetTriggers() const{return _triggers;}
	u8			GetReleases() const{return _releases;}
	Vector2i	GetMousePos() const{return _mousePos;}

	void SetMouseOffset(bool offsetActive){_offsetActive = offsetActive;}

private:
	u8			_buttons;
	u8			_triggers;
	u8			_releases;

	Vector2i	_mousePos;
	bool		_offsetActive;

};

MY_INTERFACE(MouseInputManager, MouseInput);


#endif
