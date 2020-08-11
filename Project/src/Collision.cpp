//=============================================================================
//!
//!	@file	Collision.cpp
//!	@brief	あたり判定
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
//!	デバッグ描画クラス
//=============================================================================
class MyBulletDebugDraw : public btIDebugDraw
{
private:
	//デバッグモード
	s32 m_DebugMode;
public:
	//コストラクタ
	MyBulletDebugDraw()
		: m_DebugMode(1)
	{}

	//	ラインの描画
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		Color lineColor;
		lineColor.SetFloat(color.x(), color.y(), color.z());
		Render3D::Line(from, to, lineColor);
	}

	//	ラインの描画
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
	{
		Color fromLineColor;
		Color toLineColor;
		fromLineColor.SetFloat(fromColor.x(), fromColor.y(), fromColor.z());
		toLineColor.SetFloat(toColor.x(), toColor.y(), toColor.z());
		Render3D::Line(from, to, fromLineColor, toLineColor);
	}


	//	接触点の描画
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
	{
		UNUSED(lifeTime);
		Vector3 to = PointOnB + normalOnB * distance;
		const Vector3& from = PointOnB;
		Color lineColor;
		lineColor.SetFloat(color.x(), color.y(), color.z());
		Render3D::Line(from, to, lineColor);
	}

	//	エラーの表示
	void reportErrorWarning(const char* warningString)
	{
		DEBUG_LOG(warningString);
	}

	//	未実装
	//	３Ｄテキストの描画
	void draw3dText(const btVector3& location, const char* textString)
	{
		UNUSED(location);
		UNUSED(textString);
	}

	//	デバッグモードの設定
	void setDebugMode(int debugMode)
	{
		m_DebugMode = debugMode;
	}

	//	デバッグモードの取得
	int getDebugMode() const
	{
		return m_DebugMode;
	}
};

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

//=============================================================================
//!	Bullet管理クラス
//=============================================================================
//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
CollisionManager::CollisionManager()
: _pCollisionConfiguration(nullptr)
, _pDispatcher(nullptr)
, _pBroadphase(nullptr)
, _pSolver(nullptr)
, _pDebugDraw(nullptr)
, _pDynamicsWorld(nullptr)
, _isDebugRender(true)
, _isUpdate(true)
{
}

//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
CollisionManager::~CollisionManager()
{
	Exit();
}

//! コンタクトのコールバック
extern ContactProcessedCallback gContactProcessedCallback;

//-----------------------------------------------------------------------------
//!	初期化
//-----------------------------------------------------------------------------
void CollisionManager::Init()
{
	// 衝突検出スタックアロケータ
	_pCollisionConfiguration = new btDefaultCollisionConfiguration();
	// 高精度の衝突検知(ナローフェーズ)
	_pDispatcher = new btCollisionDispatcher(_pCollisionConfiguration);
	// 低精度の衝突検知(ブロードフェーズ)
	_pBroadphase = new btDbvtBroadphase();
	// ジョイント等管理
	_pSolver = new btSequentialImpulseConstraintSolver;
	// 衝突検出のためのインタフェースとコンテ(ワールド)
	_pDynamicsWorld = new btDiscreteDynamicsWorld(_pDispatcher, _pBroadphase, _pSolver, _pCollisionConfiguration);
	// Debug描画設定
	_pDebugDraw = new MyBulletDebugDraw();
	_pDynamicsWorld->setDebugDrawer(_pDebugDraw);
	// 重力の設定
	_pDynamicsWorld->setGravity(btVector3(0, -9.8f, 0));
	gContactProcessedCallback = CollisionManager::ContactProcessed;
}

//-----------------------------------------------------------------------------
//!	更新
//-----------------------------------------------------------------------------
void CollisionManager::Update()
{
	if (!_isUpdate)return;
	for (auto* pBody : _pRigidBodies)
	{
		pBody->ResetContactCollision();
	}
	f32 deltaTime = GetDeltaTime();

	static const f32 maxtime = 1.0f / 60.0f;
	f32 time = deltaTime;
	for (;;)
	{
		f32 t = time;
		if (t > maxtime)t = maxtime;
		_pDynamicsWorld->stepSimulation(maxtime, 1, maxtime);
		time -= maxtime;
		if (time < maxtime)break;
	}

	for (auto* pBody : _pRigidBodies)
	{
		pBody->RigidBodyUpdate();
	}
}

