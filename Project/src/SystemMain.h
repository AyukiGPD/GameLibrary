//=============================================================================
//!
//!	@file	SystemMain.h
//!	@brief	���C���V�X�e��
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//-------------------------------------------------------------------
//!	���C��
//-------------------------------------------------------------------
class SystemMain : public SingletonInherit<SystemMain>
{
public:
	//!	�R���X�g���N�^
	SystemMain();
	//!	�f�X�g���N�^
	virtual ~SystemMain();

	//! ���C�����[�v
	//! @param  [in]    titleName     �^�C�g����
	//! @param  [in]    className     �N���X��
	//! @param  [in]    screenW       ��ʂ̕�
	//! @param  [in]    screenH       ��ʂ̍���
	//! @param  [in]    isWindowed    true�F�E�B���h�E, false�F�t���X�N���[��
	//! @return 
	s32					MainLoop(
		LPCWSTR titleName,
		LPCWSTR className,
		bool    isWindowed = true
		);


	//! �E�B���h�E�n���h�����擾
	//! @return �E�B���h�E�n���h��
	MY_FORCEINLINE
	HWND				GetHwnd() const
	{
		return _hwnd;
	}

private:

	//! ������
	//! @retval true    ����������
	//! @retval false   ���������s
	bool				Init();

	//! 1�t���[���X�V
	//! @retval �Ȃ�
	void				UpdateFrame();

	//! ���
	//! @retval �Ȃ�
	void				Clear();

private:
	HWND				_hwnd;					//!< �E�B���h�E�n���h��
	bool				_isExit;				//!< �I���t���O
	bool				_isRunning;

	SystemWork* _pWork;								//!< ���C���N���X

	//! �E�B���h�E�v���V�[�W��
	static LRESULT CALLBACK	WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
};

//!	�E�B���h�E�̃A�N�e�B�u��Ԃ̎擾
//!	@retval	true �A�N�e�B�u : false ��A�N�e�B�u
bool IsWindowActive();

