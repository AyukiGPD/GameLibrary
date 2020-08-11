//=============================================================================
//!
//!	@file	Particle.cpp
//!	@brief	�G�t�F�N�g�����N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

using namespace Math;

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
ParticleManager::ParticleManager()
{
}
//-----------------------------------------------------------------------------
//!	���
//-----------------------------------------------------------------------------
void ParticleManager::Clear()
{
	//	���X�g�폜
	auto itr = _pParticles.begin();
	for (; itr != _pParticles.end(); ++itr)
	{
		(*itr)->Clear();
		SafeRelease((*itr));
	}
	_pParticles.clear();
}

//-----------------------------------------------------------------------------
//! �X�V
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
		// �����Ǘ�
		//-----------------------------------------------------------
		p->_life -= deltaTime;
		if(p->_life <= 0.f)
		{
			p->Clear();
			DestroyObject(p);
			//---- ���ŏ���
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
//! �`��
//-----------------------------------------------------------------------------
void ParticleManager::Render()
{
}

