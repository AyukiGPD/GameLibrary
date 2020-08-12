//=============================================================================
//!
//!	@file	CollisionObject.h
//!	@brief	�����I�u�W�F�N�g
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//-----------------------------------------------------------------------------
//!	�����I�u�W�F�N�g
//-----------------------------------------------------------------------------
class CollisionObject : public GameObjectBase
{
public:
	friend RigidBody;
	//!	�R���X�g���N�^
	CollisionObject();
	//!	�f�X�g���N�^
	virtual ~CollisionObject();

	//!	�����ݒ�
	//!	@param	[in]	desc	�������X�g
	//!	@param	[in]	tag		�R���|�[�l���g�ǉ��^�O	default:""
	bool SetRigidBody(const RigidBody::Desc& desc, const std::string& tag = "");
	//!	�����ݒ�
	//!	@param	[in]	desc	�������X�g
	//!	@param	[in]	myGroup	���g�̃O���[�v�r�b�g
	//!	@param	[in]	filter	�ڐG����O���[�v�r�b�g
	//!	@param	[in]	tag		�R���|�[�l���g�ǉ��^�O	default:""
	bool SetRigidBody(const RigidBody::Desc& desc, u16 myGroup, u16 filter, const std::string& tag = "");

	//!	�ڐG�֐�
	//!	@param	[in]	contact	�ڐG���
	virtual void Contact(const ContactInfo& contact){ UNUSED(contact); }

	//!	�����R���|�[�l���g�擾
	//!	@return	�����R���|�[�l���g
	RigidBody* GetRigidBody()
	{
		return _pRigidBody.get();
	}

	//!	����
	//!	@param	[in]	restitution	�����W��
	void SetRestitution(f32 restitution)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetRestitution(restitution);
	}

	//!	���C
	//!	@param	[in]	friction	���C�W��
	void SetFriction(f32 friction)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetFriction(friction);
	}
	//!	�����ړ�
	//!	@param	[in]	force	�ړ��x�N�g��
	void SetForce(const Vector3& force)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetForce(force);
	}
	//!	�����ړ�
	//!	@param	[in]	force	�ړ��x�N�g��
	void AddForce(const Vector3& force)
	{
		if (!_pRigidBody)return;
		_pRigidBody->AddForce(force);
	}
	//!	������]
	//!	@param	[in]	torque	������]
	void AddTorque(const Vector3& torque)
	{
		if (!_pRigidBody)return;
		_pRigidBody->AddTorque(torque);
	}
	//!	�d��
	//!	@param	[in]	gravity	�d�̓x�N�g��
	void SetGravity(const Vector3& gravity)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetGravity(gravity);
	}

	//!	�X�e�[�g�ݒ�
	//!	@param	[in]	newState	�X�e�[�g
	void SetActivationState(COLLISION_ACTIVATE newState)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetActivationState(newState);
	}

	//!	�X�e�[�g�ݒ�(����)
	//!	@param	[in]	newState	�X�e�[�g
	void SetForceActivationState(COLLISION_ACTIVATE newState)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetForceActivationState(newState);
	}

	//!	�R���W������Ԃ̐ݒ�
	//!	@param	[in]	flags	btCollisionObject::CF�n
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

	//!	����
	//!	@return	�����W��
	f32 GetRestitution()const
	{
		if (!_pRigidBody)return 0.f;
		return _pRigidBody->GetRestitution();
	}
	//!	���C
	//!	@return	���C�W��
	f32 GetFriction()const
	{
		if (!_pRigidBody)return 0.f;
		return _pRigidBody->GetFriction();
	}
	//!	�����ړ�
	//!	@return	�����ړ��x�N�g��
	Vector3 GetForce()const
	{
		if (!_pRigidBody)return Vector3::ZERO;
		return _pRigidBody->GetForce();
	}
	//!	������]
	//!	@return	������]����
	Vector3 GetTorque()const
	{
		if (!_pRigidBody)return Vector3::ZERO;
		return _pRigidBody->GetTorque();
	}
	//!	�d��
	//!	@return	�d�̓x�N�g��
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

	//!	�X�e�[�g�擾
	//!	@return	�X�e�[�g
	COLLISION_ACTIVATE GetActivationState()const
	{
		if (!_pRigidBody)return COLLISION_ACTIVATE::STATE_EMPTY;
		_pRigidBody->GetActivationState();
	}
	//!	�R���W������Ԃ̎擾
	//!	@return	btCollisionObject::CF�n
	s32 GetCollisionFlags()const
	{
		if (!_pRigidBody)return 0;
		return _pRigidBody->GetCollisionFlags();
	}


	//!	�A�N�e�B�u��
	//!	@param	[in]	forceActivation	�����A�N�e�B�u	default : false
	void Activate(bool forceActivation = false) const
	{
		if (!_pRigidBody)return;
		_pRigidBody->Activate(forceActivation);
	}

	//!	�����蔻��
	//!	@retval	true	������
	bool IsHit()const
	{
		return _pRigidBody ? _pRigidBody->IsContact() : false;
	}

	//!	�����폜
	void ResetRigidBody();

protected:
	RefPointer<RigidBody> _pRigidBody;
};

