//=============================================================================
//!
//!	@file	Particle.cpp
//!	@brief	エフェクト生成クラス
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

using namespace Math;

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
ParticleManager::ParticleManager()
{
}
//-----------------------------------------------------------------------------
//!	解放
//-----------------------------------------------------------------------------
void ParticleManager::Clear()
{
	//	リスト削除
	auto itr = _pParticles.begin();
	for (; itr != _pParticles.end(); ++itr)
	{
		(*itr)->Clear();
		SafeRelease((*itr));
	}
	_pParticles.clear();
}

//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void ParticleManager::Update()
{
	auto* pCamera = Manager::Camera();
	assert(pCamera);
	auto* pCameraObject = pCamera->GetRunningCamera();
	assert(pCameraObject);
	f32 deltaTime = GetDeltaTime();
	auto itr = _pParticles.begin();
	for (; itr != _pParticles.end(); ++itr)
	{
		BaseParticle*	p = (*itr);
		//-----------------------------------------------------------
		// 寿命管理
		//-----------------------------------------------------------
		p->_life -= deltaTime;
		if(p->_life <= 0.f)
		{
			p->Clear();
			DestroyObject(p);
			//---- 消滅処理
			auto erase = itr;
			++itr;
			_pParticles.erase(erase);

			if (itr == _pParticles.end())return;
			p = (*itr);
		}
		p->_pCameraObject = pCameraObject;
		p->Update();
	}
}

//-----------------------------------------------------------------------------
//! 描画
//-----------------------------------------------------------------------------
void ParticleManager::Render()
{
}