//-----------------------------------------------------------------------------
//!	描画
//-----------------------------------------------------------------------------
void CollisionManager::Render()
{
	if (_isDebugRender)
	{
		Manager::Shader()->SetUniform(14, Vector3(0));
		Manager::Renderer()->SetWorldMatrix(Matrix::IDENTITY);
		_pDynamicsWorld->debugDrawWorld();
	}
}

//-----------------------------------------------------------------------------
//!	終了
//-----------------------------------------------------------------------------
void CollisionManager::Exit()
{
	SafeDelete(_pDebugDraw);
	SafeDelete(_pDynamicsWorld);
	SafeDelete(_pSolver);
	SafeDelete(_pCollisionConfiguration);
	SafeDelete(_pDispatcher);
	SafeDelete(_pBroadphase);
}

//-----------------------------------------------------------------------------
//!	物理コリジョン追加
//!	@param	[in]	pBody	物理コンポーネント
//-----------------------------------------------------------------------------
void CollisionManager::AddRigidBody(RigidBody* pBody)
{
	if (!pBody) return;
	_pDynamicsWorld->addRigidBody(pBody->_pBody);
	_pRigidBodies.push_back(pBody);
}

//-----------------------------------------------------------------------------
//!	物理コリジョン追加
//!	@param	[in]	pBody	物理コンポーネント
//!	@param	[in]	myGroup	自身のグループビット
//!	@param	[in]	filter	接触するグループビット
//-----------------------------------------------------------------------------
void CollisionManager::AddRigidBody(RigidBody* pBody, u16 myGroup, u16 filter)
{
	if (!pBody) return;
	_pDynamicsWorld->addRigidBody(pBody->_pBody, myGroup, filter);
	_pRigidBodies.push_back(pBody);
}

//-----------------------------------------------------------------------------
//!	物理コリジョン削除
//!	@param	[in]	pBody	物理コンポーネント
//-----------------------------------------------------------------------------
void CollisionManager::RemoveRigidBody(RigidBody* pBody)
{
	if (!pBody) return;
	auto itr = _pRigidBodies.begin();
	while (itr != _pRigidBodies.end())
	{
		if ((*itr) == pBody)
		{
			_pRigidBodies.erase(itr);
			break;
		}
		++itr;
	}
	pBody->_pBody->setUserPointer(nullptr);
	_pDynamicsWorld->removeRigidBody(pBody->_pBody);
}

//-----------------------------------------------------------------------------
//!	グローバルな重力設定
//!	@param	[in]	gravity	重力ベクトル
//-----------------------------------------------------------------------------
void CollisionManager::SetGravity(const Vector3& gravity)
{
	_pDynamicsWorld->setGravity(gravity);
}

//	@bug Contactの先で自殺をすると2つ目のポインタが空になる
//-----------------------------------------------------------------------------
//!	コリジョンコンタクトのコールバック関数
//-----------------------------------------------------------------------------
bool CollisionManager::ContactProcessed(btManifoldPoint& p, void* a, void* b)
{
	btRigidBody* pBody0 = (btRigidBody*)a;
	btRigidBody* pBody1 = (btRigidBody*)b;

	RigidBody* pRigid0 = (RigidBody*)pBody0->getUserPointer();
	RigidBody* pRigid1 = (RigidBody*)pBody1->getUserPointer();

	//! @TODO 問題がわからないため今後修正
	//	とりあえずの処理
	//	消したあとのオブジェクトらしきものが0xfeeefeeeになっているので
	//	ないものとしてnullにする
	//pRigid0 = u32(pRigid0) == 0xfeeefeee ? nullptr : pRigid0;
	//pRigid1 = u32(pRigid1) == 0xfeeefeee ? nullptr : pRigid1;

	//	接触処理関数を呼び出す
	if(pRigid0)
	{
		ContactInfo contact0 = { p, pRigid1 };
		pRigid0->AddContactCollision(contact0);
	}

	if(pRigid1)
	{
		ContactInfo contact1 = { p, pRigid0 };
		pRigid1->AddContactCollision(contact1);
	}

	return true;
}

//-----------------------------------------------------------------------------
//!	デバッグ表示設定
//!	@param	[in]	isDebugRender	表示フラグ
//-----------------------------------------------------------------------------
void CollisionManager::SetDebugRender(bool isDebugRender)
{
	_isDebugRender = isDebugRender;
}
