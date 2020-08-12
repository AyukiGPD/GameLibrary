//=============================================================================
//!
//!	@file	XInputPad.cpp
//!	@brief	�R���g���[���N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

static const s32 STICK_THRES_HOLD = 65535 / 6; //!< �X�e�B�b�N�������l
static const s32 TRIGGER_THRES_HOLD = 255 / 8; //!< �g���K�[�������l

static const f32 STICK_THRES_HOLD_FLOAT = 1.0f / STICK_THRES_HOLD; //!< �X�e�B�b�N�������l
static const f32 TRIGGER_THRES_HOLD_FLOAT = 1.0f / TRIGGER_THRES_HOLD; //!< �g���K�[�������l

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//-----------------------------------------------------------------------------
XInputPadManager::XInputPadManager()
: _buttons			(0)
, _triggers			(0)
, _releases			(0)
, _isVibration		(false)
, _isController		(false)
, _vibrateActivate	(false)
, _vibrateTime		(0.f)
{
}

//-----------------------------------------------------------------------------
//  @brief  �f�X�g���N�^
//-----------------------------------------------------------------------------
XInputPadManager::~XInputPadManager()
{
	VibrateOff();
}

//-----------------------------------------------------------------------------
//  @brief  ������
//  @param	[in]	vibration	�o�C�u���[�V�����L��/����
//  @param	[in]	retry		�R���g���[���[���h�����ĂȂ��ꍇ�̃|�b�v�A�b�v�\��
//  @retval true    ����������
//  @retval false   ���������s
//-----------------------------------------------------------------------------
bool XInputPadManager::Init(bool vibration, bool retry)
{
	//	Init��ʂ����񐔂𐔂���
	//	���������s�ł�����x�ǂݍ��ޏꍇ
	//	������x���̊֐����ĂԂ���
	//	�񐔐������������Ė����ɌĂׂ��Ԃ�h��
	static u32 initCount = 0;	//	Init��ʂ�����
	++initCount;	//	�J�E���g

	_isVibration = false;
	ZeroMemory(&_state,sizeof(XINPUT_STATE));	//	������
	DWORD dwResult = UpdateControllerState();	//	�ڑ���Ԃ̎擾
	bool result = false;


	if(dwResult == ERROR_SUCCESS)
	{
		result = true;
	}
	else
	{
		//	�Ăяo���񐔂𒴂����ꍇ
		if (initCount >= 5)
		{
			MessageBox(nullptr,
				L"�R���g���[���[���F������܂���ł���",
				L"�G���[",
				MB_OK);
			result = false;	//	�O�̂���false��
		}
		else if(retry == true)
		{
			s32 mb = MessageBox(nullptr,
								L"�R���g���[���[���F������܂���ł���\n���̂܂ܑ����܂����H",
								L"�G���[",
								MB_YESNO);
			if(mb == IDNO)
			{
				result = Init();
			}
		}
	}
	//	�����܂ŗ���΃J�E���g�����������Ă���
	initCount = 0;
	_vibrateActivate = vibration;
	return result;
}

