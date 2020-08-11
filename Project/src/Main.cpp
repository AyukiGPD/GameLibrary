//=============================================================================
//!
//!	@file	Main.cpp
//!	@brief	���C���\�[�X
//!
//!	@author	YKCH
//!
//=============================================================================
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif // #if NDEBUG
#include "stdafx.h"

s32 main()			//	���C���V�X�e��
{

#ifdef _DEBUG
	::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // #if _DEBUG

	s32 result = 0;
	{
		// TODO �O���Őݒ肷��悤��Game���ɃN���X���쐬����
			// TODO Initializer�݂����Ȃ��̂����
			// TODO SceneGame��o�^����@�\�������
		SystemMain main;
		bool window = true;
#ifdef _DEBUG
		window = false;
#else
		window = false;
#endif
		result = main.MainLoop(
			L"GameLibrary",		// �^�C�g����
			L"GameLibrary",		// �N���X��
			window				// true�F�E�B���h�E, false�F�t���X�N���[��
			);
	}
	return 0;
}
