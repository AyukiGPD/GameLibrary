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

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//-----------------------------------------------------------------------------
CollisionObject::CollisionObject()
{
}

//-----------------------------------------------------------------------------
//  @brief  �f�X�g���N�^
//-----------------------------------------------------------------------------
CollisionObject::~CollisionObject()
{
}

//-----------------------------------------------------------------------------
//  @brief  �����ݒ�
//  @param	[in]	desc	�������X�g
//  @param	[in]	tag		�R���|�[�l���g�ǉ��^�O	default:""
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
//  @brief  �����ݒ�
//  @param	[in]	desc	�������X�g
//  @param	[in]	myGroup	���g�̃O���[�v�r�b�g
//  @param	[in]	filter	�ڐG����O���[�v�r�b�g
//  @param	[in]	tag		�R���|�[�l���g�ǉ��^�O	default:""
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
//  @brief  �ڐG�֐�
//  @param	[in]	contact	�ڐG���
//-----------------------------------------------------------------------------
void CollisionObject::Contact(const ContactInfo& contact)
{
	UNUSED(contact);
}

//-----------------------------------------------------------------------------
//  @brief  �����R���|�[�l���g�擾
//  @return	�����R���|�[�l���g
//-----------------------------------------------------------------------------
RigidBody* CollisionObject::GetRigidBody()
{
	return _pRigidBody.get();
}

//-----------------------------------------------------------------------------
//  @brief  ����
//  @param	[in]	restitution	�����W��
//-----------------------------------------------------------------------------
void CollisionObject::SetRestitution(f32 restitution)
{
	if (!_pRigidBody)return;
	_pRigidBody->SetRestitution(restitution);
}

//-----------------------------------------------------------------------------
//  @brief  ���C
//  @param	[in]	friction	���C�W��
//-----------------------------------------------------------------------------
void CollisionObject::SetFriction(f32 friction)
{
	if (!_pRigidBody)return;
	_pRigidBody->SetFriction(friction);
}

//-----------------------------------------------------------------------------
//  @brief  �����ړ�
//  @param	[in]	force	�ړ��x�N�g��
//-----------------------------------------------------------------------------
void CollisionObject::SetForce(const Vector3& force)
{
	if (!_pRigidBody)return;
	_pRigidBody->SetForce(force);
}

//-----------------------------------------------------------------------------
//  @brief  �����ړ�
//  @param	[in]	force	�ړ��x�N�g��
//-----------------------------------------------------------------------------
void CollisionObject::AddForce(const Vector3& force)
{
	if (!_pRigidBody)return;
	_pRigidBody->AddForce(force);
}

//-----------------------------------------------------------------------------
//  @brief  ������]
//  @param	[in]	torque	������]
//-----------------------------------------------------------------------------
void CollisionObject::AddTorque(const Vector3& torque)
{
	if (!_pRigidBody)return;
	_pRigidBody->AddTorque(torque);
}

//-----------------------------------------------------------------------------
//  @brief  �d��
//  @param	[in]	gravity	�d�̓x�N�g��
//-----------------------------------------------------------------------------
void CollisionObject::SetGravity(const Vector3& gravity)
{
	if (!_pRigidBody)return;
	_pRigidBody->SetGravity(gravity);
}

//-----------------------------------------------------------------------------
//  @brief  �X�e�[�g�ݒ�
//  @param	[in]	newState	�X�e�[�g
//-----------------------------------------------------------------------------
void CollisionObject::SetActivationState(COLLISION_ACTIVATE newState)
{
	if (!_pRigidBody)return;
	_pRigidBody->SetActivationState(newState);
}

//-----------------------------------------------------------------------------
//  @brief  �X�e�[�g�ݒ�(����)
//  @param	[in]	newState	�X�e�[�g
//-----------------------------------------------------------------------------
void CollisionObject::SetForceActivationState(COLLISION_ACTIVATE newState)
{
	if (!_pRigidBody)return;
	_pRigidBody->SetForceActivationState(newState);
}

