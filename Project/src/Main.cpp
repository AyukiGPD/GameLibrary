//=============================================================================
//!
//!	@file	Main.cpp
//!	@brief	メインソース
//!
//!	@author	YKCH
//!
//=============================================================================
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif // #if NDEBUG
#include "stdafx.h"

s32 main()			//	メインシステム
{

#ifdef _DEBUG
	::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // #if _DEBUG

	s32 result = 0;
	{
		// TODO 外部で設定するようにGame側にクラスを作成する
			// TODO Initializerみたいなものを作る
			// TODO SceneGameを登録する機能も入れる
		SystemMain main;
		bool window = true;
#ifdef _DEBUG
		window = false;
#else
		window = false;
#endif
		result = main.MainLoop(
			L"GameLibrary",		// タイトル名
			L"GameLibrary",		// クラス名
			window				// true：ウィンドウ, false：フルスクリーン
			);
	}
	return 0;
}
