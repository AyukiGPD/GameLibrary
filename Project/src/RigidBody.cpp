//=============================================================================
//!
//!	@file	RigidBody.cpp
//!	@brief	�����R���|�[�l���g
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
RigidBody::RigidBody()
	: _pShape(nullptr)
	, _pBody(nullptr)
{
	AddTag("RigidBody");
}

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//  @param	[in]	pParent	�e
//  @param	[in]	desc	�������X�g
//-----------------------------------------------------------------------------
RigidBody::RigidBody(GameObjectBase* pParent, const Desc& desc)
	: ProcessBase(pParent)
	, _pShape(nullptr)
	, _pBody(nullptr)
{
	AddTag("RigidBody");
	Init(desc);
}


//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//  @param	[in]	pParent	�e
//  @param	[in]	desc	�������X�g
//  @param	[in]	myGroup	���g�̃O���[�v�r�b�g
//  @param	[in]	filter	�ڐG����O���[�v�r�b�g
//-----------------------------------------------------------------------------
RigidBody::RigidBody(GameObjectBase* pParent, const Desc& desc, u16 myGroup, u16 filter)
	: ProcessBase(pParent)
	, _pShape(nullptr)
	, _pBody(nullptr)
{
	AddTag("RigidBody");
	Init(desc, myGroup, filter);
}

//=============================================================================
//!	�����R���|�[�l���g
//=============================================================================
//-----------------------------------------------------------------------------
//  @brief  �f�X�g���N�^
//-----------------------------------------------------------------------------
RigidBody::~RigidBody()
{
	Manager::Collision()->RemoveRigidBody(this);
	delete _pBody->getMotionState();
	SafeDelete(_pShape);
	SafeDelete(_pBody);
}

//-----------------------------------------------------------------------------
//  @brief  ������
//  @param	[in]	desc	�������X�g
//-----------------------------------------------------------------------------
bool RigidBody::InitBulletObject( const Desc& desc )
{
	btCollisionShape* pShape = nullptr;
	switch (desc._shapeType)
	{
	case SHAPE::BOX:
		pShape = new btBoxShape(btVector3(desc._box.x, desc._box.y, desc._box.z));
		break;
	case SHAPE::CAPSULE:
		pShape = new btCapsuleShape(desc._capsule.radius, desc._capsule.height);
		break;
	case SHAPE::CONE:
		pShape = new btConeShape(desc._cone.radius, desc._cone.height);
		break;
	case SHAPE::CYLINDER:
		pShape = new btCylinderShape(btVector3(desc._cylinder.x, desc._cylinder.y, desc._cylinder.z));
		break;
	case SHAPE::SPHERE:
		pShape = new btSphereShape(desc._sphere);
		break;
	default:
		return false;
		break;
	}
	_pShape = pShape;
//	_centerOffset = desc._position;
	Vector3 position = desc._position + _pParent->GetWorldPosition();

	// ���
	btDefaultMotionState* pMotionState = new btDefaultMotionState(btTransform(desc._angle, position));
	btVector3 inertia(0, 0, 0);
	_pShape->calculateLocalInertia(desc._mass, inertia);
	// ����
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(desc._mass, pMotionState, _pShape, inertia);
	rigidBodyCI.m_restitution = 0.3f;
	_pBody = new btRigidBody(rigidBodyCI);
	_pBody->setUserPointer(this);
	return true;
}

//-----------------------------------------------------------------------------
//  @brief  ������
//  @param	[in]	desc	�������X�g
//-----------------------------------------------------------------------------
bool RigidBody::Init(const Desc& desc)
{
	if (desc._shapeType == SHAPE::NONE){ return false; }
	if (!_pParent){ return false; }

	bool result = InitBulletObject( desc );

	if (result)
	{
		auto* pCollision = Manager::Collision();
		pCollision->AddRigidBody(this);
	}
	return result;
}


//-----------------------------------------------------------------------------
//  @brief  ������
//  @param	[in]	desc	�������X�g
//  @param	[in]	myGroup	���g�̃O���[�v�r�b�g
//  @param	[in]	filter	�ڐG����O���[�v�r�b�g
//-----------------------------------------------------------------------------
bool RigidBody::Init(const Desc& desc, u16 myGroup, u16 filter)
{
	if (desc._shapeType == SHAPE::NONE){ return false; }
	if (!_pParent){ return false; }

	bool result = InitBulletObject( desc );

	if (result)
	{
		auto* pCollision = Manager::Collision();
		pCollision->AddRigidBody(this,myGroup,filter);
	}
	return result;
}


