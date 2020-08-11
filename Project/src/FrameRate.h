//=============================================================================
//!
//!	@file	FrameRate.h
//!	@brief	�t���[���v���N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//	�t���[�����䖢����

class FrameRate
{
public:
	//!	�R���X�g���N�^
	FrameRate();
	//!	�f�X�g���N�^
	virtual ~FrameRate();

	//!	�X�V
	void Update();

	//!	�`��
	void Render();

	//!	�f���^�^�C���̎擾
	//!	@return	�f���^�^�C��
	f32 GetDeltaTime()const
	{
		return _deltaTime;
	}

	//!	�t���[�����[�g�̎擾
	//!	@return	�t���[�����[�g
	f32 GetFrameRate()const
	{
		return _frameRate;
	}


private:	

	s32  _oldTime;		//!< �ߋ��̎���
	s32  _nowTime;		//!< ���݂̎���
	s32  _count;		//!< �J�E���g
	f32	 _frameRate;	//!< �t���[�����[�g
	s32* _frame;		//!< �t���[���P�ʂ̎���
	f32 _deltaTime;		//!< 1�t���[���ɂ�����������
};


//!	�f���^�^�C���̎擾
//!	@return	�f���^�^�C��
f32 GetDeltaTime();

//!	�t���[�����[�g�̎擾
//!	@return	�t���[�����[�g
f32 GetFrameRate();

//!	�f���^�^�C�����|�����l�̍쐬
//!	@return	�f���^�^�C�����|�����l
f32 GetDeltaValue(f32 val);

//!	�f���^�^�C�����|�����l�̍쐬
//!	@return	�f���^�^�C�����|�����l
Vector2 GetDeltaValue(const Vector2& val);

//!	�f���^�^�C�����|�����l�̍쐬
//!	@return	�f���^�^�C�����|�����l
Vector3 GetDeltaValue(const Vector3& val);

//!	�f���^�^�C�����|�����l�̍쐬
//!	@return	�f���^�^�C�����|�����l
Vector4 GetDeltaValue(const Vector4& val);

