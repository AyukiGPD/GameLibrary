//=============================================================================
//!
//!	@file	SceneManager.cpp
//!	@brief	�V�[���}�l�[�W���[
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
SceneManager::SceneManager()
:_pScene	(nullptr)
,_pNextScene(nullptr)
{
}
//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
SceneManager::~SceneManager()
{
}
//-----------------------------------------------------------------------------
//!	������
//-----------------------------------------------------------------------------
void SceneManager::Init()
{
	if(_pScene != nullptr)
	{
		_pScene->Init();
	}
}
//-----------------------------------------------------------------------------
//!	�X�V
//-----------------------------------------------------------------------------
void SceneManager::Update()
{
	//	���̃V�[���ւ̐؂�ւ�
	if(_pNextScene != nullptr)
	{
		if(_pScene != nullptr)
		{
			Manager::Particle()->Clear();
			_pScene->Exit();		//	�I��
			SafeDelete(_pScene);	//	���
		}
		_pScene = _pNextScene;	//	�؂�ւ�
		_pNextScene = nullptr;	//	���g�������
		if (_pScene)
		{
			_pScene->Init();		//	����������
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
//!	�`��
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
//!	�I��
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


