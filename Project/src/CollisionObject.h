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
//! @brief  �����I�u�W�F�N�g
//-----------------------------------------------------------------------------
class CollisionObject : public GameObjectBase
{
public:
	friend RigidBody;

	//! @brief  �R���X�g���N�^
	CollisionObject();

	//! @brief  �f�X�g���N�^
	virtual ~CollisionObject();

	//! @brief  �����ݒ�
	//!	@param	[in]	desc	�������X�g
	//!	@param	[in]	tag		�R���|�[�l���g�ǉ��^�O	default:""
	bool SetRigidBody(const RigidBody::Desc& desc, const std::string& tag = "");

	//! @brief  �����ݒ�
	//!	@param	[in]	desc	�������X�g
	//!	@param	[in]	myGroup	���g�̃O���[�v�r�b�g
	//!	@param	[in]	filter	�ڐG����O���[�v�r�b�g
	//!	@param	[in]	tag		�R���|�[�l���g�ǉ��^�O	default:""
	bool SetRigidBody(const RigidBody::Desc& desc, u16 myGroup, u16 filter, const std::string& tag = "");

	//! @brief  �ڐG�֐�
	//!	@param	[in]	contact	�ڐG���
	virtual void Contact(const ContactInfo& contact);

	//! @brief  �����R���|�[�l���g�擾
	//!	@return	�����R���|�[�l���g
	RigidBody* GetRigidBody();

	//! @brief  ����
	//!	@param	[in]	restitution	�����W��
	void SetRestitution(f32 restitution);

	//! @brief  ���C
	//!	@param	[in]	friction	���C�W��
	void SetFriction(f32 friction);

	//! @brief  �����ړ�
	//!	@param	[in]	force	�ړ��x�N�g��
	void SetForce(const Vector3& force);

	//! @brief  �����ړ�
	//!	@param	[in]	force	�ړ��x�N�g��
	void AddForce(const Vector3& force);

	//! @brief  ������]
	//!	@param	[in]	torque	������]
	void AddTorque(const Vector3& torque);

	//! @brief  �d��
	//!	@param	[in]	gravity	�d�̓x�N�g��
	void SetGravity(const Vector3& gravity);

	//! @brief  �X�e�[�g�ݒ�
	//!	@param	[in]	newState	�X�e�[�g
	void SetActivationState(COLLISION_ACTIVATE newState);

	//! @brief  �X�e�[�g�ݒ�(����)
	//!	@param	[in]	newState	�X�e�[�g
	void SetForceActivationState(COLLISION_ACTIVATE newState);

	//! @brief  �R���W������Ԃ̐ݒ�
	//!	@param	[in]	flags	btCollisionObject::CF�n
	void SetCollisionFlags(s32 flags);

	//! @brief  ���ʐݒ�
	//!	@param	[in]	mass	����
	void SetMass(f32 mass);

	//! @brief  ����
	//!	@return	�����W��
	f32 GetRestitution()const;

	//! @brief  ���C
	//!	@return	���C�W��
	f32 GetFriction()const;

	//! @brief  �����ړ�
	//!	@return	�����ړ��x�N�g��
	Vector3 GetForce()const;

	//! @brief  ������]
	//!	@return	������]����
	Vector3 GetTorque()const;

	//! @brief  �d��
	//!	@return	�d�̓x�N�g��
	Vector3 GetGravity()const;

	//! @brief  ���`���x
	//!	@return	���`���x�x�N�g��
	Vector3 GetLinearVelocity()const;

	//! @brief  �X�e�[�g�擾
	//!	@return	�X�e�[�g
	COLLISION_ACTIVATE GetActivationState()const;

	//! @brief  �R���W������Ԃ̎擾
	//!	@return	btCollisionObject::CF�n
	s32 GetCollisionFlags()const;

	//! @brief  �A�N�e�B�u��
	//!	@param	[in]	forceActivation	�����A�N�e�B�u	default : false
	void Activate(bool forceActivation = false) const;

	//! @brief  �����蔻��
	//!	@retval	true	������
	bool IsHit()const;

	//! @brief  �����폜
	void ResetRigidBody();

protected:
	RefPointer<RigidBody> _pRigidBody;
};