//-----------------------------------------------------------------------------
//  @brief  �R���W������Ԃ̐ݒ�
//  @param	[in]	flags	btCollisionObject::CF�n
//-----------------------------------------------------------------------------
void CollisionObject::SetCollisionFlags(s32 flags)
{
	if (!_pRigidBody)return;
	_pRigidBody->SetCollisionFlags(flags);
}

//-----------------------------------------------------------------------------
//  @brief  ���ʐݒ�
//  @param	[in]	mass	����
//-----------------------------------------------------------------------------
void CollisionObject::SetMass(f32 mass)
{
	if (!_pRigidBody)return;
	_pRigidBody->SetMass(mass);
}

//-----------------------------------------------------------------------------
//  @brief  ����
//  @return	�����W��
//-----------------------------------------------------------------------------
f32 CollisionObject::GetRestitution() const
{
	if (!_pRigidBody)return 0.f;
	return _pRigidBody->GetRestitution();
}

//-----------------------------------------------------------------------------
//  @brief  ���C
//  @return	���C�W��
//-----------------------------------------------------------------------------
f32 CollisionObject::GetFriction() const
{
	if (!_pRigidBody)return 0.f;
	return _pRigidBody->GetFriction();
}

//-----------------------------------------------------------------------------
//  @brief  �����ړ�
//  @return	�����ړ��x�N�g��
//-----------------------------------------------------------------------------
Vector3 CollisionObject::GetForce() const
{
	if (!_pRigidBody)return Vector3::ZERO;
	return _pRigidBody->GetForce();
}

//-----------------------------------------------------------------------------
//  @brief  ������]
//  @return	������]����
//-----------------------------------------------------------------------------
Vector3 CollisionObject::GetTorque() const
{
	if (!_pRigidBody)return Vector3::ZERO;
	return _pRigidBody->GetTorque();
}

//-----------------------------------------------------------------------------
//  @brief  �d��
//  @return	�d�̓x�N�g��
//-----------------------------------------------------------------------------
Vector3 CollisionObject::GetGravity() const
{
	if (!_pRigidBody)return Vector3::ZERO;
	return _pRigidBody->GetGravity();
}

//-----------------------------------------------------------------------------
//  @brief  ���`���x
//  @return	���`���x�x�N�g��
//-----------------------------------------------------------------------------
Vector3 CollisionObject::GetLinearVelocity() const
{
	if (!_pRigidBody)return Vector3::ZERO;
	return _pRigidBody->GetLinearVelocity();
}

//-----------------------------------------------------------------------------
//  @brief  �X�e�[�g�擾
//  @return	�X�e�[�g
//-----------------------------------------------------------------------------
COLLISION_ACTIVATE CollisionObject::GetActivationState() const
{
	if (!_pRigidBody)return COLLISION_ACTIVATE::STATE_EMPTY;
	return _pRigidBody->GetActivationState();
}

//-----------------------------------------------------------------------------
//  @brief  �R���W������Ԃ̎擾
//  @return	btCollisionObject::CF�n
//-----------------------------------------------------------------------------
s32 CollisionObject::GetCollisionFlags() const
{
	if (!_pRigidBody)return 0;
	return _pRigidBody->GetCollisionFlags();
}

//-----------------------------------------------------------------------------
//  @brief  �A�N�e�B�u��
//  @param	[in]	forceActivation	�����A�N�e�B�u	default : false
//-----------------------------------------------------------------------------
void CollisionObject::Activate(bool forceActivation) const
{
	if (!_pRigidBody)return;
	_pRigidBody->Activate(forceActivation);
}


//-----------------------------------------------------------------------------
//  @brief  �����蔻��
//  @retval	true	������
//-----------------------------------------------------------------------------
bool CollisionObject::IsHit() const
{
	return _pRigidBody ? _pRigidBody->IsContact() : false;
}

//-----------------------------------------------------------------------------
//  @brief  �����폜
//-----------------------------------------------------------------------------
void CollisionObject::ResetRigidBody()
{
	ReleaseProcess(_pRigidBody.get());
	_pRigidBody.reset();
}
