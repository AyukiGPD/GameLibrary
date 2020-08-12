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

//-----------------------------------------------------------------------------
//  @brief  コンストラクタ
//-----------------------------------------------------------------------------
RigidBody::RigidBody()
	: _pShape(nullptr)
	, _pBody(nullptr)
{
	AddTag("RigidBody");
}

//-----------------------------------------------------------------------------
//  @brief  コンストラクタ
//  @param	[in]	pParent	親
//  @param	[in]	desc	引数リスト
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
//  @brief  コンストラクタ
//  @param	[in]	pParent	親
//  @param	[in]	desc	引数リスト
//  @param	[in]	myGroup	自身のグループビット
//  @param	[in]	filter	接触するグループビット
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
//!	物理コンポーネント
//=============================================================================
//-----------------------------------------------------------------------------
//  @brief  デストラクタ
//-----------------------------------------------------------------------------
RigidBody::~RigidBody()
{
	Manager::Collision()->RemoveRigidBody(this);
	delete _pBody->getMotionState();
	SafeDelete(_pShape);
	SafeDelete(_pBody);
}

//-----------------------------------------------------------------------------
//  @brief  初期化
//  @param	[in]	desc	引数リスト
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
//  @brief  初期化
//  @param	[in]	desc	引数リスト
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
//  @brief  初期化
//  @param	[in]	desc	引数リスト
//  @param	[in]	myGroup	自身のグループビット
//  @param	[in]	filter	接触するグループビット
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
//  @brief  更新
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
//  @brief  物理更新
//-----------------------------------------------------------------------------
void RigidBody::RigidBodyUpdate()
{
	if(!_pParent) return;

	btTransform trans = _pBody->getWorldTransform();
	_pParent->SetWorldPosition(trans.getOrigin() /*- _centerOffset*/);
	_pParent->SetWorldRotate(Quaternion(trans.getRotation()));
}

//-----------------------------------------------------------------------------
//  @brief  接触時に呼ばれる関数
//  @param	[in]	contact	接触情報
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
//  @brief  接触情報追加
//  @param	[in]	contact	接触情報
//-----------------------------------------------------------------------------
void RigidBody::AddContactCollision(const ContactInfo& contact)
{
	_contacts.push_back(contact);
	Contact(contact);	//	接触関数を呼び出す
}

//-----------------------------------------------------------------------------
//  @brief  接触情報削除
//-----------------------------------------------------------------------------
void RigidBody::ResetContactCollision()
{
	_contacts.clear();
}

//-----------------------------------------------------------------------------
//  @brief  接触情報タグ検索
//  @param	[in]	tag		タグ
//  @return	検索結果の接触情報
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
//  @brief  接触情報タグ検索
//  @param	[in]	tags	タグ
//  @return	検索結果の接触情報
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
//  @brief  反発
//  @param	[in]	restitution	反発係数
//-----------------------------------------------------------------------------
void RigidBody::SetRestitution(f32 restitution)
{
	_pBody->setRestitution(restitution);
}

//-----------------------------------------------------------------------------
//  @brief  摩擦
//  @param	[in]	friction	摩擦係数
//-----------------------------------------------------------------------------
void RigidBody::SetFriction(f32 friction)
{
	_pBody->setFriction(friction);
}

//-----------------------------------------------------------------------------
//  @brief  物理回転
//  @param	[in]	torque	回転方向
//-----------------------------------------------------------------------------
void RigidBody::AddTorque(const Vector3& torque)
{
	_pBody->applyTorque(torque);
}

//-----------------------------------------------------------------------------
//  @brief  物理移動
//  @param	[in]	force	移動ベクトル
//-----------------------------------------------------------------------------
void RigidBody::SetForce(const Vector3& force)
{
	ClearForceTorque();
	_pBody->applyCentralForce(force);
}

//-----------------------------------------------------------------------------
//  @brief  物理移動
//  @param	[in]	force	移動ベクトル
//-----------------------------------------------------------------------------
void RigidBody::AddForce(const Vector3& force)
{
	_pBody->applyCentralForce(force);
}

