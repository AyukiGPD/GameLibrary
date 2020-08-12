//=============================================================================
//!
//!	@file	CollisionManager.h
//!	@brief	衝突判定管理
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once
/*
	bulletに関するクラスで
	オーバーライドされたnewを使ってはいけない

	ヘッダー内ではnewを使わない
*/

//	http://bulletphysics.org/Bullet/BulletFull/classbtCollisionShape.html

//-----------------------------------------------------------------------------
//!	Bullet管理クラス
//-----------------------------------------------------------------------------
class CollisionManager : public ManagerBase
{
public:
	//!	コンストラクタ
	CollisionManager();
	//!	デストラクタ
	virtual ~CollisionManager();

	//!	初期化
	void Init();
	//!	更新
	void Update();
	//!	描画
	void Render();
	//!	終了
	void Exit();

	btDiscreteDynamicsWorld* GetDynamicsWorld(){ return _pDynamicsWorld; }

	//!	物理コリジョン追加
	//!	@param	[in]	pBody	物理コンポーネント
	void AddRigidBody(RigidBody* pBody);

	//!	物理コリジョン追加
	//!	@param	[in]	pBody	物理コンポーネント
	//!	@param	[in]	myGroup	自身のグループビット
	//!	@param	[in]	filter	接触するグループビット
	void AddRigidBody(RigidBody* pBody, u16 myGroup, u16 filter);

	//!	物理コリジョン削除
	//!	@param	[in]	pBody	物理コンポーネント
	void RemoveRigidBody(RigidBody* pBody);

	//!	グローバルな重力設定
	//!	@param	[in]	gravity	重力ベクトル
	void SetGravity(const Vector3& gravity);

	//!	デバッグ表示設定
	//!	@param	[in]	isDebugRender	表示フラグ
	void SetDebugRender(bool isDebugRender);
	//!	デバッグ表示取得
	//!	@return 表示フラグ
	bool GetDebugRender()const{ return _isDebugRender; }

	//!	コリジョンコンタクトのコールバック関数
	static bool ContactProcessed(btManifoldPoint& p, void* a, void* b);

	//!	物理更新フラグ設定
	//!	@param	[in]	更新フラグ
	void SetUpdateFlag(bool isUpdate)
	{
		_isUpdate = isUpdate;
	}

	//!	更新フラグ
	//!	@retval	false	物理の更新をしない
	bool IsUpdate()const
	{
		return _isUpdate;
	}

private:
	btDefaultCollisionConfiguration*	 _pCollisionConfiguration;	//!< 衝突検出スタックアロケータ
	btCollisionDispatcher*				 _pDispatcher;				//!< 高精度の衝突検知(ナローフェーズ)
	btBroadphaseInterface*				 _pBroadphase;				//!< 低精度の衝突検知(ブロードフェーズ)
	btSequentialImpulseConstraintSolver* _pSolver;					//!< ジョイント等管理
	btDiscreteDynamicsWorld*			 _pDynamicsWorld;			//!< 衝突検出のためのインタフェースとコンテ(ワールド)
	btIDebugDraw*						 _pDebugDraw;				//!< デバッグ描画オブジェクト

	std::vector<RigidBody*>				 _pRigidBodies;				//!< リジッドボディ

	bool _isUpdate;			//!< 更新フラグ
	bool _isDebugRender;	//!< デバッグ描画フラグ

};

MANAGER_INTERFACE(CollisionManager, Collision);