//-----------------------------------------------------------------------------
//  @brief  �X�V
//-----------------------------------------------------------------------------
void RigidBody::Update()
{
	btTransform trans;

	Matrix matWorld = _pParent->GetWorldMatrix();
	Vector3 scale = matWorld.GetScale();
	matWorld = Matrix::Normalize(matWorld);

	trans.setOrigin(/*_centerOffset +*/ matWorld.GetPosition());
	trans.setRotation(matWorld.GetRotateQuaternion());

	_pBody->setWorldTransform(trans);
	_pBody->getCollisionShape()->setLocalScaling(scale);
}

//-----------------------------------------------------------------------------
//  @brief  �����X�V
//-----------------------------------------------------------------------------
void RigidBody::RigidBodyUpdate()
{
	if(!_pParent) return;

	btTransform trans = _pBody->getWorldTransform();
	_pParent->SetWorldPosition(trans.getOrigin() /*- _centerOffset*/);
	_pParent->SetWorldRotate(Quaternion(trans.getRotation()));
}

//-----------------------------------------------------------------------------
//  @brief  �ڐG���ɌĂ΂��֐�
//  @param	[in]	contact	�ڐG���
//-----------------------------------------------------------------------------
void RigidBody::Contact(const ContactInfo& contact)
{
	if(_pParent)
	{
		CollisionObject* pC_Object = dynamic_cast<CollisionObject*>(_pParent);
		if(pC_Object)
		{
			pC_Object->Contact(contact);
		}
	}
}

//-----------------------------------------------------------------------------
//  @brief  �ڐG���ǉ�
//  @param	[in]	contact	�ڐG���
//-----------------------------------------------------------------------------
void RigidBody::AddContactCollision(const ContactInfo& contact)
{
	_contacts.push_back(contact);
	Contact(contact);	//	�ڐG�֐����Ăяo��
}

//-----------------------------------------------------------------------------
//  @brief  �ڐG���폜
//-----------------------------------------------------------------------------
void RigidBody::ResetContactCollision()
{
	_contacts.clear();
}

//-----------------------------------------------------------------------------
//  @brief  �ڐG���^�O����
//  @param	[in]	tag		�^�O
//  @return	�������ʂ̐ڐG���
//-----------------------------------------------------------------------------
ContactInfo RigidBody::SearchContactTag(const std::string& tag)const
{
	ContactInfo result;
	for (auto contact : _contacts)
	{
		if (!contact._pRigidBody->SearchTag(tag))continue;
		result = contact;
		break;
	}
	return result;
}

//-----------------------------------------------------------------------------
//  @brief  �ڐG���^�O����
//  @param	[in]	tags	�^�O
//  @return	�������ʂ̐ڐG���
//-----------------------------------------------------------------------------
ContactInfo RigidBody::SearchContactTag(const Tags& tags)const
{
	ContactInfo result;
	for (auto contact : _contacts)
	{
		if (!contact._pRigidBody->SearchTag(tags))continue;
		result = contact;
	}
	return result;
}

//-----------------------------------------------------------------------------
//  @brief  ����
//  @param	[in]	restitution	�����W��
//-----------------------------------------------------------------------------
void RigidBody::SetRestitution(f32 restitution)
{
	_pBody->setRestitution(restitution);
}

//-----------------------------------------------------------------------------
//  @brief  ���C
//  @param	[in]	friction	���C�W��
//-----------------------------------------------------------------------------
void RigidBody::SetFriction(f32 friction)
{
	_pBody->setFriction(friction);
}

//-----------------------------------------------------------------------------
//  @brief  ������]
//  @param	[in]	torque	��]����
//-----------------------------------------------------------------------------
void RigidBody::AddTorque(const Vector3& torque)
{
	_pBody->applyTorque(torque);
}

//-----------------------------------------------------------------------------
//  @brief  �����ړ�
//  @param	[in]	force	�ړ��x�N�g��
//-----------------------------------------------------------------------------
void RigidBody::SetForce(const Vector3& force)
{
	ClearForceTorque();
	_pBody->applyCentralForce(force);
}

//-----------------------------------------------------------------------------
//  @brief  �����ړ�
//  @param	[in]	force	�ړ��x�N�g��
//-----------------------------------------------------------------------------
void RigidBody::AddForce(const Vector3& force)
{
	_pBody->applyCentralForce(force);
}

//-----------------------------------------------------------------------------
//  @brief  �d��
//  @param	[in]	gravity	�d�̓x�N�g��
//-----------------------------------------------------------------------------
void RigidBody::SetGravity(const Vector3& gravity)
{
	_pBody->setGravity(gravity);
}

//-----------------------------------------------------------------------------
//  @brief  ����
//  @return	�����W��
//-----------------------------------------------------------------------------
f32 RigidBody::GetRestitution()const
{
	return _pBody->getRestitution();
}

