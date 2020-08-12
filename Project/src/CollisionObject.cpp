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

//=============================================================================
//!	物理オブジェクト
//=============================================================================
//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
CollisionObject::CollisionObject()
{
}

//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
CollisionObject::~CollisionObject()
{
}

//-----------------------------------------------------------------------------
//!	物理設定
//!	@param	[in]	desc	引数リスト
//!	@param	[in]	tag		コンポーネント追加タグ	default:""
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
//!	物理設定
//!	@param	[in]	desc	引数リスト
//!	@param	[in]	myGroup	自身のグループビット
//!	@param	[in]	filter	接触するグループビット
//!	@param	[in]	tag		コンポーネント追加タグ	default:""
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
//!	物理削除
//-----------------------------------------------------------------------------
void CollisionObject::ResetRigidBody()
{
	ReleaseProcess(_pRigidBody.get());
	_pRigidBody.reset();
}
