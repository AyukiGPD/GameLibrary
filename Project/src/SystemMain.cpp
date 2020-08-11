//=============================================================================
//!
//!	@file	SystemMain.cpp
//!	@brief	���C���V�X�e��
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
//	�R���X�g���N�^
//-----------------------------------------------------------------------------
SystemMain::SystemMain()
: _hwnd(nullptr)				// �E�B���h�E�n���h��
, _isExit(false)				// �I���t���O
, _pWork(nullptr)				// �V�[��
, _isRunning(false)
{
}

//-----------------------------------------------------------------------------
//	�f�X�g���N�^
//-----------------------------------------------------------------------------
SystemMain::~SystemMain()
{
}

//-----------------------------------------------------------------------------
//! ���C�����[�v
//! @param  [in]    titleName     �^�C�g����
//! @param  [in]    className     �N���X��
//! @param  [in]    isWindowed    true�F�E�B���h�E, false�F�t���X�N���[��
//! @return 
//-----------------------------------------------------------------------------
s32 SystemMain::MainLoop(
	LPCWSTR titleName,
	LPCWSTR className,
	bool     isWindowed   /* = true */
	)
{
	if (_isRunning == true)
	{
		return 0;
	}

	_isRunning = true;
	assert((SCRN_WIDTH != 0) || (SCRN_HEIGHT != 0));

	HINSTANCE  hInstance = GetModuleHandle(nullptr);
	{
		//-----------------------------------------------------------------
		// �E�B���h�E�N���X�̓o�^
		//-----------------------------------------------------------------
		WNDCLASSEX	 wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);									// ���̍\���̂̃T�C�Y
		wcex.style = CS_HREDRAW | CS_VREDRAW;								// �E�B���h�E�̋���
		wcex.lpfnWndProc = SystemMain::WndProc;								// �E�B���h�E�v���V�[�W���֐���
		wcex.cbClsExtra = 0;												// ���̍\���̂̌�Ɋm�ۂ���t�����̃o�C�g��
		wcex.cbWndExtra = 0;												// �E�B���h�E �C���X�^���X�̌�Ɋm�ۂ���t�����̃o�C�g��
		wcex.hInstance = hInstance;											// �������g�̃C���X�^���X�n���h��
		wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);					// �f�t�H���g�̃A�v���P�[�V�����A�C�R��
		//wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);						// �J�[�\���̎��
		wcex.hCursor = LoadCursor(nullptr, IDC_CROSS);						// �J�[�\���̎��
		wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);			// �w�i�F
		wcex.lpszMenuName = nullptr;										// ���j���[��
		wcex.lpszClassName = className;										// �E�B���h�E�N���X��
		wcex.hIconSm = nullptr;												// �A�C�R���f�[�^�i16x16�j
		RegisterClassEx(&wcex);


		//-----------------------------------------------------------------
		// �K�v�ȃE�B���h�E�T�C�Y���v�Z
		//-----------------------------------------------------------------
		RECT	rect = { 0, 0, SCRN_WIDTH, SCRN_HEIGHT };

		DWORD style;
		s32 x, y, w, h;

		if (isWindowed)
		{
			//---- �E�B���h�E
			//	�E�B���h�E�̓T�C�Y�ύX�s��
//			style = WS_OVERLAPPEDWINDOW;	// �I�[�o�[���b�v
			style = WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE;
			::AdjustWindowRect(&rect, style, false);
			w = rect.right - rect.left;
			h = rect.bottom - rect.top;
			//x = (GetSystemMetrics(SM_CXSCREEN) - w) >> 1;
			//y = (GetSystemMetrics(SM_CYSCREEN) - h) >> 1;
			x = 0;
			y = 0;
		}
		else
		{
			//---- �t���X�N���[��
			//	�T�C�Y�ɍ��킹�ăX�N���[���̃T�C�Y�ŕ\��

			::GetWindowRect(GetDesktopWindow(), &rect);
			style = WS_POPUPWINDOW;		// �|�b�v�A�b�v
//			style = WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE;
			w = SCRN_WIDTH;
			h = SCRN_HEIGHT;
			x = rect.left;
			y = rect.top;

			//DEVMODE devMode;
			//s32 nMode = 0;
			////	��ʃT�C�Y�ύX
			//while (EnumDisplaySettings(nullptr, nMode++, &devMode))
			//{
			//	//�T�C�Y�ɃT�|�[�g���Ă��邩
			//	if (devMode.dmPelsWidth != SCRN_WIDTH || devMode.dmPelsHeight != SCRN_HEIGHT)continue;
			//	//32�r�b�g�J���[���T�|�[�g���Ă��邩
			//	if (devMode.dmBitsPerPel != 32)continue;
			//	//�e�X�g���Đ؂�ւ����Ȃ�
			//	if (ChangeDisplaySettings(&devMode, CDS_TEST) == DISP_CHANGE_SUCCESSFUL)break;
			//}
			//ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);

		}

		//-----------------------------------------------------------------
		// �E�B���h�E�̍쐬
		//-----------------------------------------------------------------
		_hwnd = CreateWindow(
			className,			// �E�B���h�E�N���X��(WNDCLASSEX�Ɠ������O)
			titleName,			// �^�C�g����
			style,				// �E�B���h�E�X�^�C��
			x,					// �w
			y,					// �x
			w,					// ��
			h,					// ����
			nullptr,			// �e�̃E�B���h�E�n���h��	
			nullptr,			// ���j���[�n���h��
			hInstance,			// �C���X�^���X�n���h��
			nullptr				// WM_CREATE�̈���
			);
		if (_hwnd == nullptr)
		{
			_isRunning = false;
			return 0;
		}
	}


	//	OpenGL������
	HGLRC hrc = Open_GL::InitializeOpenGL(_hwnd);
	if (hrc == nullptr) {
		_isRunning = false;
		return 0;
	}
	//	GLEW������
	if (Open_GL::InitializeGLEW() == false) {
		_isRunning = false;
		return 0;
	}


	//-----------------------------------------------------------------
	// �V�X�e���̏�����
	//-----------------------------------------------------------------
	if (Init() == false)
	{
		_isRunning = false;
		return 0;
	}

	ShowWindow(_hwnd, SW_SHOW);
	UpdateWindow(_hwnd);

	//================================================================
	// ���C�����b�Z�[�W���[�v
	//================================================================
	MSG		msg;
	ZeroMemory(&msg, sizeof(msg));

	do
	{
		if (_isExit) break;

		// ���b�Z�[�W�̎擾��GetMessage()�ł͂Ȃ�PeekMessage()���g�p�B
		// GetMessage ()�̓��b�Z�[�W���Ȃ��ꍇ�ɃX���[�v��~����̂ɑ΂���
		// PeekMessage()�͑҂����ɖ߂��Ă���B�Q�[���Ȃǂ̃��A���^�C������������
		// �A�v���P�[�V������PeekMessage()�̂ق����D�܂���
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);		// ���b�Z�[�W�̕ϊ�
			DispatchMessage(&msg);		// ���b�Z�[�W�̓]���ʒm
		}
		else
		{
			//--- �V�X�e���̍X�V
			UpdateFrame();
		}
	}
	while (msg.message != WM_QUIT);


	//-----------------------------------------------------------------
	// �V�X�e���̉��
	//-----------------------------------------------------------------
	Clear();
	Open_GL::DestroyOpenGL(_hwnd, hrc);
	UnregisterClass(className, hInstance);
	return (s32)msg.wParam;
}
//-----------------------------------------------------------------------------
//	������
//	@retval true    ����������
//	@retval false   ���������s
//-----------------------------------------------------------------------------
bool SystemMain::Init()
{
	_pWork = new SystemWork();
	if ((_pWork == nullptr) || (_pWork->Init() == false))
	{
		return false;
	}

#ifndef _DEBUG
	//	�J�[�\����\��
	int count = 0;
	while (count >= 0)
	{
		count = ShowCursor(FALSE);
	}
#endif

	return true;
}

