//=============================================================================
//!
//!	@file	SceneGame.cpp
//!	@brief	�Q�[���V�[��
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//using namespace Game;
//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
SceneGame::SceneGame()
: _isPause(false)
{
}
//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
SceneGame::~SceneGame()
{
}
//-----------------------------------------------------------------------------
//!	������
//-----------------------------------------------------------------------------
void SceneGame::Init()
{
	// TODO ��������������������������
	// TODO �Q�[�����ɐ�������������
	//	�J���������ݒ�
	{
		CALL_MANAGER(Manager::Camera(), pCamera);
		CameraObject::Desc desc;
		desc.LookAt(Vector3(0, -100, 300), Vector3(0, 0, 0), Vector3(0, 1, 0));
		desc.Perspective(45.0f, SCRN_WIDTH, SCRN_HEIGHT, 0.1f, 10000.0f);
		_pCamera = CreateGameObject<CameraObject>();

		if (_pCamera->Set<PhysicalBasedRendering>(desc))
		{
			_pCamera->InitHUDObject();	//	HUD�I�u�W�F�N�g��Init
			pCamera->ChangeCamera(_pCamera.get());
			SetSceneCamera(_pCamera.get());

			//	�g�[���}�b�s���O�̃t�B���^�̐ݒ�
			auto* pFilter = static_cast<PhysicalBasedRendering*>(_pCamera->GetDeferredRenderer())->GeToneMappingFilter();
			pFilter->SetExposureMin(3.0f);
		}
	}

	//	���s����
	{
		LightObject::Desc desc(Vector3(1, 1, 1), Color(255), LIGHT_MODE::DIRECTIONAL, 10.0f);
		_pDirectionalLight = CreateGameObject<LightObject>(desc);
	}

	//	���̑��_����
	for(auto point : { Vector3(100.0f, 200.0f, 50.0f), Vector3(-100.0f, 200.0f, 50.0f) })
	{
		LightObject::Desc desc(point, Color::WHITE, LIGHT_MODE::POINT, 15.0f);
		auto* pLight = CreateGameObject<LightObject>(desc).get();
		UNUSED(pLight);
	}

	auto* pCollision = Manager::Collision();
#ifndef RELEASE_FINAL
	pCollision->SetDebugRender(true);		//	�f�o�b�O�`��
#else
	pCollision->SetDebugRender(false);		//	�f�o�b�O�`��
#endif

//	//	�Q�[���Ǘ��N���X
//	_pGameManager = CreateGameObjectInit<GameManager>();
//
	//	�t�H���g�ǂݍ���
	_font = Manager::Font()->Load("assets/JF-Dot-Shinonome16.ttf");

//	CALL_MANAGER(Manager::Sound(), pSoundManager);
//
//	//	�T�E���h�̃��X�i�[�ݒ�
//	Matrix mat = _pCamera->GetWorldMatrix();
//	pSoundManager->SetListener(mat);
//
//	//	BGM�ǂݍ���
//	_pBgmSounds[0] = CreateGameObjectInit<SoundObject>("assets/Backstreet.wav", true, false);
//	_pBgmSounds[1] = CreateGameObjectInit<SoundObject>("assets/DRIVE_BEAT.wav", true, false);
//	_pBgmSounds[2] = CreateGameObjectInit<SoundObject>("assets/GrenadeDiving.wav", true, false);
//	
//	//	�{�����[���ݒ�
//	_pBgmSounds[0]->Volume(0.5f);
//	_pBgmSounds[1]->Volume(0.5f);
//	_pBgmSounds[2]->Volume(0.5f);
}
//-----------------------------------------------------------------------------
//!	�X�V
//-----------------------------------------------------------------------------
void SceneGame::Update()
{
}

//-----------------------------------------------------------------------------
//!	�`��
//-----------------------------------------------------------------------------
void SceneGame::Render()
{
}
//-----------------------------------------------------------------------------
//!	�I��
//-----------------------------------------------------------------------------
void SceneGame::Exit()
{
}





