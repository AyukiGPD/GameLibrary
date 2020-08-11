//=============================================================================
//!
//!	@file	XInputPad.h
//!	@brief	�R���g���[���N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

#include <XInput.h>
#pragma comment(lib,"XInput9_1_0.lib")

enum class XINPUTPAD : u64
{
	//�f�W�^�������L�[
	DPAD_LEFT		=	( static_cast<u64>(1) <<   0 ),		//!< ��
	DPAD_RIGHT		=	( static_cast<u64>(1) <<   1 ),		//!< �E
	DPAD_UP			=	( static_cast<u64>(1) <<   2 ),		//!< ��
	DPAD_DOWN		=	( static_cast<u64>(1) <<   3 ),		//!< ��

	A				=	( static_cast<u64>(1) <<   4 ),		//!< A
	B				=	( static_cast<u64>(1) <<   5 ),		//!< B
	X				=	( static_cast<u64>(1) <<   6 ),		//!< X
	Y				=	( static_cast<u64>(1) <<   7 ),		//!< Y
	START			=	( static_cast<u64>(1) <<   8 ),		//!< START
	BACK			=	( static_cast<u64>(1) <<   9 ),		//!< BACK
	LEFT_THUMB		=	( static_cast<u64>(1) <<  10 ),		//!< ���X�e�B�b�N�{�^��
	RIGHT_THUMB		=	( static_cast<u64>(1) <<  11 ),		//!< �E�X�e�B�b�N�{�^��
	LEFT_SHOULDER	=	( static_cast<u64>(1) <<  12 ),		//!< L�{�^��
	RIGHT_SHOULDER	=	( static_cast<u64>(1) <<  13 ),		//!< R�{�^��
	LEFT_TRIGGER	=	( static_cast<u64>(1) <<  14 ),		//!< L�g���K�[
	RIGHT_TRIGGER	=	( static_cast<u64>(1) <<  15 ),		//!< R�g���K�[

	//�A�i���O�����L�[
	//	���X�e�B�b�N
	LSTICK_LEFT		=	( static_cast<u64>(1) <<  16 ),		//!< ��
	LSTICK_RIGHT	=	( static_cast<u64>(1) <<  17 ),		//!< �E
	LSTICK_UP		=	( static_cast<u64>(1) <<  18 ),		//!< ��
	LSTICK_DOWN		=	( static_cast<u64>(1) <<  19 ),		//!< ��
	//	�E�X�e�B�b�N
	RSTICK_LEFT		=	( static_cast<u64>(1) <<  20 ),		//!< ��
	RSTICK_RIGHT	=	( static_cast<u64>(1) <<  21 ),		//!< �E
	RSTICK_UP		=	( static_cast<u64>(1) <<  22 ),		//!< ��
	RSTICK_DOWN		=	( static_cast<u64>(1) <<  23 ),		//!< ��

};

class XInputPadManager : public ManagerBase
{
public:

public:
	//!	�R���X�g���N�^
	XInputPadManager();
	//!	�f�X�g���N�^
	virtual ~XInputPadManager();

	//!	������
	//!	@param	[in]	vibration	�o�C�u���[�V�����L��/����
	//!	@param	[in]	retry		�R���g���[���[���h�����ĂȂ��ꍇ�̃|�b�v�A�b�v�\��
	//! @retval true    ����������
	//! @retval false   ���������s
	bool Init(bool vibration = true,bool retry = true);
	//!	�X�V
	void Update();

	//!	�o�C�u���[�V����ON
	//!	@param	[in]	time	�쓮����
	//!	@param	[in]	left	�����[�^�[
	//!	@param	[in]	right	�E���[�^�[
	void VibrateOnTime(f32 time,f32 left = 1.f, f32 right = 1.f);
	//!	�o�C�u���[�V����ON
	//!	@param	[in]	left	�����[�^�[
	//!	@param	[in]	right	�E���[�^�[
	void VibrateOn(f32 left = 1.f, f32 right = 1.f);
	//!	�o�C�u���[�V����OFF
	void VibrateOff();


	//!	�X�e�[�g�擾
	//!	@return	�R���g���[���[�X�e�[�g
	XINPUT_STATE GetState()const{ return _state; }

	//!	���L�[
	//!	@param	[in]	pad	�L�[�R�[�h
	bool GetButtons(XINPUTPAD pad) const
	{
		return (_buttons & (u32)pad) != 0;
	}
	//!	�g���K�[
	//!	@param	[in]	pad	�L�[�R�[�h
	bool GetTriggers(XINPUTPAD pad) const
	{
		return (_triggers & (u32)pad) != 0;
	}
	//!	�����[�X
	//!	@param	[in]	pad	�L�[�R�[�h
	bool GetReleases(XINPUTPAD pad) const
	{
		return (_releases & (u32)pad) != 0;
	}

	//!	�L�[�X�P�[���擾
	//!	@param	[in]	pad	�L�[�R�[�h
	//!	@return	�L�[�X�P�[��
	f32 GetScale( XINPUTPAD pad ) const;

	//!	���X�e�B�b�N�擾
	//!	@param	[in]	minVal	�Œ�T�C�Y
	//!	@return	�X�e�B�b�N�̒l
	Vector2 GetStickL( f32 minVal = 0.2f ) const;
	//!	�E�X�e�B�b�N�擾
	//!	@param	[in]	minVal	�Œ�T�C�Y
	//!	@return	�X�e�B�b�N�̒l
	Vector2 GetStickR( f32 minVal = 0.2f ) const;


	//!	�R���g���[���[�̗L��
	bool IsController() const { return _isController; }
	//!	�o�C�u���[�V����ON/OFF
	bool IsVibrate()const{ return _isVibration; }


private:
	//!	�ڑ���Ԃ̎擾
	//!	@return	�p�b�h���
	DWORD UpdateControllerState();

private:

	XINPUT_STATE	_state;			//!< �R���g���[���[�X�e�[�g
	bool			_isController;	//!< �R���g���[���[�ڑ�

	u32				_buttons;		//!< ���L�[�f�[�^
	u32				_triggers;		//!< �g���K�[�L�[�f�[�^
	u32				_releases;		//!< �����[�X�L�[�f�[�^

	bool			_isVibration;		//!< �o�C�u���[�V����

	bool			_vibrateActivate;	//!< �o�C�u���[�V�����L��/����

	f32				_vibrateTime;
};

MANAGER_INTERFACE(XInputPadManager, XInput);

