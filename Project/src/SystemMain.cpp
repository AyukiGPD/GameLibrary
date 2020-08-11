//=============================================================================
//!
//!	@file	SystemMain.cpp
//!	@brief	メインシステム
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
SystemMain::SystemMain()
: _hwnd(nullptr)				// ウィンドウハンドル
, _isExit(false)				// 終了フラグ
, _pWork(nullptr)				// シーン
, _isRunning(false)
{
}

//-----------------------------------------------------------------------------
//	デストラクタ
//-----------------------------------------------------------------------------
SystemMain::~SystemMain()
{
}

//-----------------------------------------------------------------------------
//! メインループ
//! @param  [in]    titleName     タイトル名
//! @param  [in]    className     クラス名
//! @param  [in]    isWindowed    true：ウィンドウ, false：フルスクリーン
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
		// ウィンドウクラスの登録
		//-----------------------------------------------------------------
		WNDCLASSEX	 wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);									// この構造体のサイズ
		wcex.style = CS_HREDRAW | CS_VREDRAW;								// ウィンドウの挙動
		wcex.lpfnWndProc = SystemMain::WndProc;								// ウィンドウプロシージャ関数名
		wcex.cbClsExtra = 0;												// この構造体の後に確保する付加情報のバイト数
		wcex.cbWndExtra = 0;												// ウィンドウ インスタンスの後に確保する付加情報のバイト数
		wcex.hInstance = hInstance;											// 自分自身のインスタンスハンドル
		wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);					// デフォルトのアプリケーションアイコン
		//wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);						// カーソルの種類
		wcex.hCursor = LoadCursor(nullptr, IDC_CROSS);						// カーソルの種類
		wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);			// 背景色
		wcex.lpszMenuName = nullptr;										// メニュー名
		wcex.lpszClassName = className;										// ウィンドウクラス名
		wcex.hIconSm = nullptr;												// アイコンデータ（16x16）
		RegisterClassEx(&wcex);


		//-----------------------------------------------------------------
		// 必要なウィンドウサイズを計算
		//-----------------------------------------------------------------
		RECT	rect = { 0, 0, SCRN_WIDTH, SCRN_HEIGHT };

		DWORD style;
		s32 x, y, w, h;

		if (isWindowed)
		{
			//---- ウィンドウ
			//	ウィンドウはサイズ変更不可
//			style = WS_OVERLAPPEDWINDOW;	// オーバーラップ
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
			//---- フルスクリーン
			//	サイズに合わせてスクリーンのサイズで表示

			::GetWindowRect(GetDesktopWindow(), &rect);
			style = WS_POPUPWINDOW;		// ポップアップ
//			style = WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE;
			w = SCRN_WIDTH;
			h = SCRN_HEIGHT;
			x = rect.left;
			y = rect.top;

			//DEVMODE devMode;
			//s32 nMode = 0;
			////	画面サイズ変更
			//while (EnumDisplaySettings(nullptr, nMode++, &devMode))
			//{
			//	//サイズにサポートしているか
			//	if (devMode.dmPelsWidth != SCRN_WIDTH || devMode.dmPelsHeight != SCRN_HEIGHT)continue;
			//	//32ビットカラーをサポートしているか
			//	if (devMode.dmBitsPerPel != 32)continue;
			//	//テストして切り替えれるなら
			//	if (ChangeDisplaySettings(&devMode, CDS_TEST) == DISP_CHANGE_SUCCESSFUL)break;
			//}
			//ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);

		}

		//-----------------------------------------------------------------
		// ウィンドウの作成
		//-----------------------------------------------------------------
		_hwnd = CreateWindow(
			className,			// ウィンドウクラス名(WNDCLASSEXと同じ名前)
			titleName,			// タイトル名
			style,				// ウィンドウスタイル
			x,					// Ｘ
			y,					// Ｙ
			w,					// 幅
			h,					// 高さ
			nullptr,			// 親のウィンドウハンドル	
			nullptr,			// メニューハンドル
			hInstance,			// インスタンスハンドル
			nullptr				// WM_CREATEの引数
			);
		if (_hwnd == nullptr)
		{
			_isRunning = false;
			return 0;
		}
	}


	//	OpenGL初期化
	HGLRC hrc = Open_GL::InitializeOpenGL(_hwnd);
	if (hrc == nullptr) {
		_isRunning = false;
		return 0;
	}
	//	GLEW初期化
	if (Open_GL::InitializeGLEW() == false) {
		_isRunning = false;
		return 0;
	}


	//-----------------------------------------------------------------
	// システムの初期化
	//-----------------------------------------------------------------
	if (Init() == false)
	{
		_isRunning = false;
		return 0;
	}

	ShowWindow(_hwnd, SW_SHOW);
	UpdateWindow(_hwnd);

	//================================================================
	// メインメッセージループ
	//================================================================
	MSG		msg;
	ZeroMemory(&msg, sizeof(msg));

	do
	{
		if (_isExit) break;

		// メッセージの取得にGetMessage()ではなくPeekMessage()を使用。
		// GetMessage ()はメッセージがない場合にスリープ停止するのに対して
		// PeekMessage()は待たずに戻ってくる。ゲームなどのリアルタイム処理をする
		// アプリケーションはPeekMessage()のほうが好ましい
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);		// メッセージの変換
			DispatchMessage(&msg);		// メッセージの転送通知
		}
		else
		{
			//--- システムの更新
			UpdateFrame();
		}
	}
	while (msg.message != WM_QUIT);


	//-----------------------------------------------------------------
	// システムの解放
	//-----------------------------------------------------------------
	Clear();
	Open_GL::DestroyOpenGL(_hwnd, hrc);
	UnregisterClass(className, hInstance);
	return (s32)msg.wParam;
}
//-----------------------------------------------------------------------------
//	初期化
//	@retval true    初期化成功
//	@retval false   初期化失敗
//-----------------------------------------------------------------------------
bool SystemMain::Init()
{
	_pWork = new SystemWork();
	if ((_pWork == nullptr) || (_pWork->Init() == false))
	{
		return false;
	}

#ifndef _DEBUG
	//	カーソル非表示
	int count = 0;
	while (count >= 0)
	{
		count = ShowCursor(FALSE);
	}
#endif

	return true;
}

