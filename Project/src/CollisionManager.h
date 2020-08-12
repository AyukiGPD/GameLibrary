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
	//! @brief  コンストラクタ
	CollisionManager();

	//! @brief  デストラクタ
	virtual ~CollisionManager();

	//! @brief  初期化
	void Init();

	//! @brief  更新
	void Update();

	//! @brief  描画
	void Render();

	//! @brief  終了
	void Exit();

	//! @brief  衝突検出のためのインタフェース
	btDiscreteDynamicsWorld* GetDynamicsWorld();

	//! @brief  物理コリジョン追加
	//!	@param	[in]	pBody	物理コンポーネント
	void AddRigidBody(RigidBody* pBody);

	//! @brief  物理コリジョン追加
	//!	@param	[in]	pBody	物理コンポーネント
	//!	@param	[in]	myGroup	自身のグループビット
	//!	@param	[in]	filter	接触するグループビット
	void AddRigidBody(RigidBody* pBody, u16 myGroup, u16 filter);

	//! @brief  物理コリジョン削除
	//!	@param	[in]	pBody	物理コンポーネント
	void RemoveRigidBody(RigidBody* pBody);

	//! @brief  グローバルな重力設定
	//!	@param	[in]	gravity	重力ベクトル
	void SetGravity(const Vector3& gravity);

	//! @brief  デバッグ表示設定
	//!	@param	[in]	isDebugRender	表示フラグ
	void SetDebugRender(bool isDebugRender);
	
	//! @brief  デバッグ表示取得
	//!	@return 表示フラグ
	bool GetDebugRender()const;

	//! @brief  コリジョンコンタクトのコールバック関数
	static bool ContactProcessed(btManifoldPoint& p, void* a, void* b);

	//! @brief  物理更新フラグ設定
	//!	@param	[in]	更新フラグ
	void SetUpdateFlag(bool isUpdate);

	//! @brief  更新フラグ
	//!	@retval	false	物理の更新をしない
	bool IsUpdate()const;

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

// マネージャ取得関数生成
MANAGER_INTERFACE(CollisionManager, Collision);
