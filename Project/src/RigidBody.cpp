//=============================================================================
//!
//!	@file	RigidBody.cpp
//!	@brief	物理コンポーネント
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
//!	物理コンポーネント
//=============================================================================
//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
RigidBody::~RigidBody()
{
	Manager::Collision()->RemoveRigidBody(this);
	delete _pBody->getMotionState();
	SafeDelete(_pShape);
	SafeDelete(_pBody);
}

//-----------------------------------------------------------------------------
//!	初期化
//!	@param	[in]	desc	引数リスト
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

	// 状態
	btDefaultMotionState* pMotionState = new btDefaultMotionState(btTransform(desc._angle, position));
	btVector3 inertia(0, 0, 0);
	_pShape->calculateLocalInertia(desc._mass, inertia);
	// 慣性
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(desc._mass, pMotionState, _pShape, inertia);
	rigidBodyCI.m_restitution = 0.3f;
	_pBody = new btRigidBody(rigidBodyCI);
	_pBody->setUserPointer(this);
	return true;
}

//-----------------------------------------------------------------------------
//!	初期化
//!	@param	[in]	desc	引数リスト
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
//!	初期化
//!	@param	[in]	desc	引数リスト
//!	@param	[in]	myGroup	自身のグループビット
//!	@param	[in]	filter	接触するグループビット
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
//!	更新
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
//!	物理更新
//-----------------------------------------------------------------------------
void RigidBody::RigidBodyUpdate()
{
	if(!_pParent) return;

	btTransform trans = _pBody->getWorldTransform();
	_pParent->SetWorldPosition(trans.getOrigin() /*- _centerOffset*/);
	_pParent->SetWorldRotate(Quaternion(trans.getRotation()));
}

//-----------------------------------------------------------------------------
//!	接触時に呼ばれる関数
//!	@param	[in]	contact	接触情報
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
//!	接触情報追加
//!	@param	[in]	contact	接触情報
//-----------------------------------------------------------------------------
void RigidBody::AddContactCollision(const ContactInfo& contact)
{
	_contacts.push_back(contact);
	Contact(contact);	//	接触関数を呼び出す
}

//-----------------------------------------------------------------------------
//!	接触情報削除
//-----------------------------------------------------------------------------
void RigidBody::ResetContactCollision()
{
	_contacts.clear();
}

//-----------------------------------------------------------------------------
//!	接触情報タグ検索
//!	@param	[in]	tag		タグ
//!	@return	検索結果の接触情報
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
//!	接触情報タグ検索
//!	@param	[in]	tags	タグ
//!	@return	検索結果の接触情報
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
//!	反発
//!	@param	[in]	restitution	反発係数
//-----------------------------------------------------------------------------
void RigidBody::SetRestitution(f32 restitution)
{
	_pBody->setRestitution(restitution);
}

//-----------------------------------------------------------------------------
//!	摩擦
//!	@param	[in]	friction	摩擦係数
//-----------------------------------------------------------------------------
void RigidBody::SetFriction(f32 friction)
{
	_pBody->setFriction(friction);
}

//-----------------------------------------------------------------------------
//!	物理回転
//!	@param	[in]	torque	回転方向
//-----------------------------------------------------------------------------
void RigidBody::AddTorque(const Vector3& torque)
{
	_pBody->applyTorque(torque);
}

//-----------------------------------------------------------------------------
//!	物理移動
//!	@param	[in]	force	移動ベクトル
//-----------------------------------------------------------------------------
void RigidBody::SetForce(const Vector3& force)
{
	ClearForceTorque();
	_pBody->applyCentralForce(force);
}

//-----------------------------------------------------------------------------
//!	物理移動
//!	@param	[in]	force	移動ベクトル
//-----------------------------------------------------------------------------
void RigidBody::AddForce(const Vector3& force)
{
	_pBody->applyCentralForce(force);
}

//-----------------------------------------------------------------------------
//!	重力
//!	@param	[in]	gravity	重力ベクトル
//-----------------------------------------------------------------------------
void RigidBody::SetGravity(const Vector3& gravity)
{
	_pBody->setGravity(gravity);
}

//-----------------------------------------------------------------------------
//!	反発
//!	@return	反発係数
//-----------------------------------------------------------------------------
f32 RigidBody::GetRestitution()const
{
	return _pBody->getRestitution();
}

//-----------------------------------------------------------------------------
//!	摩擦
//!	@return	摩擦係数
//-----------------------------------------------------------------------------
f32 RigidBody::GetFriction()const
{
	return _pBody->getFriction();
}

//-----------------------------------------------------------------------------
//!	物理回転
//!	@return	物理回転方向
//-----------------------------------------------------------------------------
Vector3 RigidBody::GetTorque()const
{
	return _pBody->getTotalTorque();
}

//-----------------------------------------------------------------------------
//!	物理移動
//!	@return	物理移動ベクトル
//-----------------------------------------------------------------------------
Vector3 RigidBody::GetForce()const
{
	Vector3 result = _pBody->getTotalForce();
	return result;
}

//-----------------------------------------------------------------------------
//!	重力
//!	@return	重力ベクトル
//-----------------------------------------------------------------------------
Vector3 RigidBody::GetGravity()const
{
	return _pBody->getGravity();
}

//-----------------------------------------------------------------------------
//!	物理移動リセット
//-----------------------------------------------------------------------------
void RigidBody::ClearForceTorque()
{
	_pBody->clearForces();
}

Vector3 RigidBody::GetLinearVelocity()const
{
	return _pBody->getLinearVelocity();
}

