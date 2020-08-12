//=============================================================================
//!
//!	@file	CollisionObject.cpp
//!	@brief	�����I�u�W�F�N�g
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

/*
	new�̃I�[�o�[���C�h��bullet�Ŕ��̂�
	�����ł�new�̃I�[�o�[���C�h��؂�
*/
#undef new

//=============================================================================
//!	�����I�u�W�F�N�g
//=============================================================================
//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
CollisionObject::CollisionObject()
{
}

//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
CollisionObject::~CollisionObject()
{
}

//-----------------------------------------------------------------------------
//!	�����ݒ�
//!	@param	[in]	desc	�������X�g
//!	@param	[in]	tag		�R���|�[�l���g�ǉ��^�O	default:""
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
//!	�����ݒ�
//!	@param	[in]	desc	�������X�g
//!	@param	[in]	myGroup	���g�̃O���[�v�r�b�g
//!	@param	[in]	filter	�ڐG����O���[�v�r�b�g
//!	@param	[in]	tag		�R���|�[�l���g�ǉ��^�O	default:""
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
//!	�����폜
//-----------------------------------------------------------------------------
void CollisionObject::ResetRigidBody()
{
	ReleaseProcess(_pRigidBody.get());
	_pRigidBody.reset();
}
