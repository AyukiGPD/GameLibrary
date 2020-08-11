//=============================================================================
//!
//!	@file	OpenGL.cpp
//!	@brief	OpenGL初期化処理
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
//!	OpenGLのピクセルフォーマットを設定
//!	@param	hdc		[in]	ディスプレイデバイスコンテキスト
//!	@retval	true	正常終了	(成功)
//!	@retval	false	エラー終了	(失敗)
//-----------------------------------------------------------------------------
static bool SetupPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),								// この構造体のサイズ
		1,															// 構造体のバージョン番号(1)
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,	// 特性フラグ(ダブルバッファ)
		PFD_TYPE_RGBA,												// ピクセルのカラーデータ(RGBAカラー or インデックスカラー)
		32,															// カラーのビット数
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0,  0, 0, 0,
		32,															// デプスバッファのピクセル当りのビット数
		0,															// ステンシルバッファのピクセル当りのビット数
		0,
		PFD_MAIN_PLANE,												// レイヤタイプ(Win32ではPFD_MAIN_PLANEである必要がある)
		0,															// オーバーレイとアンダーレイの数(0)
		0, 0, 0
	};

	// 現在のコンテキストにピクセルフォーマットを設定
	u32		pixelformat;
	if( (pixelformat = ChoosePixelFormat(hdc, &pfd) ) == 0 )
	{
		MessageBox(nullptr, L"ピクセルフォーマットの選択に失敗しました.", L"SetupPixelFormat()", MB_OK);
		return false;
	}
	// ピクセルフォーマットを設定
    if( !SetPixelFormat(hdc, pixelformat, &pfd) )
	{
		MessageBox(nullptr, L"ピクセルフォーマットの設定に失敗しました.", L"SetupPixelFormat()", MB_OK);
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
//!	OpenGLを初期化
//!	@param	hwnd	[in]	ウィンドウハンドル
//!	@retval	OpenGLリソースハンドラ(nullptrの場合は初期化失敗)
//-----------------------------------------------------------------------------
HGLRC Open_GL::InitializeOpenGL(HWND hwnd)
{
	//------------------------------------------------------------------
	// ウィンドウのデバイスコンテキストを取得
	//------------------------------------------------------------------
	HDC		hdc = GetDC(hwnd);
	if (hdc == nullptr)
	{
		//コンテキストが取れてなかったらエラー
		MessageBox(hwnd, L"デバイスコンテキスト取得に失敗しました.", L"InitializeOpenGL()", MB_OK);
		return nullptr;
	}
	//------------------------------------------------------------------
	// ピクセルフォーマットを設定
	//------------------------------------------------------------------
	if( SetupPixelFormat(hdc) == false )
	{
		return nullptr;
	}

	//------------------------------------------------------------------
	// OpenGLリソースハンドラを生成
	//------------------------------------------------------------------
	HGLRC	hrc = wglCreateContext(hdc);
	if( hrc == 0 )
	{
		// OpenGLリソースハンドラを生成できなかったらエラー
		MessageBox(hwnd, L"OpenGLリソースハンドラ生成に失敗しました.", L"InitializeOpenGL()", MB_OK);
		return nullptr;
	}
	//---- OpenGLリソースとコンテキストを関連付け
	if( wglMakeCurrent(hdc, hrc) == FALSE )
	{
		MessageBox(hwnd, L"OpenGLリソースとコンテキスト関連付けに失敗しました.", L"InitializeOpenGL()", MB_OK);
		return nullptr;
	}

	//------------------------------------------------------------------
	// OpenGL初期設定
	//------------------------------------------------------------------
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// 画面クリアの設定
	glClearDepth(1.0f);						// Ｚバッファの初期化値

	//------------------------------------------------------------------
	// デバイスコンテキストを解放する
	//------------------------------------------------------------------
	ReleaseDC(hwnd, hdc);
	return hrc;
}

//-----------------------------------------------------------------------------
//!	OpenGLを解放
//!	@param	hwnd	[in]	ウィンドウハンドル
//!	@param	hrc		[in]	OpenGLリソースハンドラ
//!	@retval	true	正常終了	(成功)
//!	@retval	false	エラー終了	(失敗)
//-----------------------------------------------------------------------------
bool Open_GL::DestroyOpenGL(HWND hwnd, HGLRC hrc)
{
	// リソースとデバイスコンテキストを解放
	wglMakeCurrent(0, 0);
	//------------------------------------------------------------------
	// コンテキストを削除
	//------------------------------------------------------------------
    if( wglDeleteContext(hrc) == FALSE )
	{
		MessageBox(hwnd, L"OpenGLリソースの解放に失敗しました.", L"DestroyOpenGL()", MB_OK);
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//! GLEWの初期化
//! @retval true    初期化成功
//! @retval false   初期化失敗
//-----------------------------------------------------------------------------
bool Open_GL::InitializeGLEW()
{
	//	GLEW初期化
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