//-----------------------------------------------------------------------------
//	1�t���[���X�V
//	@retval �Ȃ�
//-----------------------------------------------------------------------------
void SystemMain::UpdateFrame()
{
	//	Alt+F4�I��
	if (GetKeyState(VK_MENU) & 0x8000)
	{
		if (GetKeyState(VK_F4) & 0x8000)
		{
			_isExit = true;
			return;
		}
	}

#ifdef _DEBUG
	if (GetKeyState(VK_ESCAPE) & 0x8000)
	{
		_isExit = true;
		return;
	}
#endif

	//	�X�V����
	_pWork->Update();

	//	�`�揈��
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderManager* pRenderer = Manager::Renderer();

		//	�A���t�@�L����
//		pRenderer->EnableAlphaBlending();

		pRenderer->DisableDepthTest();			//	DEPTH����
		glMatrixMode(GL_PROJECTION);			//	�v���W�F�N�V�����ݒ�
		glLoadIdentity();						//	�P�ʍs��
		LoadMatrix(Matrix::OrthoMatrix(0.0f, (f32)SCRN_WIDTH, (f32)SCRN_HEIGHT, 0.0f, 1.0f, -1.0f));

		glMatrixMode(GL_MODELVIEW);				//	���f���r���[�ݒ�
		glLoadIdentity();						//	�P�ʍs��
	}

	_pWork->Render();



	//---- ��ʍX�V
	{
		HDC hdc = GetDC(_hwnd);
//		if (hdc)
		{
			SwapBuffers(hdc);
			ReleaseDC(_hwnd, hdc);
		}
//		InvalidateRect(_hwnd, nullptr, false);
	}
}

