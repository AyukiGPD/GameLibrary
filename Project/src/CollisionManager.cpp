//=============================================================================
//!
//!	@file	CollisionManager.cpp
//!	@brief	衝突判定管理
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
