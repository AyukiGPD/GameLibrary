//=============================================================================
//!
//!	@file	SceneGame.cpp
//!	@brief	ゲームシーン
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//using namespace Game;
//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
SceneGame::SceneGame()
: _isPause(false)
{
}
//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
SceneGame::~SceneGame()
{
}
//-----------------------------------------------------------------------------
//!	初期化
//-----------------------------------------------------------------------------
void SceneGame::Init()
{
	// TODO 生成処理をもう少し整理する
	// TODO ゲーム側に生成処理を入れる
	//	カメラ初期設定
	{
		CALL_MANAGER(Manager::Camera(), pCamera);
		CameraObject::Desc desc;
		desc.LookAt(Vector3(0, -100, 300), Vector3(0, 0, 0), Vector3(0, 1, 0));
		desc.Perspective(45.0f, SCRN_WIDTH, SCRN_HEIGHT, 0.1f, 10000.0f);
		_pCamera = CreateGameObject<CameraObject>();

		if (_pCamera->Set<PhysicalBasedRendering>(desc))
		{
			_pCamera->InitHUDObject();	//	HUDオブジェクトのInit
			pCamera->ChangeCamera(_pCamera.get());
			SetSceneCamera(_pCamera.get());

			//	トーンマッピングのフィルタの設定
			auto* pFilter = static_cast<PhysicalBasedRendering*>(_pCamera->GetDeferredRenderer())->GeToneMappingFilter();
			pFilter->SetExposureMin(3.0f);
		}
	}

	//	平行光源
	{
		LightObject::Desc desc(Vector3(1, 1, 1), Color(255), LIGHT_MODE::DIRECTIONAL, 10.0f);
		_pDirectionalLight = CreateGameObject<LightObject>(desc);
	}

	//	その他点光源
	for(auto point : { Vector3(100.0f, 200.0f, 50.0f), Vector3(-100.0f, 200.0f, 50.0f) })
	{
		LightObject::Desc desc(point, Color::WHITE, LIGHT_MODE::POINT, 15.0f);
		auto* pLight = CreateGameObject<LightObject>(desc).get();
		UNUSED(pLight);
	}

	auto* pCollision = Manager::Collision();
#ifndef RELEASE_FINAL
	pCollision->SetDebugRender(true);		//	デバッグ描画
#else
	pCollision->SetDebugRender(false);		//	デバッグ描画
#endif

//	//	ゲーム管理クラス
//	_pGameManager = CreateGameObjectInit<GameManager>();
//
	//	フォント読み込み
	_font = Manager::Font()->Load("assets/JF-Dot-Shinonome16.ttf");

//	CALL_MANAGER(Manager::Sound(), pSoundManager);
//
//	//	サウンドのリスナー設定
//	Matrix mat = _pCamera->GetWorldMatrix();
//	pSoundManager->SetListener(mat);
//
//	//	BGM読み込み
//	_pBgmSounds[0] = CreateGameObjectInit<SoundObject>("assets/Backstreet.wav", true, false);
//	_pBgmSounds[1] = CreateGameObjectInit<SoundObject>("assets/DRIVE_BEAT.wav", true, false);
//	_pBgmSounds[2] = CreateGameObjectInit<SoundObject>("assets/GrenadeDiving.wav", true, false);
//	
//	//	ボリューム設定
//	_pBgmSounds[0]->Volume(0.5f);
//	_pBgmSounds[1]->Volume(0.5f);
//	_pBgmSounds[2]->Volume(0.5f);
}
//-----------------------------------------------------------------------------
//!	更新
//-----------------------------------------------------------------------------
void SceneGame::Update()
{
}

//-----------------------------------------------------------------------------
//!	描画
//-----------------------------------------------------------------------------
void SceneGame::Render()
{
}
//-----------------------------------------------------------------------------
//!	終了
//-----------------------------------------------------------------------------
void SceneGame::Exit()
{
}





