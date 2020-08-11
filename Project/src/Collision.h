//=============================================================================
//!
//!	@file	Collision.h
//!	@brief	あたり判定
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

class RigidBody;
//	http://bulletphysics.org/Bullet/BulletFull/classbtCollisionShape.html

//!	接触情報構造体
struct ContactInfo
{
	//!	コンストラクタ
	ContactInfo()
	: _point()
	, _pRigidBody(nullptr)
	{}

	//!	コンストラクタ
	//!	@param	[in]	point		接触点情報
	//!	@param	[in]	pRigidBody	接触コンポーネントポインタ
	ContactInfo(const btManifoldPoint& point, RigidBody* pRigidBody)
	: _point(point)
	, _pRigidBody(pRigidBody)
	{}

	//!	nullチェック
	//!	@retval	true なし
	//!	@retval	false あり
	bool IsEmpty()const{ return _pRigidBody == nullptr; }

	//!	nullチェック
	//!	@retval	true なし
	//!	@retval	false あり
	operator bool()const{ return !IsEmpty(); }

	btManifoldPoint _point;	//!< 接触点の情報
	RigidBody* _pRigidBody;	//!< 接触オブジェクトのリジッドコンポーネント
};

//!	コリジョングループ
enum COLLOSION_GROUP
{
	COLLOSION_GROUP_01 = (u16)1 << 0,
	COLLOSION_GROUP_02 = (u16)1 << 1,
	COLLOSION_GROUP_03 = (u16)1 << 2,
	COLLOSION_GROUP_04 = (u16)1 << 3,
	COLLOSION_GROUP_05 = (u16)1 << 4,
	COLLOSION_GROUP_06 = (u16)1 << 5,
	COLLOSION_GROUP_07 = (u16)1 << 6,
	COLLOSION_GROUP_08 = (u16)1 << 7,
	COLLOSION_GROUP_09 = (u16)1 << 8,
	COLLOSION_GROUP_10 = (u16)1 << 9,
	COLLOSION_GROUP_11 = (u16)1 << 10,
	COLLOSION_GROUP_12 = (u16)1 << 11,
	COLLOSION_GROUP_13 = (u16)1 << 12,
	COLLOSION_GROUP_14 = (u16)1 << 13,
	COLLOSION_GROUP_15 = (u16)1 << 14,
	COLLOSION_GROUP_16 = (u16)1 << 15,
//	COLLOSION_GROUP_17 = (u16)1 << 16,
//	COLLOSION_GROUP_18 = (u16)1 << 17,
//	COLLOSION_GROUP_19 = (u16)1 << 18,
//	COLLOSION_GROUP_20 = (u16)1 << 19,
//	COLLOSION_GROUP_21 = (u16)1 << 20,
//	COLLOSION_GROUP_22 = (u16)1 << 21,
//	COLLOSION_GROUP_23 = (u16)1 << 22,
//	COLLOSION_GROUP_24 = (u16)1 << 23,
//	COLLOSION_GROUP_25 = (u16)1 << 24,
//	COLLOSION_GROUP_26 = (u16)1 << 25,
//	COLLOSION_GROUP_27 = (u16)1 << 26,
//	COLLOSION_GROUP_28 = (u16)1 << 27,
//	COLLOSION_GROUP_29 = (u16)1 << 28,
//	COLLOSION_GROUP_30 = (u16)1 << 29,
//	COLLOSION_GROUP_31 = (u16)1 << 30,
//	COLLOSION_GROUP_32 = (u16)1 << 31,

	COLLOSION_GROUP_ALL = USHRT_MAX,

};

enum class COLLISION_ACTIVATE : s32
{
	STATE_ACTIVE_TAG			= ACTIVE_TAG,
	STATE_ISLAND_SLEEPING		= ISLAND_SLEEPING,
	STATE_WANTS_DEACTIVATION	= WANTS_DEACTIVATION,
	STATE_DISABLE_DEACTIVATION	= DISABLE_DEACTIVATION,
	STATE_DISABLE_SIMULATION	= DISABLE_SIMULATION,