//-----------------------------------------------------------------------------
//  @brief  �X�V
//-----------------------------------------------------------------------------
void XInputPadManager::Update()
{
	UpdateControllerState();
	if (_isController == false)	return;

	//	�e�L�[�̎擾
	u32 buttons    = 0;
	u32 oldButtons = _buttons;

	WORD padButtons = _state.Gamepad.wButtons;

	//	�f�W�^�������L�[
	if( padButtons & XINPUT_GAMEPAD_DPAD_LEFT				)	buttons |= (u64)XINPUTPAD::DPAD_LEFT; 
	if( padButtons & XINPUT_GAMEPAD_DPAD_RIGHT				)	buttons |= (u64)XINPUTPAD::DPAD_RIGHT;
	if( padButtons & XINPUT_GAMEPAD_DPAD_UP					)	buttons |= (u64)XINPUTPAD::DPAD_UP;
	if( padButtons & XINPUT_GAMEPAD_DPAD_DOWN				)	buttons |= (u64)XINPUTPAD::DPAD_DOWN;
	//	�e��{�^��
	if( padButtons & XINPUT_GAMEPAD_A						)	buttons |= (u64)XINPUTPAD::A;
	if( padButtons & XINPUT_GAMEPAD_B						)	buttons |= (u64)XINPUTPAD::B;
	if( padButtons & XINPUT_GAMEPAD_X						)	buttons |= (u64)XINPUTPAD::X;
	if( padButtons & XINPUT_GAMEPAD_Y						)	buttons |= (u64)XINPUTPAD::Y;
	if( padButtons & XINPUT_GAMEPAD_START					)	buttons |= (u64)XINPUTPAD::START;
	if( padButtons & XINPUT_GAMEPAD_BACK					)	buttons |= (u64)XINPUTPAD::BACK;
	if( padButtons & XINPUT_GAMEPAD_LEFT_THUMB				)	buttons |= (u64)XINPUTPAD::LEFT_THUMB;
	if( padButtons & XINPUT_GAMEPAD_RIGHT_THUMB				)	buttons |= (u64)XINPUTPAD::RIGHT_THUMB;
	if( padButtons & XINPUT_GAMEPAD_LEFT_SHOULDER			)	buttons |= (u64)XINPUTPAD::LEFT_SHOULDER;
	if( padButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER			)	buttons |= (u64)XINPUTPAD::RIGHT_SHOULDER;
	if( _state.Gamepad.bLeftTrigger  > TRIGGER_THRES_HOLD	)	buttons |= (u64)XINPUTPAD::LEFT_TRIGGER;
	if( _state.Gamepad.bRightTrigger > TRIGGER_THRES_HOLD	)	buttons |= (u64)XINPUTPAD::RIGHT_TRIGGER;
	//	�A�i���O�����L�[
	//	��
	if( _state.Gamepad.sThumbLX <= -STICK_THRES_HOLD		)	buttons |= (u64)XINPUTPAD::LSTICK_LEFT;
	if( _state.Gamepad.sThumbLX >=  STICK_THRES_HOLD		)	buttons |= (u64)XINPUTPAD::LSTICK_RIGHT;
	if( _state.Gamepad.sThumbLY >=  STICK_THRES_HOLD		)	buttons |= (u64)XINPUTPAD::LSTICK_UP;
	if( _state.Gamepad.sThumbLY <= -STICK_THRES_HOLD		)	buttons |= (u64)XINPUTPAD::LSTICK_DOWN;
	//	�E
	if( _state.Gamepad.sThumbRX <= -STICK_THRES_HOLD		)	buttons |= (u64)XINPUTPAD::RSTICK_LEFT;
	if( _state.Gamepad.sThumbRX >=  STICK_THRES_HOLD		)	buttons |= (u64)XINPUTPAD::RSTICK_RIGHT;
	if( _state.Gamepad.sThumbRY >=  STICK_THRES_HOLD		)	buttons |= (u64)XINPUTPAD::RSTICK_UP;
	if( _state.Gamepad.sThumbRY <= -STICK_THRES_HOLD		)	buttons |= (u64)XINPUTPAD::RSTICK_DOWN;

	//	�t���O����
	_buttons  = buttons;
	_triggers = (buttons ^ oldButtons) & buttons;
	_releases = (buttons ^ oldButtons) & oldButtons;



	//	�o�C�u���[�V����
	if(_isVibration)
	{
		_vibrateTime -= GetDeltaTime();
		if(_vibrateTime <= 0.f)
		{
			VibrateOff();
		}
	}
}

//-----------------------------------------------------------------------------
//  @brief  �o�C�u���[�V����ON
//  @param	[in]	time	�쓮����
//  @param	[in]	left	�����[�^�[
//  @param	[in]	right	�E���[�^�[
//-----------------------------------------------------------------------------
void XInputPadManager::VibrateOnTime(f32 time, f32 left, f32 right)
{
	_vibrateTime = time;
	VibrateOn(left, right);
}

//-----------------------------------------------------------------------------
//  @brief  �o�C�u���[�V����ON
//  @param	[in]	left	�����[�^�[
//  @param	[in]	right	�E���[�^�[
//-----------------------------------------------------------------------------
void XInputPadManager::VibrateOn(f32 left, f32 right)
{
	if (_isController == false)	return;
	if (_vibrateActivate == false) return; 

	XINPUT_VIBRATION vibration;
	ZeroMemory( &vibration, sizeof(XINPUT_VIBRATION) );

	vibration.wLeftMotorSpeed  = (WORD)( min(left  * 65535.f,65535.f) );
	vibration.wRightMotorSpeed = (WORD)( min(right * 65535.f,65535.f) );

	XInputSetState(0, &vibration);
	_isVibration = true;
}