//-----------------------------------------------------------------------------
//  @brief  ���C
//  @return	���C�W��
//-----------------------------------------------------------------------------
f32 RigidBody::GetFriction()const
{
	return _pBody->getFriction();
}

//-----------------------------------------------------------------------------
//  @brief  ������]
//  @return	������]����
//-----------------------------------------------------------------------------
Vector3 RigidBody::GetTorque()const
{
	return _pBody->getTotalTorque();
}

//-----------------------------------------------------------------------------
//  @brief  �����ړ�
//  @return	�����ړ��x�N�g��
//-----------------------------------------------------------------------------
Vector3 RigidBody::GetForce()const
{
	Vector3 result = _pBody->getTotalForce();
	return result;
}

//-----------------------------------------------------------------------------
//  @brief  �d��
//  @return	�d�̓x�N�g��
//-----------------------------------------------------------------------------
Vector3 RigidBody::GetGravity()const
{
	return _pBody->getGravity();
}

//-----------------------------------------------------------------------------
//  @brief  �����ړ����Z�b�g
//-----------------------------------------------------------------------------
void RigidBody::ClearForceTorque()
{
	_pBody->clearForces();
}

//-----------------------------------------------------------------------------
//  @brief  ���`���x
//  @return	���`���x�x�N�g��
//-----------------------------------------------------------------------------
Vector3 RigidBody::GetLinearVelocity()const
{
	return _pBody->getLinearVelocity();
}

//-----------------------------------------------------------------------------
//  @brief  �����蔻��
//  @retval	true	������
//-----------------------------------------------------------------------------
bool RigidBody::IsContact() const
{
	return !_contacts.empty();
}

//-----------------------------------------------------------------------------
//  @brief  �ڐG���擾
//  @return	�ڐG���̔z��
//-----------------------------------------------------------------------------
std::vector<ContactInfo> RigidBody::GetContacts() const
{
	return _contacts;
}

//-----------------------------------------------------------------------------
//  @brief  Bullet�̕����I�u�W�F�N�g
//  @return	Bullet�̕����I�u�W�F�N�g
//-----------------------------------------------------------------------------
btRigidBody* RigidBody::GetbtRigidBody()
{
	return _pBody;
}

//-----------------------------------------------------------------------------
//  @brief  �A�N�e�B�u��
//  @param	[in]	forceActivation	�����A�N�e�B�u	default : false
//-----------------------------------------------------------------------------
void RigidBody::Activate(bool forceActivation) const
{
	if (_pBody)
	{
		_pBody->activate(forceActivation);
	}
}

//-----------------------------------------------------------------------------
//  @brief  �R���W������Ԃ̐ݒ�
//  @param	[in]	flags
//-----------------------------------------------------------------------------
void RigidBody::SetCollisionFlags(s32 flags)
{
	_pBody->setCollisionFlags(flags);
}

//-----------------------------------------------------------------------------
//  @brief  �R���W������Ԃ̎擾
//  @return	�R���W�����t���O
//-----------------------------------------------------------------------------
s32 RigidBody::GetCollisionFlags() const
{
	return _pBody->getCollisionFlags();
}

//-----------------------------------------------------------------------------
//  @brief  �X�e�[�g�ݒ�
//  @param	[in]	newState	�X�e�[�g
//-----------------------------------------------------------------------------
void RigidBody::SetActivationState(COLLISION_ACTIVATE newState)
{
	if (_pBody)
	{
		_pBody->setActivationState((s32)newState);
	}
}

//-----------------------------------------------------------------------------
//  @brief  �X�e�[�g�ݒ�(����)
//  @param	[in]	newState	�X�e�[�g
//-----------------------------------------------------------------------------
void RigidBody::SetForceActivationState(COLLISION_ACTIVATE newState)
{
	if (_pBody)
	{
		_pBody->forceActivationState((s32)newState);
	}
}

//-----------------------------------------------------------------------------
//  @brief  �X�e�[�g�擾
//  @return	�X�e�[�g
//-----------------------------------------------------------------------------
COLLISION_ACTIVATE RigidBody::GetActivationState() const
{
	if (_pBody)
	{
		return (COLLISION_ACTIVATE)_pBody->getActivationState();
	}
	return COLLISION_ACTIVATE::STATE_EMPTY;
}

//-----------------------------------------------------------------------------
//  @brief  ���ʐݒ�
//  @param	[in]	mass	����
//-----------------------------------------------------------------------------
void RigidBody::SetMass(f32 mass)
{
	if (_pBody)
	{
		btVector3 inertia;
		_pBody->getCollisionShape()->calculateLocalInertia(mass, inertia);
		_pBody->setMassProps(mass, inertia);
	}
}