	STATE_EMPTY					= -1,
};
enum COLLISION_FLAG
{
	STATIC_OBJECT						= btCollisionObject::CF_STATIC_OBJECT,
	KINEMATIC_OBJECT					= btCollisionObject::CF_KINEMATIC_OBJECT,
	NO_CONTACT_RESPONSE					= btCollisionObject::CF_NO_CONTACT_RESPONSE,
	CUSTOM_MATERIAL_CALLBACK			= btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK,
	CHARACTER_OBJECT					= btCollisionObject::CF_CHARACTER_OBJECT,
	DISABLE_VISUALIZE_OBJECT			= btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT,
	DISABLE_SPU_COLLISION_PROCESSING	= btCollisionObject::CF_DISABLE_SPU_COLLISION_PROCESSING,
};

//-----------------------------------------------------------------------------
//!	物理コンポーネント
//-----------------------------------------------------------------------------
class RigidBody : public ProcessBase
{
public:
	//!	形状
	enum class SHAPE
	{
		BOX,
		CAPSULE,
		CONE,
		CYLINDER,
		SPHERE,

		NONE
	};

	struct Desc
	{
		//!	コンストラクタ
		Desc()
		: _position(Vector3::ZERO)
		, _angle(Quaternion::IDENTITY)
		, _mass(0.f)
		, _shapeType(SHAPE::NONE)
		{
		}

		//!	コンストラクタ
		//!	@param	[in]	position	座標
		//!	@param	[in]	angle		姿勢
		//!	@param	[in]	mass		質量
		//!	@param	[in]	shapeType	形状タイプ
		Desc(const Vector3& position, const Quaternion& angle, f32 mass, SHAPE shapeType)
		: _position(position)
		, _angle(angle)
		, _mass(mass)
		, _shapeType(shapeType)
		{
		}

		Vector3	_position;		//!< 座標(local)
		Quaternion	_angle;			//!< 姿勢
		f32		_mass;			//!< 質量
		SHAPE	_shapeType;		//!< 形状タイプ

		//!	形状サイズ	形状ごとに共用体で保持
		union
		{
			struct
			{
				f32 x, y, z;
			}_box;				//!< box
			f32 _sphere;		//!< sphere
			struct
			{
				f32 radius;
				f32 height;
			}_capsule;			//!< capsule
			struct
			{
				f32 radius;
				f32 height;
			}_cone;				//!< cone
			struct
			{
				f32 x, y, z;
			}_cylinder;			//!< cylinder
		};
	};

public:
	friend class CollisionManager;


	//!	コンストラクタ
	RigidBody()
	: _pShape		(nullptr)
	, _pBody		(nullptr)
	{
		AddTag("RigidBody");
	}

	//!	コンストラクタ
	//!	@param	[in]	pParent	親
	//!	@param	[in]	desc	引数リスト
	RigidBody(GameObjectBase* pParent, const Desc& desc)
	: ProcessBase	(pParent)
	, _pShape		(nullptr)
	, _pBody		(nullptr)
	{
		AddTag("RigidBody");
		Init(desc);
	}

	//!	コンストラクタ
	//!	@param	[in]	pParent	親
	//!	@param	[in]	desc	引数リスト
	//!	@param	[in]	myGroup	自身のグループビット
	//!	@param	[in]	filter	接触するグループビット
	RigidBody(GameObjectBase* pParent, const Desc& desc, u16 myGroup, u16 filter)
	: ProcessBase	(pParent)
	, _pShape		(nullptr)
	, _pBody		(nullptr)
	{
		AddTag("RigidBody");
		Init(desc, myGroup,filter);
	}



	//!	デストラクタ
	virtual ~RigidBody();
	
	//!	初期化
	//!	@param	[in]	desc	引数リスト
	bool Init(const Desc& desc);

	//!	初期化
	//!	@param	[in]	desc	引数リスト
	//!	@param	[in]	myGroup	自身のグループビット
	//!	@param	[in]	filter	接触するグループビット
	bool Init(const Desc& desc, u16 myGroup, u16 filter);

	//!	更新
	void Update();
	//!	物理更新
	void RigidBodyUpdate();


	//!	接触時に呼ばれる関数
	//!	@param	[in]	contact	接触情報
	virtual void Contact(const ContactInfo& contact);

	//!	接触情報追加
	//!	@param	[in]	contact	接触情報
	void AddContactCollision(const ContactInfo& contact);

	//!	接触情報削除
	void ResetContactCollision();

	//!	接触情報タグ検索
	//!	@param	[in]	tag		タグ
	//!	@return	検索結果の接触情報
	ContactInfo SearchContactTag(const std::string& tag)const;