//-----------------------------------------------------------------------------
//  @brief  �o�C�u���[�V����OFF
//-----------------------------------------------------------------------------
void XInputPadManager::VibrateOff()
{
	if (_isController == false)	return;
	if(_vibrateActivate == false) return;

	XINPUT_VIBRATION vibration;
	ZeroMemory( &vibration, sizeof(XINPUT_VIBRATION) );
	XInputSetState(0, &vibration);
	_isVibration = false;
}

//-----------------------------------------------------------------------------
//  @brief  �L�[�X�P�[���擾
//  @param	[in]	pad	�L�[�R�[�h
//  @return	�L�[�X�P�[��
//-----------------------------------------------------------------------------
f32 XInputPadManager::GetScale( XINPUTPAD pad ) const
{
	s16 val = 0;
	switch(pad)
	{
	case XINPUTPAD::LEFT_TRIGGER:
		val = _state.Gamepad.bLeftTrigger;
		break;
	case XINPUTPAD::RIGHT_TRIGGER:
		val = _state.Gamepad.bRightTrigger;
		break;
	case XINPUTPAD::LSTICK_RIGHT:
		val = -_state.Gamepad.sThumbLX;
		break;
	case XINPUTPAD::LSTICK_LEFT:
		val =  _state.Gamepad.sThumbLX;
		break;
	case XINPUTPAD::LSTICK_UP:
		val =  _state.Gamepad.sThumbLY;
		break;
	case XINPUTPAD::LSTICK_DOWN:
		val = -_state.Gamepad.sThumbLY;
		break;
	case XINPUTPAD::RSTICK_RIGHT:
		val = -_state.Gamepad.sThumbRX;
		break;
	case XINPUTPAD::RSTICK_LEFT:
		val =  _state.Gamepad.sThumbRX;
		break;
	case XINPUTPAD::RSTICK_UP:
		val =  _state.Gamepad.sThumbRY;
		break;
	case XINPUTPAD::RSTICK_DOWN:
		val = -_state.Gamepad.sThumbRY;
		break;
	default:
		val = ((_buttons & (u32)pad) != 0) ? SHRT_MAX : 0;
		break;
	}

	f32 result = f32(val) * STICK_THRES_HOLD_FLOAT;
	return result;
}

//-----------------------------------------------------------------------------
//  @brief  ���X�e�B�b�N�擾
//  @param	[in]	minVal	�Œ�T�C�Y
//  @return	�X�e�B�b�N�̒l
//-----------------------------------------------------------------------------
Vector2 XInputPadManager::GetStickL( f32 minVal ) const
{
	Vector2 result;
	f32 x = sinf((f32(_state.Gamepad.sThumbLX) + FLT_EPSILON) / f32(SHRT_MAX));
	f32 y = sinf((f32(_state.Gamepad.sThumbLY) + FLT_EPSILON) / f32(SHRT_MAX));
	result.x = (x > minVal || x < -minVal) ? x : 0.f;
	result.y = (y > minVal || y < -minVal) ? y : 0.f;
	return result;

}

//-----------------------------------------------------------------------------
//  @brief  �E�X�e�B�b�N�擾
//  @param	[in]	minVal	�Œ�T�C�Y
//  @return	�X�e�B�b�N�̒l
//-----------------------------------------------------------------------------
Vector2 XInputPadManager::GetStickR( f32 minVal ) const
{
	Vector2 result;
	f32 x = sinf((f32(_state.Gamepad.sThumbRX) + FLT_EPSILON) / f32(SHRT_MAX));
	f32 y = sinf((f32(_state.Gamepad.sThumbRY) + FLT_EPSILON) / f32(SHRT_MAX));
	result.x = (x > minVal || x < -minVal) ? x : 0.f;
	result.y = (y > minVal || y < -minVal) ? y : 0.f;
	return result;
}

//-----------------------------------------------------------------------------
//  @brief  �ڑ���Ԃ̎擾
//  @return	�p�b�h���
//-----------------------------------------------------------------------------
DWORD XInputPadManager::UpdateControllerState()
{
	DWORD dwResult = XInputGetState(0, &_state);
	if (dwResult == ERROR_SUCCESS)
	{
		_isController = true;
	}
	else
	{
		_isController = false;
//		DEBUG_LOG(L"Controller Update Error");
	}
	return dwResult;
}


