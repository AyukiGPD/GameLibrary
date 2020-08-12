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

//=============================================================================
//!	�����R���|�[�l���g
//=============================================================================
//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
RigidBody::~RigidBody()
{
	Manager::Collision()->RemoveRigidBody(this);
	delete _pBody->getMotionState();
	SafeDelete(_pShape);
	SafeDelete(_pBody);
}

//-----------------------------------------------------------------------------
//!	������
//!	@param	[in]	desc	�������X�g
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
//!	������
//!	@param	[in]	desc	�������X�g
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
//!	������
//!	@param	[in]	desc	�������X�g
//!	@param	[in]	myGroup	���g�̃O���[�v�r�b�g
//!	@param	[in]	filter	�ڐG����O���[�v�r�b�g
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
//!	�X�V
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
//!	�����X�V
//-----------------------------------------------------------------------------
void RigidBody::RigidBodyUpdate()
{
	if(!_pParent) return;

	btTransform trans = _pBody->getWorldTransform();
	_pParent->SetWorldPosition(trans.getOrigin() /*- _centerOffset*/);
	_pParent->SetWorldRotate(Quaternion(trans.getRotation()));
}

//-----------------------------------------------------------------------------
//!	�ڐG���ɌĂ΂��֐�
//!	@param	[in]	contact	�ڐG���
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
//!	�ڐG���ǉ�
//!	@param	[in]	contact	�ڐG���
//-----------------------------------------------------------------------------
void RigidBody::AddContactCollision(const ContactInfo& contact)
{
	_contacts.push_back(contact);
	Contact(contact);	//	�ڐG�֐����Ăяo��
}

//-----------------------------------------------------------------------------
//!	�ڐG���폜
//-----------------------------------------------------------------------------
void RigidBody::ResetContactCollision()
{
	_contacts.clear();
}

//-----------------------------------------------------------------------------
//!	�ڐG���^�O����
//!	@param	[in]	tag		�^�O
//!	@return	�������ʂ̐ڐG���
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
//!	�ڐG���^�O����
//!	@param	[in]	tags	�^�O
//!	@return	�������ʂ̐ڐG���
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
//!	����
//!	@param	[in]	restitution	�����W��
//-----------------------------------------------------------------------------
void RigidBody::SetRestitution(f32 restitution)
{
	_pBody->setRestitution(restitution);
}

//-----------------------------------------------------------------------------
//!	���C
//!	@param	[in]	friction	���C�W��
//-----------------------------------------------------------------------------
void RigidBody::SetFriction(f32 friction)
{
	_pBody->setFriction(friction);
}

//-----------------------------------------------------------------------------
//!	������]
//!	@param	[in]	torque	��]����
//-----------------------------------------------------------------------------
void RigidBody::AddTorque(const Vector3& torque)
{
	_pBody->applyTorque(torque);
}

//-----------------------------------------------------------------------------
//!	�����ړ�
//!	@param	[in]	force	�ړ��x�N�g��
//-----------------------------------------------------------------------------
void RigidBody::SetForce(const Vector3& force)
{
	ClearForceTorque();
	_pBody->applyCentralForce(force);
}

//-----------------------------------------------------------------------------
//!	�����ړ�
//!	@param	[in]	force	�ړ��x�N�g��
//-----------------------------------------------------------------------------
void RigidBody::AddForce(const Vector3& force)
{
	_pBody->applyCentralForce(force);
}

//-----------------------------------------------------------------------------
//!	�d��
//!	@param	[in]	gravity	�d�̓x�N�g��
//-----------------------------------------------------------------------------
void RigidBody::SetGravity(const Vector3& gravity)
{
	_pBody->setGravity(gravity);
}

//-----------------------------------------------------------------------------
//!	����
//!	@return	�����W��
//-----------------------------------------------------------------------------
f32 RigidBody::GetRestitution()const
{
	return _pBody->getRestitution();
}

//-----------------------------------------------------------------------------
//!	���C
//!	@return	���C�W��
//-----------------------------------------------------------------------------
f32 RigidBody::GetFriction()const
{
	return _pBody->getFriction();
}

//-----------------------------------------------------------------------------
//!	������]
//!	@return	������]����
//-----------------------------------------------------------------------------
Vector3 RigidBody::GetTorque()const
{
	return _pBody->getTotalTorque();
}

//-----------------------------------------------------------------------------
//!	�����ړ�
//!	@return	�����ړ��x�N�g��
//-----------------------------------------------------------------------------
Vector3 RigidBody::GetForce()const
{
	Vector3 result = _pBody->getTotalForce();
	return result;
}

//-----------------------------------------------------------------------------
//!	�d��
//!	@return	�d�̓x�N�g��
//-----------------------------------------------------------------------------
Vector3 RigidBody::GetGravity()const
{
	return _pBody->getGravity();
}

//-----------------------------------------------------------------------------
//!	�����ړ����Z�b�g
//-----------------------------------------------------------------------------
void RigidBody::ClearForceTorque()
{
	_pBody->clearForces();
}

Vector3 RigidBody::GetLinearVelocity()const
{
	return _pBody->getLinearVelocity();
}

