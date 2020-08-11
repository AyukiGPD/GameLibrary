//=============================================================================
//!
//!	@file	SceneManager.cpp
//!	@brief	シーンマネージャー
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
SceneManager::SceneManager()
:_pScene	(nullptr)
,_pNextScene(nullptr)
{
}
//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
SceneManager::~SceneManager()
{
}
//-----------------------------------------------------------------------------
//!	初期化
//-----------------------------------------------------------------------------
void SceneManager::Init()
{
	if(_pScene != nullptr)
	{
		_pScene->Init();
	}
}
//-----------------------------------------------------------------------------
//!	更新
//-----------------------------------------------------------------------------
void SceneManager::Update()
{
	//	次のシーンへの切り替え
	if(_pNextScene != nullptr)
	{
		if(_pScene != nullptr)
		{
			Manager::Particle()->Clear();
			_pScene->Exit();		//	終了
			SafeDelete(_pScene);	//	解放
		}
		_pScene = _pNextScene;	//	切り替え
		_pNextScene = nullptr;	//	中身を手放す
		if (_pScene)
		{
			_pScene->Init();		//	初期化処理
		}
		else
		{
			MY_ASSERT(false,"NotCreated SceneObject");
		}

	}

	if(_pScene != nullptr)
	{
		_pScene->Update();
		_pScene->ObjectUpdate();
		Manager::Particle()->Update();
	}
}
//-----------------------------------------------------------------------------
//!	描画
//-----------------------------------------------------------------------------
void SceneManager::Render()
{
	if(_pScene != nullptr)
	{
		CameraObject* pSceneCamera = _pScene->GetSceneCamera();
		if (!pSceneCamera)
		{
			DEBUG_LOG("SceneCamera Not Val");
			return;
		}

		pSceneCamera->Begin();
		{
			_pScene->Render();
			_pScene->ObjectRender();
			Manager::Particle()->Render();

			Manager::Collision()->Render();
		}
		pSceneCamera->End();
		Manager::Renderer()->EnableAlphaBlending();
		pSceneCamera->RenderBuffer();
		pSceneCamera->RenderHUD();
	}
}
//-----------------------------------------------------------------------------
//!	終了
//-----------------------------------------------------------------------------
void SceneManager::Exit()
{
	if(_pScene != nullptr)
	{
		_pScene->Exit();
	}
	SafeDelete(_pScene);
	SafeDelete(_pNextScene);

}


