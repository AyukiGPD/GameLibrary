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
//! @brief  物理オブジェクト
//-----------------------------------------------------------------------------
class CollisionObject : public GameObjectBase
{
public:
	friend RigidBody;

	//! @brief  コンストラクタ
	CollisionObject();

	//! @brief  デストラクタ
	virtual ~CollisionObject();

	//! @brief  物理設定
	//!	@param	[in]	desc	引数リスト
	//!	@param	[in]	tag		コンポーネント追加タグ	default:""
	bool SetRigidBody(const RigidBody::Desc& desc, const std::string& tag = "");

	//! @brief  物理設定
	//!	@param	[in]	desc	引数リスト
	//!	@param	[in]	myGroup	自身のグループビット
	//!	@param	[in]	filter	接触するグループビット
	//!	@param	[in]	tag		コンポーネント追加タグ	default:""
	bool SetRigidBody(const RigidBody::Desc& desc, u16 myGroup, u16 filter, const std::string& tag = "");

	//! @brief  接触関数
	//!	@param	[in]	contact	接触情報
	virtual void Contact(const ContactInfo& contact);

	//! @brief  物理コンポーネント取得
	//!	@return	物理コンポーネント
	RigidBody* GetRigidBody();

	//! @brief  反発
	//!	@param	[in]	restitution	反発係数
	void SetRestitution(f32 restitution);

	//! @brief  摩擦
	//!	@param	[in]	friction	摩擦係数
	void SetFriction(f32 friction);

	//! @brief  物理移動
	//!	@param	[in]	force	移動ベクトル
	void SetForce(const Vector3& force);

	//! @brief  物理移動
	//!	@param	[in]	force	移動ベクトル
	void AddForce(const Vector3& force);

	//! @brief  物理回転
	//!	@param	[in]	torque	物理回転
	void AddTorque(const Vector3& torque);

	//! @brief  重力
	//!	@param	[in]	gravity	重力ベクトル
	void SetGravity(const Vector3& gravity);

	//! @brief  ステート設定
	//!	@param	[in]	newState	ステート
	void SetActivationState(COLLISION_ACTIVATE newState);

	//! @brief  ステート設定(強制)
	//!	@param	[in]	newState	ステート
	void SetForceActivationState(COLLISION_ACTIVATE newState);

	//! @brief  コリジョン状態の設定
	//!	@param	[in]	flags	btCollisionObject::CF系
	void SetCollisionFlags(s32 flags);

	//! @brief  質量設定
	//!	@param	[in]	mass	質量
	void SetMass(f32 mass);

	//! @brief  反発
	//!	@return	反発係数
	f32 GetRestitution()const;

	//! @brief  摩擦
	//!	@return	摩擦係数
	f32 GetFriction()const;

	//! @brief  物理移動
	//!	@return	物理移動ベクトル
	Vector3 GetForce()const;

	//! @brief  物理回転
	//!	@return	物理回転方向
	Vector3 GetTorque()const;

	//! @brief  重力
	//!	@return	重力ベクトル
	Vector3 GetGravity()const;

	//! @brief  線形速度
	//!	@return	線形速度ベクトル
	Vector3 GetLinearVelocity()const;

	//! @brief  ステート取得
	//!	@return	ステート
	COLLISION_ACTIVATE GetActivationState()const;

	//! @brief  コリジョン状態の取得
	//!	@return	btCollisionObject::CF系
	s32 GetCollisionFlags()const;

	//! @brief  アクティブ化
	//!	@param	[in]	forceActivation	強制アクティブ	default : false
	void Activate(bool forceActivation = false) const;

	//! @brief  当たり判定
	//!	@retval	true	あたり
	bool IsHit()const;

	//! @brief  物理削除
	void ResetRigidBody();

protected:
	RefPointer<RigidBody> _pRigidBody;
};