	//!	接触情報タグ検索
	//!	@param	[in]	tags	タグ
	//!	@return	検索結果の接触情報
	ContactInfo SearchContactTag(const Tags& tags)const;

	//!	反発
	//!	@param	[in]	restitution	反発係数
	void SetRestitution(f32 restitution);

	//!	摩擦
	//!	@param	[in]	friction	摩擦係数
	void SetFriction(f32 friction);

	//!	物理回転
	//!	@param	[in]	torque	回転方向
	void AddTorque(const Vector3& torque);

	//!	物理移動
	//!	@param	[in]	force	移動ベクトル
	void SetForce(const Vector3& force);

	//!	物理移動
	//!	@param	[in]	force	移動ベクトル
	void AddForce(const Vector3& force);

	//!	重力
	//!	@param	[in]	gravity	重力ベクトル
	void SetGravity(const Vector3& gravity);

	//!	反発
	//!	@return	反発係数
	f32 GetRestitution()const;
	//!	摩擦
	//!	@return	摩擦係数
	f32 GetFriction()const;
	//!	物理回転
	//!	@return	物理回転方向
	Vector3 GetTorque()const;
	//!	物理移動
	//!	@return	物理移動ベクトル
	Vector3 GetForce()const;
	//!	重力
	//!	@return	重力ベクトル
	Vector3 GetGravity()const;

	//!	物理移動リセット
	void ClearForceTorque();

	Vector3 GetLinearVelocity()const;

	//!	当たり判定
	//!	@retval	true	あたり
	bool IsContact()const
	{
		return !_contacts.empty();
	}

	//!	接触情報取得
	//!	@return	接触情報の配列
	std::vector<ContactInfo> GetContacts()const
	{
		return _contacts;
	}

	//!	Bulletの物理オブジェクト
	//!	@return	Bulletの物理オブジェクト
	btRigidBody* GetbtRigidBody()
	{
		return _pBody;
	}

	//!	アクティブ化
	//!	@param	[in]	forceActivation	強制アクティブ	default : false
	void Activate(bool forceActivation = false) const
	{
		if (_pBody)
		{
			_pBody->activate(forceActivation);
		}
	}

	//!	コリジョン状態の設定
	//!	@param	[in]	flags
	void SetCollisionFlags(s32 flags)
	{
		_pBody->setCollisionFlags(flags);
	}

	//!	コリジョン状態の取得
	//!	@return	コリジョンフラグ
	s32 GetCollisionFlags()const
	{
		return _pBody->getCollisionFlags();
	}

	//!	ステート設定
	//!	@param	[in]	newState	ステート
	void SetActivationState(COLLISION_ACTIVATE newState)
	{
		if (_pBody)
		{
			_pBody->setActivationState((s32)newState);
		}
	}

	//!	ステート設定(強制)
	//!	@param	[in]	newState	ステート
	void SetForceActivationState(COLLISION_ACTIVATE newState)
	{
		if (_pBody)
		{
			_pBody->forceActivationState((s32)newState);
		}
	}

	//!	ステート取得
	//!	@return	ステート
	COLLISION_ACTIVATE GetActivationState()const
	{
		if (_pBody)
		{
			return (COLLISION_ACTIVATE)_pBody->getActivationState();
		}
		return COLLISION_ACTIVATE::STATE_EMPTY;
	}

	//!	質量設定
	//!	@param	[in]	mass	質量
	void SetMass(f32 mass)
	{
		if (_pBody)
		{
			btVector3 inertia;
			_pBody->getCollisionShape()->calculateLocalInertia(mass, inertia);
			_pBody->setMassProps(mass, inertia);
		}
	}

private:
	//!	初期化
	//!	@param	[in]	desc	引数リスト
	bool InitBulletObject(const Desc& desc);


	btCollisionShape*	_pShape;		//!< 形状
	btRigidBody*		_pBody;			//!< 物理

//	Vector3				_centerOffset;	//!< コリジョン座標オフセット

	std::vector<ContactInfo> _contacts;	//!< 接触情報
};


//-----------------------------------------------------------------------------
//!	物理オブジェクト
//-----------------------------------------------------------------------------
class CollisionObject : public GameObjectBase
{
public:
	friend RigidBody;
	//!	コンストラクタ
	CollisionObject();
	//!	デストラクタ
	virtual ~CollisionObject();