//-----------------------------------------------------------------------------
//  @brief  重力
//  @param	[in]	gravity	重力ベクトル
//-----------------------------------------------------------------------------
void RigidBody::SetGravity(const Vector3& gravity)
{
	_pBody->setGravity(gravity);
}

//-----------------------------------------------------------------------------
//  @brief  反発
//  @return	反発係数
//-----------------------------------------------------------------------------
f32 RigidBody::GetRestitution()const
{
	return _pBody->getRestitution();
}

//-----------------------------------------------------------------------------
//  @brief  摩擦
//  @return	摩擦係数
//-----------------------------------------------------------------------------
f32 RigidBody::GetFriction()const
{
	return _pBody->getFriction();
}

//-----------------------------------------------------------------------------
//  @brief  物理回転
//  @return	物理回転方向
//-----------------------------------------------------------------------------
Vector3 RigidBody::GetTorque()const
{
	return _pBody->getTotalTorque();
}

//-----------------------------------------------------------------------------
//  @brief  物理移動
//  @return	物理移動ベクトル
//-----------------------------------------------------------------------------
Vector3 RigidBody::GetForce()const
{
	Vector3 result = _pBody->getTotalForce();
	return result;
}

//-----------------------------------------------------------------------------
//  @brief  重力
//  @return	重力ベクトル
//-----------------------------------------------------------------------------
Vector3 RigidBody::GetGravity()const
{
	return _pBody->getGravity();
}

//-----------------------------------------------------------------------------
//  @brief  物理移動リセット
//-----------------------------------------------------------------------------
void RigidBody::ClearForceTorque()
{
	_pBody->clearForces();
}

//-----------------------------------------------------------------------------
//  @brief  線形速度
//  @return	線形速度ベクトル
//-----------------------------------------------------------------------------
Vector3 RigidBody::GetLinearVelocity()const
{
	return _pBody->getLinearVelocity();
}

//-----------------------------------------------------------------------------
//  @brief  当たり判定
//  @retval	true	あたり
//-----------------------------------------------------------------------------
bool RigidBody::IsContact() const
{
	return !_contacts.empty();
}

//-----------------------------------------------------------------------------
//  @brief  接触情報取得
//  @return	接触情報の配列
//-----------------------------------------------------------------------------
std::vector<ContactInfo> RigidBody::GetContacts() const
{
	return _contacts;
}

//-----------------------------------------------------------------------------
//  @brief  Bulletの物理オブジェクト
//  @return	Bulletの物理オブジェクト
//-----------------------------------------------------------------------------
btRigidBody* RigidBody::GetbtRigidBody()
{
	return _pBody;
}

//-----------------------------------------------------------------------------
//  @brief  アクティブ化
//  @param	[in]	forceActivation	強制アクティブ	default : false
//-----------------------------------------------------------------------------
void RigidBody::Activate(bool forceActivation) const
{
	if (_pBody)
	{
		_pBody->activate(forceActivation);
	}
}

//-----------------------------------------------------------------------------
//  @brief  コリジョン状態の設定
//  @param	[in]	flags
//-----------------------------------------------------------------------------
void RigidBody::SetCollisionFlags(s32 flags)
{
	_pBody->setCollisionFlags(flags);
}

//-----------------------------------------------------------------------------
//  @brief  コリジョン状態の取得
//  @return	コリジョンフラグ
//-----------------------------------------------------------------------------
s32 RigidBody::GetCollisionFlags() const
{
	return _pBody->getCollisionFlags();
}

//-----------------------------------------------------------------------------
//  @brief  ステート設定
//  @param	[in]	newState	ステート
//-----------------------------------------------------------------------------
void RigidBody::SetActivationState(COLLISION_ACTIVATE newState)
{
	if (_pBody)
	{
		_pBody->setActivationState((s32)newState);
	}
}

//-----------------------------------------------------------------------------
//  @brief  ステート設定(強制)
//  @param	[in]	newState	ステート
//-----------------------------------------------------------------------------
void RigidBody::SetForceActivationState(COLLISION_ACTIVATE newState)
{
	if (_pBody)
	{
		_pBody->forceActivationState((s32)newState);
	}
}

//-----------------------------------------------------------------------------
//  @brief  ステート取得
//  @return	ステート
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
//  @brief  質量設定
//  @param	[in]	mass	質量
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

