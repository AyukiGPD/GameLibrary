//=============================================================================
//!
//!	@file	OpenGL.h
//!	@brief	OpenGL初期化処理
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//=============================================================================
//! OpenGLライブラリ
//=============================================================================
namespace Open_GL
{
	//!	OpenGLを初期化
	//! @param  [in]    hwnd    ウィンドウハンドル
	//! @return 描画コンテキスト(nullptrなら失敗)
	HGLRC	InitializeOpenGL(HWND hwnd);

	//!	OpenGLを解放
	//! @param  [in]    hwnd    ウィンドウハンドル
	//! @retval true    成功
	//! @retval false   失敗
	bool	DestroyOpenGL(HWND hwnd, HGLRC hrc);

	//! GLEWの初期化
	//! @retval true    初期化成功
	//! @retval false   初期化失敗
	bool	InitializeGLEW();
}	//	namespace Open_GL