//-----------------------------------------------------------------------------
//	���
//	@retval �Ȃ�
//-----------------------------------------------------------------------------
void SystemMain::Clear()
{
	_isRunning = false;
	_pWork->Exit();
	SafeDelete(_pWork);
}
//-----------------------------------------------------------------------------
//	�E�B���h�E�v���V�[�W��
//	@param  [in]    hwnd	�E�B���h�E�n���h��
//	@param  [in]    message	���b�Z�[�W�̎��
//	@param  [in]    wparam	�p�����[�^1
//	@param  [in]    lparam	�p�����[�^2
//	@return ���b�Z�[�W�����̌��ʂ̒l
//-----------------------------------------------------------------------------
LRESULT CALLBACK SystemMain::WndProc(HWND hWnd,	UINT message,WPARAM wparam,	LPARAM lparam)
{
	switch (message)
	{
	//-----------------------------------------------------------------
	case WM_CREATE:		// �E�B���h�E�쐬����
	//-----------------------------------------------------------------
	{
							CREATESTRUCT* p = (CREATESTRUCT*)lparam;
							UNUSED(p);
	}
		return 0;
	//-----------------------------------------------------------------
	case WM_PAINT:		// �ĕ`��v��
	//-----------------------------------------------------------------
	{
							PAINTSTRUCT	ps;
							HDC			hdc = BeginPaint(hWnd, &ps);
							UNUSED(hdc);
							EndPaint(hWnd, &ps);
	}
		return 0;
	//-----------------------------------------------------------------
	case WM_DESTROY:	// �E�B���h�E���폜����Ƃ�
	//-----------------------------------------------------------------
	{
							PostQuitMessage(0);	// GetMessage()�̂Ƃ��� 0 ���Ԃ�
	}
		break;
	//-----------------------------------------------------------------
	} // switch( message )

	//---- �f�t�H���g�̃E�B���h�E�v���V�[�W��
	return DefWindowProc(hWnd, message, wparam, lparam);
}


//!	�E�B���h�E�̃A�N�e�B�u��Ԃ̎擾
//!	@retval	true �A�N�e�B�u : false ��A�N�e�B�u
bool IsWindowActive()
{
	SystemMain* pSystem = SystemMain::GetInstance();
	//	���݃A�N�e�B�u�ȃE�B���h�E�̃n���h�����擾
	HWND activeHwnd = GetActiveWindow();
	HWND myHwnd = pSystem->GetHwnd();
	return (myHwnd == activeHwnd);
}



