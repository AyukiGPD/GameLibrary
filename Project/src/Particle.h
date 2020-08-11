//=============================================================================
//!
//!	@file	Particle.h
//!	@brief	エフェクト生成クラス
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//!	パーティクルベース
class BaseParticle : public GameObjectBase
{
	friend class ParticleManager;
public:
	//!	コンストラクタ
	BaseParticle()
	:_life(0.f)
	, _pCameraObject(nullptr)
	{}

	//!デストラクタ
	virtual ~BaseParticle(){}

	virtual void Clear(){}

	//!	更新
	virtual void Update(){}
	//!	描画
	virtual void Render(){}
protected:
	f32				_life;				//!< 寿命 (秒)
	f32				_lifeMax;			//!< 寿命 (秒)
	class CameraObject*	_pCameraObject;	//!< 使われているカメラのポインタ(Managerから渡される)

};

//	オブジェクト化予定
//!	パーティクル管理クラス
class ParticleManager : public ManagerBase
{
public:
	//! コンストラクタ
	ParticleManager();
	
	//!	解放
	void Clear();

	//! 出現
	template <typename T, typename... Args>
	bool Create(const Vector3& position, f32 life, Args&&... args)
	{
		//	最大登録可能個数まで使い切っていた場合は登録を中断
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
	//! 出現
	template <typename T, typename... Args>
	bool Create(const Matrix& matrix, f32 life, Args&&... args)
	{
		//	最大登録可能個数まで使い切っていた場合は登録を中断
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
	//! 更新
	void		Update();

	//! 描画
	void		Render();
private:
	//! パーティクルの登録可能最大個数
	static const int	PARTICLE_MAX = 10000;

	std::list<BaseParticle*> _pParticles;	//!< パーティクルのポインタ

};

MANAGER_INTERFACE(ParticleManager, Particle);


