//=============================================================================
//!
//!	@file	CollisionObject.h
//!	@brief	物理オブジェクト
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//-----------------------------------------------------------------------------
//!	物理オブジェクト
//-----------------------------------------------------------------------------
class CollisionObject : public GameObjectBase
{
public:
	friend RigidBody;
	//!	コンストラクタ
	CollisionObject();
	//!	デストラクタ
	virtual ~CollisionObject();

	//!	物理設定
	//!	@param	[in]	desc	引数リスト
	//!	@param	[in]	tag		コンポーネント追加タグ	default:""
	bool SetRigidBody(const RigidBody::Desc& desc, const std::string& tag = "");
	//!	物理設定
	//!	@param	[in]	desc	引数リスト
	//!	@param	[in]	myGroup	自身のグループビット
	//!	@param	[in]	filter	接触するグループビット
	//!	@param	[in]	tag		コンポーネント追加タグ	default:""
	bool SetRigidBody(const RigidBody::Desc& desc, u16 myGroup, u16 filter, const std::string& tag = "");

	//!	接触関数
	//!	@param	[in]	contact	接触情報
	virtual void Contact(const ContactInfo& contact){ UNUSED(contact); }

	//!	物理コンポーネント取得
	//!	@return	物理コンポーネント
	RigidBody* GetRigidBody()
	{
		return _pRigidBody.get();
	}

	//!	反発
	//!	@param	[in]	restitution	反発係数
	void SetRestitution(f32 restitution)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetRestitution(restitution);
	}

	//!	摩擦
	//!	@param	[in]	friction	摩擦係数
	void SetFriction(f32 friction)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetFriction(friction);
	}
	//!	物理移動
	//!	@param	[in]	force	移動ベクトル
	void SetForce(const Vector3& force)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetForce(force);
	}
	//!	物理移動
	//!	@param	[in]	force	移動ベクトル
	void AddForce(const Vector3& force)
	{
		if (!_pRigidBody)return;
		_pRigidBody->AddForce(force);
	}
	//!	物理回転
	//!	@param	[in]	torque	物理回転
	void AddTorque(const Vector3& torque)
	{
		if (!_pRigidBody)return;
		_pRigidBody->AddTorque(torque);
	}
	//!	重力
	//!	@param	[in]	gravity	重力ベクトル
	void SetGravity(const Vector3& gravity)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetGravity(gravity);
	}

	//!	ステート設定
	//!	@param	[in]	newState	ステート
	void SetActivationState(COLLISION_ACTIVATE newState)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetActivationState(newState);
	}

	//!	ステート設定(強制)
	//!	@param	[in]	newState	ステート
	void SetForceActivationState(COLLISION_ACTIVATE newState)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetForceActivationState(newState);
	}

	//!	コリジョン状態の設定
	//!	@param	[in]	flags	btCollisionObject::CF系
	void SetCollisionFlags(s32 flags)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetCollisionFlags(flags);
	}

	void SetMass(f32 mass)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetMass(mass);
	}

	//!	反発
	//!	@return	反発係数
	f32 GetRestitution()const
	{
		if (!_pRigidBody)return 0.f;
		return _pRigidBody->GetRestitution();
	}
	//!	摩擦
	//!	@return	摩擦係数
	f32 GetFriction()const
	{
		if (!_pRigidBody)return 0.f;
		return _pRigidBody->GetFriction();
	}
	//!	物理移動
	//!	@return	物理移動ベクトル
	Vector3 GetForce()const
	{
		if (!_pRigidBody)return Vector3::ZERO;
		return _pRigidBody->GetForce();
	}
	//!	物理回転
	//!	@return	物理回転方向
	Vector3 GetTorque()const
	{
		if (!_pRigidBody)return Vector3::ZERO;
		return _pRigidBody->GetTorque();
	}
	//!	重力
	//!	@return	重力ベクトル
	Vector3 GetGravity()const
	{
		if (!_pRigidBody)return Vector3::ZERO;
		return _pRigidBody->GetGravity();
	}

	Vector3 GetLinearVelocity()const
	{
		if (!_pRigidBody)return Vector3::ZERO;
		return _pRigidBody->GetLinearVelocity();
	}

	//!	ステート取得
	//!	@return	ステート
	COLLISION_ACTIVATE GetActivationState()const
	{
		if (!_pRigidBody)return COLLISION_ACTIVATE::STATE_EMPTY;
		_pRigidBody->GetActivationState();
	}
	//!	コリジョン状態の取得
	//!	@return	btCollisionObject::CF系
	s32 GetCollisionFlags()const
	{
		if (!_pRigidBody)return 0;
		return _pRigidBody->GetCollisionFlags();
	}


	//!	アクティブ化
	//!	@param	[in]	forceActivation	強制アクティブ	default : false
	void Activate(bool forceActivation = false) const
	{
		if (!_pRigidBody)return;
		_pRigidBody->Activate(forceActivation);
	}

	//!	当たり判定
	//!	@retval	true	あたり
	bool IsHit()const
	{
		return _pRigidBody ? _pRigidBody->IsContact() : false;
	}

	//!	物理削除
	void ResetRigidBody();

protected:
	RefPointer<RigidBody> _pRigidBody;
};

