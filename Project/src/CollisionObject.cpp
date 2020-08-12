//=============================================================================
//!
//!	@file	CollisionObject.cpp
//!	@brief	物理オブジェクト
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

/*
	newのオーバーライドがbulletで被るので
	ここではnewのオーバーライドを切る
*/
#undef new

//-----------------------------------------------------------------------------
//  @brief  コンストラクタ
//-----------------------------------------------------------------------------
CollisionObject::CollisionObject()
{
}

//-----------------------------------------------------------------------------
//  @brief  デストラクタ
//-----------------------------------------------------------------------------
CollisionObject::~CollisionObject()
{
}

//-----------------------------------------------------------------------------
//  @brief  物理設定
//  @param	[in]	desc	引数リスト
//  @param	[in]	tag		コンポーネント追加タグ	default:""
//-----------------------------------------------------------------------------
bool CollisionObject::SetRigidBody(const RigidBody::Desc& desc, const std::string& tag)
{
	if(_pRigidBody) return true;
	_pRigidBody = CreateRefPointer<RigidBody>();
	AddProcess(_pRigidBody.get());
	if(!_pRigidBody->Init(desc))
	{
		ReleaseProcess(_pRigidBody.get());
		return false;
	}
	_pRigidBody->AddTag(tag);
	return true;
}

//-----------------------------------------------------------------------------
//  @brief  物理設定
//  @param	[in]	desc	引数リスト
//  @param	[in]	myGroup	自身のグループビット
//  @param	[in]	filter	接触するグループビット
//  @param	[in]	tag		コンポーネント追加タグ	default:""
//-----------------------------------------------------------------------------
bool CollisionObject::SetRigidBody(const RigidBody::Desc& desc, u16 myGroup, u16 filter, const std::string& tag)
{
	if(_pRigidBody) return true;
	_pRigidBody = CreateRefPointer<RigidBody>();
	AddProcess(_pRigidBody.get());
	if(!_pRigidBody->Init(desc, myGroup, filter))
	{
		ReleaseProcess(_pRigidBody.get());
		return false;
	}
	_pRigidBody->AddTag(tag);
	return true;
}

//-----------------------------------------------------------------------------
//  @brief  接触関数
//  @param	[in]	contact	接触情報
//-----------------------------------------------------------------------------
void CollisionObject::Contact(const ContactInfo& contact)
{
	UNUSED(contact);
}

//-----------------------------------------------------------------------------
//  @brief  物理コンポーネント取得
//  @return	物理コンポーネント
//-----------------------------------------------------------------------------
RigidBody* CollisionObject::GetRigidBody()
{
	return _pRigidBody.get();
}

//-----------------------------------------------------------------------------
//  @brief  反発
//  @param	[in]	restitution	反発係数
//-----------------------------------------------------------------------------
void CollisionObject::SetRestitution(f32 restitution)
{
	if (!_pRigidBody)return;
	_pRigidBody->SetRestitution(restitution);
}

//-----------------------------------------------------------------------------
//  @brief  摩擦
//  @param	[in]	friction	摩擦係数
//-----------------------------------------------------------------------------
void CollisionObject::SetFriction(f32 friction)
{
	if (!_pRigidBody)return;
	_pRigidBody->SetFriction(friction);
}

//-----------------------------------------------------------------------------
//  @brief  物理移動
//  @param	[in]	force	移動ベクトル
//-----------------------------------------------------------------------------
void CollisionObject::SetForce(const Vector3& force)
{
	if (!_pRigidBody)return;
	_pRigidBody->SetForce(force);
}

//-----------------------------------------------------------------------------
//  @brief  物理移動
//  @param	[in]	force	移動ベクトル
//-----------------------------------------------------------------------------
void CollisionObject::AddForce(const Vector3& force)
{
	if (!_pRigidBody)return;
	_pRigidBody->AddForce(force);
}

//-----------------------------------------------------------------------------
//  @brief  物理回転
//  @param	[in]	torque	物理回転
//-----------------------------------------------------------------------------
void CollisionObject::AddTorque(const Vector3& torque)
{
	if (!_pRigidBody)return;
	_pRigidBody->AddTorque(torque);
}

//-----------------------------------------------------------------------------
//  @brief  重力
//  @param	[in]	gravity	重力ベクトル
//-----------------------------------------------------------------------------
void CollisionObject::SetGravity(const Vector3& gravity)
{
	if (!_pRigidBody)return;
	_pRigidBody->SetGravity(gravity);
}

