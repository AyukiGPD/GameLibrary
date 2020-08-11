//=============================================================================
//!
//!	@file	OpenGL.h
//!	@brief	OpenGL����������
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//=============================================================================
//! OpenGL���C�u����
//=============================================================================
namespace Open_GL
{
	//!	OpenGL��������
	//! @param  [in]    hwnd    �E�B���h�E�n���h��
	//! @return �`��R���e�L�X�g(nullptr�Ȃ玸�s)
	HGLRC	InitializeOpenGL(HWND hwnd);

	//!	OpenGL�����
	//! @param  [in]    hwnd    �E�B���h�E�n���h��
	//! @retval true    ����
	//! @retval false   ���s
	bool	DestroyOpenGL(HWND hwnd, HGLRC hrc);

	//! GLEW�̏�����
	//! @retval true    ����������
	//! @retval false   ���������s
	bool	InitializeGLEW();
}	//	namespace Open_GL


