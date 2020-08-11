//=============================================================================
//!
//!	@file	OpenGL.cpp
//!	@brief	OpenGL����������
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
//!	OpenGL�̃s�N�Z���t�H�[�}�b�g��ݒ�
//!	@param	hdc		[in]	�f�B�X�v���C�f�o�C�X�R���e�L�X�g
//!	@retval	true	����I��	(����)
//!	@retval	false	�G���[�I��	(���s)
//-----------------------------------------------------------------------------
static bool SetupPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),								// ���̍\���̂̃T�C�Y
		1,															// �\���̂̃o�[�W�����ԍ�(1)
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,	// �����t���O(�_�u���o�b�t�@)
		PFD_TYPE_RGBA,												// �s�N�Z���̃J���[�f�[�^(RGBA�J���[ or �C���f�b�N�X�J���[)
		32,															// �J���[�̃r�b�g��
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0,  0, 0, 0,
		32,															// �f�v�X�o�b�t�@�̃s�N�Z������̃r�b�g��
		0,															// �X�e���V���o�b�t�@�̃s�N�Z������̃r�b�g��
		0,
		PFD_MAIN_PLANE,												// ���C���^�C�v(Win32�ł�PFD_MAIN_PLANE�ł���K�v������)
		0,															// �I�[�o�[���C�ƃA���_�[���C�̐�(0)
		0, 0, 0
	};

	// ���݂̃R���e�L�X�g�Ƀs�N�Z���t�H�[�}�b�g��ݒ�
	u32		pixelformat;
	if( (pixelformat = ChoosePixelFormat(hdc, &pfd) ) == 0 )
	{
		MessageBox(nullptr, L"�s�N�Z���t�H�[�}�b�g�̑I���Ɏ��s���܂���.", L"SetupPixelFormat()", MB_OK);
		return false;
	}
	// �s�N�Z���t�H�[�}�b�g��ݒ�
    if( !SetPixelFormat(hdc, pixelformat, &pfd) )
	{
		MessageBox(nullptr, L"�s�N�Z���t�H�[�}�b�g�̐ݒ�Ɏ��s���܂���.", L"SetupPixelFormat()", MB_OK);
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
//!	OpenGL��������
//!	@param	hwnd	[in]	�E�B���h�E�n���h��
//!	@retval	OpenGL���\�[�X�n���h��(nullptr�̏ꍇ�͏��������s)
//-----------------------------------------------------------------------------
HGLRC Open_GL::InitializeOpenGL(HWND hwnd)
{
	//------------------------------------------------------------------
	// �E�B���h�E�̃f�o�C�X�R���e�L�X�g���擾
	//------------------------------------------------------------------
	HDC		hdc = GetDC(hwnd);
	if (hdc == nullptr)
	{
		//�R���e�L�X�g�����ĂȂ�������G���[
		MessageBox(hwnd, L"�f�o�C�X�R���e�L�X�g�擾�Ɏ��s���܂���.", L"InitializeOpenGL()", MB_OK);
		return nullptr;
	}
	//------------------------------------------------------------------
	// �s�N�Z���t�H�[�}�b�g��ݒ�
	//------------------------------------------------------------------
	if( SetupPixelFormat(hdc) == false )
	{
		return nullptr;
	}

	//------------------------------------------------------------------
	// OpenGL���\�[�X�n���h���𐶐�
	//------------------------------------------------------------------
	HGLRC	hrc = wglCreateContext(hdc);
	if( hrc == 0 )
	{
		// OpenGL���\�[�X�n���h���𐶐��ł��Ȃ�������G���[
		MessageBox(hwnd, L"OpenGL���\�[�X�n���h�������Ɏ��s���܂���.", L"InitializeOpenGL()", MB_OK);
		return nullptr;
	}
	//---- OpenGL���\�[�X�ƃR���e�L�X�g���֘A�t��
	if( wglMakeCurrent(hdc, hrc) == FALSE )
	{
		MessageBox(hwnd, L"OpenGL���\�[�X�ƃR���e�L�X�g�֘A�t���Ɏ��s���܂���.", L"InitializeOpenGL()", MB_OK);
		return nullptr;
	}

	//------------------------------------------------------------------
	// OpenGL�����ݒ�
	//------------------------------------------------------------------
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// ��ʃN���A�̐ݒ�
	glClearDepth(1.0f);						// �y�o�b�t�@�̏������l

	//------------------------------------------------------------------
	// �f�o�C�X�R���e�L�X�g���������
	//------------------------------------------------------------------
	ReleaseDC(hwnd, hdc);
	return hrc;
}

//-----------------------------------------------------------------------------
//!	OpenGL�����
//!	@param	hwnd	[in]	�E�B���h�E�n���h��
//!	@param	hrc		[in]	OpenGL���\�[�X�n���h��
//!	@retval	true	����I��	(����)
//!	@retval	false	�G���[�I��	(���s)
//-----------------------------------------------------------------------------
bool Open_GL::DestroyOpenGL(HWND hwnd, HGLRC hrc)
{
	// ���\�[�X�ƃf�o�C�X�R���e�L�X�g�����
	wglMakeCurrent(0, 0);
	//------------------------------------------------------------------
	// �R���e�L�X�g���폜
	//------------------------------------------------------------------
    if( wglDeleteContext(hrc) == FALSE )
	{
		MessageBox(hwnd, L"OpenGL���\�[�X�̉���Ɏ��s���܂���.", L"DestroyOpenGL()", MB_OK);
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//! GLEW�̏�����
//! @retval true    ����������
//! @retval false   ���������s
//-----------------------------------------------------------------------------
bool Open_GL::InitializeGLEW()
{
	//	GLEW������
	if (glewInit() != GLEW_OK)
	{
		return false;
	}
#ifdef _DEBUG
	std::cout << "Vendor :"		<< glGetString(GL_VENDOR)					<< std::endl;
	std::cout << "GPU : "		<< glGetString(GL_RENDERER)					<< std::endl;
	std::cout << "OpenGL ver. "	<< glGetString(GL_VERSION)					<< std::endl;
	std::cout << "GLSL ver. "	<< glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif

	return true;
}