//-----------------------------------------------------------------------------
//  @brief  ステート設定
//  @param	[in]	newState	ステート
//-----------------------------------------------------------------------------
void CollisionObject::SetActivationState(COLLISION_ACTIVATE newState)
{
	if (!_pRigidBody)return;
	_pRigidBody->SetActivationState(newState);
}

//-----------------------------------------------------------------------------
//  @brief  ステート設定(強制)
//  @param	[in]	newState	ステート
//-----------------------------------------------------------------------------
void CollisionObject::SetForceActivationState(COLLISION_ACTIVATE newState)
{
	if (!_pRigidBody)return;
	_pRigidBody->SetForceActivationState(newState);
}

//-----------------------------------------------------------------------------
//  @brief  コリジョン状態の設定
//  @param	[in]	flags	btCollisionObject::CF系
//-----------------------------------------------------------------------------
void CollisionObject::SetCollisionFlags(s32 flags)
{
	if (!_pRigidBody)return;
	_pRigidBody->SetCollisionFlags(flags);
}

//-----------------------------------------------------------------------------
//  @brief  質量設定
//  @param	[in]	mass	質量
//-----------------------------------------------------------------------------
void CollisionObject::SetMass(f32 mass)
{
	if (!_pRigidBody)return;
	_pRigidBody->SetMass(mass);
}

//-----------------------------------------------------------------------------
//  @brief  反発
//  @return	反発係数
//-----------------------------------------------------------------------------
f32 CollisionObject::GetRestitution() const
{
	if (!_pRigidBody)return 0.f;
	return _pRigidBody->GetRestitution();
}

//-----------------------------------------------------------------------------
//  @brief  摩擦
//  @return	摩擦係数
//-----------------------------------------------------------------------------
f32 CollisionObject::GetFriction() const
{
	if (!_pRigidBody)return 0.f;
	return _pRigidBody->GetFriction();
}

//-----------------------------------------------------------------------------
//  @brief  物理移動
//  @return	物理移動ベクトル
//-----------------------------------------------------------------------------
Vector3 CollisionObject::GetForce() const
{
	if (!_pRigidBody)return Vector3::ZERO;
	return _pRigidBody->GetForce();
}

//-----------------------------------------------------------------------------
//  @brief  物理回転
//  @return	物理回転方向
//-----------------------------------------------------------------------------
Vector3 CollisionObject::GetTorque() const
{
	if (!_pRigidBody)return Vector3::ZERO;
	return _pRigidBody->GetTorque();
}

//-----------------------------------------------------------------------------
//  @brief  重力
//  @return	重力ベクトル
//-----------------------------------------------------------------------------
Vector3 CollisionObject::GetGravity() const
{
	if (!_pRigidBody)return Vector3::ZERO;
	return _pRigidBody->GetGravity();
}

//-----------------------------------------------------------------------------
//  @brief  線形速度
//  @return	線形速度ベクトル
//-----------------------------------------------------------------------------
Vector3 CollisionObject::GetLinearVelocity() const
{
	if (!_pRigidBody)return Vector3::ZERO;
	return _pRigidBody->GetLinearVelocity();
}

//-----------------------------------------------------------------------------
//  @brief  ステート取得
//  @return	ステート
//-----------------------------------------------------------------------------
COLLISION_ACTIVATE CollisionObject::GetActivationState() const
{
	if (!_pRigidBody)return COLLISION_ACTIVATE::STATE_EMPTY;
	return _pRigidBody->GetActivationState();
}

//-----------------------------------------------------------------------------
//  @brief  コリジョン状態の取得
//  @return	btCollisionObject::CF系
//-----------------------------------------------------------------------------
s32 CollisionObject::GetCollisionFlags() const
{
	if (!_pRigidBody)return 0;
	return _pRigidBody->GetCollisionFlags();
}

//-----------------------------------------------------------------------------
//  @brief  アクティブ化
//  @param	[in]	forceActivation	強制アクティブ	default : false
//-----------------------------------------------------------------------------
void CollisionObject::Activate(bool forceActivation) const
{
	if (!_pRigidBody)return;
	_pRigidBody->Activate(forceActivation);
}


//-----------------------------------------------------------------------------
//  @brief  当たり判定
//  @retval	true	あたり
//-----------------------------------------------------------------------------
bool CollisionObject::IsHit() const
{
	return _pRigidBody ? _pRigidBody->IsContact() : false;
}

//-----------------------------------------------------------------------------
//  @brief  物理削除
//-----------------------------------------------------------------------------
void CollisionObject::ResetRigidBody()
{
	ReleaseProcess(_pRigidBody.get());
	_pRigidBody.reset();
}
