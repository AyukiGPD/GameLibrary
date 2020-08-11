//=============================================================================
//!
//!	@file	Particle.h
//!	@brief	�G�t�F�N�g�����N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//!	�p�[�e�B�N���x�[�X
class BaseParticle : public GameObjectBase
{
	friend class ParticleManager;
public:
	//!	�R���X�g���N�^
	BaseParticle()
	:_life(0.f)
	, _pCameraObject(nullptr)
	{}

	//!�f�X�g���N�^
	virtual ~BaseParticle(){}

	virtual void Clear(){}

	//!	�X�V
	virtual void Update(){}
	//!	�`��
	virtual void Render(){}
protected:
	f32				_life;				//!< ���� (�b)
	f32				_lifeMax;			//!< ���� (�b)
	class CameraObject*	_pCameraObject;	//!< �g���Ă���J�����̃|�C���^(Manager����n�����)

};

//	�I�u�W�F�N�g���\��
//!	�p�[�e�B�N���Ǘ��N���X
class ParticleManager : public ManagerBase
{
public:
	//! �R���X�g���N�^
	ParticleManager();
	
	//!	���
	void Clear();

	//! �o��
	template <typename T, typename... Args>
	bool Create(const Vector3& position, f32 life, Args&&... args)
	{
		//	�ő�o�^�\���܂Ŏg���؂��Ă����ꍇ�͓o�^�𒆒f
		if(_pParticles.size() >= PARTICLE_MAX)
		{
			return false;
		}
		auto pParticle = CreateGameObjectInit<T>(args...);
		pParticle->_life	= life;
		pParticle->_lifeMax = 1.f / life;

		pParticle->SetWorldPosition(position);

		_pParticles.push_back(pParticle.get());

		return true;
	}
	//! �o��
	template <typename T, typename... Args>
	bool Create(const Matrix& matrix, f32 life, Args&&... args)
	{
		//	�ő�o�^�\���܂Ŏg���؂��Ă����ꍇ�͓o�^�𒆒f
		if(_pParticles.size() >= PARTICLE_MAX)
		{
			return false;
		}
		auto pParticle = CreateGameObjectInit<T>(args...);
		pParticle->_life	= life;
		pParticle->_lifeMax = 1.f / life;

		pParticle->SetWorldMatrix(matrix);

		_pParticles.push_back(pParticle.get());

		return true;
	}
	//! �X�V
	void		Update();

	//! �`��
	void		Render();
private:
	//! �p�[�e�B�N���̓o�^�\�ő��
	static const int	PARTICLE_MAX = 10000;

	std::list<BaseParticle*> _pParticles;	//!< �p�[�e�B�N���̃|�C���^

};

MANAGER_INTERFACE(ParticleManager, Particle);