//-----------------------------------------------------------------------------
//	1フレーム更新
//	@retval なし
//-----------------------------------------------------------------------------
void SystemMain::UpdateFrame()
{
	//	Alt+F4終了
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

	//	更新処理
	_pWork->Update();

	//	描画処理
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderManager* pRenderer = Manager::Renderer();

		//	アルファ有効化
//		pRenderer->EnableAlphaBlending();

		pRenderer->DisableDepthTest();			//	DEPTH無効
		glMatrixMode(GL_PROJECTION);			//	プロジェクション設定
		glLoadIdentity();						//	単位行列
		LoadMatrix(Matrix::OrthoMatrix(0.0f, (f32)SCRN_WIDTH, (f32)SCRN_HEIGHT, 0.0f, 1.0f, -1.0f));

		glMatrixMode(GL_MODELVIEW);				//	モデルビュー設定
		glLoadIdentity();						//	単位行列
	}

	_pWork->Render();



	//---- 画面更新
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
//	解放
//	@retval なし
//-----------------------------------------------------------------------------
void SystemMain::Clear()
{
	_isRunning = false;
	_pWork->Exit();
	SafeDelete(_pWork);
}
//-----------------------------------------------------------------------------
//	ウィンドウプロシージャ
//	@param  [in]    hwnd	ウィンドウハンドル
//	@param  [in]    message	メッセージの種類
//	@param  [in]    wparam	パラメータ1
//	@param  [in]    lparam	パラメータ2
//	@return メッセージ処理の結果の値
//-----------------------------------------------------------------------------
LRESULT CALLBACK SystemMain::WndProc(HWND hWnd,	UINT message,WPARAM wparam,	LPARAM lparam)
{
	switch (message)
	{
	//-----------------------------------------------------------------
	case WM_CREATE:		// ウィンドウ作成直後
	//-----------------------------------------------------------------
	{
							CREATESTRUCT* p = (CREATESTRUCT*)lparam;
							UNUSED(p);
	}
		return 0;
	//-----------------------------------------------------------------
	case WM_PAINT:		// 再描画要求
	//-----------------------------------------------------------------
	{
							PAINTSTRUCT	ps;
							HDC			hdc = BeginPaint(hWnd, &ps);
							UNUSED(hdc);
							EndPaint(hWnd, &ps);
	}
		return 0;
	//-----------------------------------------------------------------
	case WM_DESTROY:	// ウィンドウを削除するとき
	//-----------------------------------------------------------------
	{
							PostQuitMessage(0);	// GetMessage()のときに 0 が返る
	}
		break;
	//-----------------------------------------------------------------
	} // switch( message )

	//---- デフォルトのウィンドウプロシージャ
	return DefWindowProc(hWnd, message, wparam, lparam);
}


//!	ウィンドウのアクティブ状態の取得
//!	@retval	true アクティブ : false 非アクティブ
bool IsWindowActive()
{
	SystemMain* pSystem = SystemMain::GetInstance();
	//	現在アクティブなウィンドウのハンドルを取得
	HWND activeHwnd = GetActiveWindow();
	HWND myHwnd = pSystem->GetHwnd();
	return (myHwnd == activeHwnd);
}



