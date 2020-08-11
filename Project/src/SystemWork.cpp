//=============================================================================
//!
//!	@file	WorkSpace.cpp
//!	@brief	���[�N�X�y�[�X
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

std::list<ManagerBase*> ManagerBundle::_pManagers;

//!	�R���X�g���N�^
ManagerBundle::ManagerBundle()
{
	AddManager<KeyInputManager>();
	AddManager<XInputPadManager>();
	AddManager<SceneManager>();
	AddManager<CollisionManager>();
	AddManager<CameraManager>();
	AddManager<LightManager>();
	AddManager<ShaderManager>();
	AddManager<FontManager>();
	AddManager<RenderManager>();
	AddManager<ParticleManager>();
	AddManager<ModelManager>();
	AddManager<DebugManager>();
	AddManager<SoundManager>();

//	AddManager<SoundPlayer>();
}

//!	�f�X�g���N�^
ManagerBundle::~ManagerBundle()
{

	SingletonFinalizer::Finalize();
}

//void ManagerBundle::Update()
//{
//	for (auto* pManager : _pManagers)
//	{
//		pManager->Update();
//	}
//}
//
//void ManagerBundle::Render()
//{
//	for (auto* pManager : _pManagers)
//	{
//		pManager->Render();
//	}
//}


//!	�I��
void ManagerBundle::Exit()
{
	for(auto* pManager : _pManagers)
	{
		pManager->Exit();
	}
}


//-------------------------------------------------------------------
//!	������
//-------------------------------------------------------------------
bool SystemWork::Init()
{
	_pManager = new ManagerBundle;

	Manager::Scene()->SceneJump<SceneGame>();

#ifdef RELEASE_FINAL
	Manager::XInput()->Init(true);
#else
	Manager::XInput()->Init(false);
#endif
	Manager::Collision()->Init();

	Math::RandSeed((unsigned)time(nullptr));	//	�����_���V�[�h�l

	Manager::Renderer()->Init();
	Manager::Scene()->Init();

	Manager::Debug()->SetDebugFontHundler(Manager::Font()->Load("C:\\Windows\\Fonts\\msgothic.ttc", 16));

	Manager::Sound()->Init();	// �Z�b�g�A�b�v


	return true;
}
//-------------------------------------------------------------------
//!	�X�V
//-------------------------------------------------------------------
void SystemWork::Update()
{
	Manager::XInput()->Update();
	Manager::KeyInput()->Update();
//	Manager::MouseInput()->Update();

	Manager::Renderer()->Update();
	Manager::Debug()->Update();

	if(Manager::Debug()->IsUpdateActive())
	{
		Manager::Sound()->Update();
		Manager::Scene()->Update();
		Manager::Collision()->Update();
	}
}

//-------------------------------------------------------------------
//!	�`��
//-------------------------------------------------------------------
void SystemWork::Render()
{
	Manager::Scene()->Render();
	Manager::Debug()->Render();

}
//-------------------------------------------------------------------
//!	�I��
//-------------------------------------------------------------------
void SystemWork::Exit()
{
	_pManager->Exit();
	SafeDelete(_pManager);
}