	//!	物理設定
	//!	@param	[in]	desc	引数リスト
	//!	@param	[in]	tag		コンポーネント追加タグ	default:""
	bool SetRigidBody(const RigidBody::Desc& desc, const std::string& tag = "");
	//!	物理設定
	//!	@param	[in]	desc	引数リスト
	//!	@param	[in]	myGroup	自身のグループビット
	//!	@param	[in]	filter	接触するグループビット
	//!	@param	[in]	tag		コンポーネント追加タグ	default:""
	bool SetRigidBody(const RigidBody::Desc& desc, u16 myGroup, u16 filter, const std::string& tag = "");

	//!	接触関数
	//!	@param	[in]	contact	接触情報
	virtual void Contact(const ContactInfo& contact){ UNUSED(contact); }

	//!	物理コンポーネント取得
	//!	@return	物理コンポーネント
	RigidBody* GetRigidBody()
	{
		return _pRigidBody.get();
	}

	//!	反発
	//!	@param	[in]	restitution	反発係数
	void SetRestitution(f32 restitution)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetRestitution(restitution);
	}

	//!	摩擦
	//!	@param	[in]	friction	摩擦係数
	void SetFriction(f32 friction)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetFriction(friction);
	}
	//!	物理移動
	//!	@param	[in]	force	移動ベクトル
	void SetForce(const Vector3& force)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetForce(force);
	}
	//!	物理移動
	//!	@param	[in]	force	移動ベクトル
	void AddForce(const Vector3& force)
	{
		if (!_pRigidBody)return;
		_pRigidBody->AddForce(force);
	}
	//!	物理回転
	//!	@param	[in]	torque	物理回転
	void AddTorque(const Vector3& torque)
	{
		if (!_pRigidBody)return;
		_pRigidBody->AddTorque(torque);
	}
	//!	重力
	//!	@param	[in]	gravity	重力ベクトル
	void SetGravity(const Vector3& gravity)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetGravity(gravity);
	}

	//!	ステート設定
	//!	@param	[in]	newState	ステート
	void SetActivationState(COLLISION_ACTIVATE newState)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetActivationState(newState);
	}

	//!	ステート設定(強制)
	//!	@param	[in]	newState	ステート
	void SetForceActivationState(COLLISION_ACTIVATE newState)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetForceActivationState(newState);
	}

	//!	コリジョン状態の設定
	//!	@param	[in]	flags	btCollisionObject::CF系
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

	//!	反発
	//!	@return	反発係数
	f32 GetRestitution()const
	{
		if (!_pRigidBody)return 0.f;
		return _pRigidBody->GetRestitution();
	}
	//!	摩擦
	//!	@return	摩擦係数
	f32 GetFriction()const
	{
		if (!_pRigidBody)return 0.f;
		return _pRigidBody->GetFriction();
	}
	//!	物理移動
	//!	@return	物理移動ベクトル
	Vector3 GetForce()const
	{
		if (!_pRigidBody)return Vector3::ZERO;
		return _pRigidBody->GetForce();
	}
	//!	物理回転
	//!	@return	物理回転方向
	Vector3 GetTorque()const
	{
		if (!_pRigidBody)return Vector3::ZERO;
		return _pRigidBody->GetTorque();
	}
	//!	重力
	//!	@return	重力ベクトル
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

	//!	ステート取得
	//!	@return	ステート
	COLLISION_ACTIVATE GetActivationState()const
	{
		if (!_pRigidBody)return COLLISION_ACTIVATE::STATE_EMPTY;
		_pRigidBody->GetActivationState();
	}
	//!	コリジョン状態の取得
	//!	@return	btCollisionObject::CF系
	s32 GetCollisionFlags()const
	{
		if (!_pRigidBody)return 0;
		return _pRigidBody->GetCollisionFlags();
	}


	//!	アクティブ化
	//!	@param	[in]	forceActivation	強制アクティブ	default : false
	void Activate(bool forceActivation = false) const
	{
		if (!_pRigidBody)return;
		_pRigidBody->Activate(forceActivation);
	}

	//!	当たり判定
	//!	@retval	true	あたり
	bool IsHit()const
	{
		return _pRigidBody ? _pRigidBody->IsContact() : false;
	}

	//!	物理削除
	void ResetRigidBody();

protected:
	RefPointer<RigidBody> _pRigidBody;
};

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