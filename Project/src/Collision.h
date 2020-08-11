//=============================================================================
//!
//!	@file	Collision.h
//!	@brief	�����蔻��
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once
/*
	bullet�Ɋւ���N���X��
	�I�[�o�[���C�h���ꂽnew���g���Ă͂����Ȃ�

	�w�b�_�[���ł�new���g��Ȃ�
*/

class RigidBody;
//	http://bulletphysics.org/Bullet/BulletFull/classbtCollisionShape.html

//!	�ڐG���\����
struct ContactInfo
{
	//!	�R���X�g���N�^
	ContactInfo()
	: _point()
	, _pRigidBody(nullptr)
	{}

	//!	�R���X�g���N�^
	//!	@param	[in]	point		�ڐG�_���
	//!	@param	[in]	pRigidBody	�ڐG�R���|�[�l���g�|�C���^
	ContactInfo(const btManifoldPoint& point, RigidBody* pRigidBody)
	: _point(point)
	, _pRigidBody(pRigidBody)
	{}

	//!	null�`�F�b�N
	//!	@retval	true �Ȃ�
	//!	@retval	false ����
	bool IsEmpty()const{ return _pRigidBody == nullptr; }

	//!	null�`�F�b�N
	//!	@retval	true �Ȃ�
	//!	@retval	false ����
	operator bool()const{ return !IsEmpty(); }

	btManifoldPoint _point;	//!< �ڐG�_�̏��
	RigidBody* _pRigidBody;	//!< �ڐG�I�u�W�F�N�g�̃��W�b�h�R���|�[�l���g
};

//!	�R���W�����O���[�v
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
//!	�����R���|�[�l���g
//-----------------------------------------------------------------------------
class RigidBody : public ProcessBase
{
public:
	//!	�`��
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
		//!	�R���X�g���N�^
		Desc()
		: _position(Vector3::ZERO)
		, _angle(Quaternion::IDENTITY)
		, _mass(0.f)
		, _shapeType(SHAPE::NONE)
		{
		}

		//!	�R���X�g���N�^
		//!	@param	[in]	position	���W
		//!	@param	[in]	angle		�p��
		//!	@param	[in]	mass		����
		//!	@param	[in]	shapeType	�`��^�C�v
		Desc(const Vector3& position, const Quaternion& angle, f32 mass, SHAPE shapeType)
		: _position(position)
		, _angle(angle)
		, _mass(mass)
		, _shapeType(shapeType)
		{
		}

		Vector3	_position;		//!< ���W(local)
		Quaternion	_angle;			//!< �p��
		f32		_mass;			//!< ����
		SHAPE	_shapeType;		//!< �`��^�C�v

		//!	�`��T�C�Y	�`�󂲂Ƃɋ��p�̂ŕێ�
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


	//!	�R���X�g���N�^
	RigidBody()
	: _pShape		(nullptr)
	, _pBody		(nullptr)
	{
		AddTag("RigidBody");
	}

	//!	�R���X�g���N�^
	//!	@param	[in]	pParent	�e
	//!	@param	[in]	desc	�������X�g
	RigidBody(GameObjectBase* pParent, const Desc& desc)
	: ProcessBase	(pParent)
	, _pShape		(nullptr)
	, _pBody		(nullptr)
	{
		AddTag("RigidBody");
		Init(desc);
	}

	//!	�R���X�g���N�^
	//!	@param	[in]	pParent	�e
	//!	@param	[in]	desc	�������X�g
	//!	@param	[in]	myGroup	���g�̃O���[�v�r�b�g
	//!	@param	[in]	filter	�ڐG����O���[�v�r�b�g
	RigidBody(GameObjectBase* pParent, const Desc& desc, u16 myGroup, u16 filter)
	: ProcessBase	(pParent)
	, _pShape		(nullptr)
	, _pBody		(nullptr)
	{
		AddTag("RigidBody");
		Init(desc, myGroup,filter);
	}



	//!	�f�X�g���N�^
	virtual ~RigidBody();
	
	//!	������
	//!	@param	[in]	desc	�������X�g
	bool Init(const Desc& desc);

	//!	������
	//!	@param	[in]	desc	�������X�g
	//!	@param	[in]	myGroup	���g�̃O���[�v�r�b�g
	//!	@param	[in]	filter	�ڐG����O���[�v�r�b�g
	bool Init(const Desc& desc, u16 myGroup, u16 filter);

	//!	�X�V
	void Update();
	//!	�����X�V
	void RigidBodyUpdate();


	//!	�ڐG���ɌĂ΂��֐�
	//!	@param	[in]	contact	�ڐG���
	virtual void Contact(const ContactInfo& contact);

	//!	�ڐG���ǉ�
	//!	@param	[in]	contact	�ڐG���
	void AddContactCollision(const ContactInfo& contact);

	//!	�ڐG���폜
	void ResetContactCollision();

	//!	�ڐG���^�O����
	//!	@param	[in]	tag		�^�O
	//!	@return	�������ʂ̐ڐG���
	ContactInfo SearchContactTag(const std::string& tag)const;

	//!	�ڐG���^�O����
	//!	@param	[in]	tags	�^�O
	//!	@return	�������ʂ̐ڐG���
	ContactInfo SearchContactTag(const Tags& tags)const;

	//!	����
	//!	@param	[in]	restitution	�����W��
	void SetRestitution(f32 restitution);

	//!	���C
	//!	@param	[in]	friction	���C�W��
	void SetFriction(f32 friction);

	//!	������]
	//!	@param	[in]	torque	��]����
	void AddTorque(const Vector3& torque);

	//!	�����ړ�
	//!	@param	[in]	force	�ړ��x�N�g��
	void SetForce(const Vector3& force);

	//!	�����ړ�
	//!	@param	[in]	force	�ړ��x�N�g��
	void AddForce(const Vector3& force);

	//!	�d��
	//!	@param	[in]	gravity	�d�̓x�N�g��
	void SetGravity(const Vector3& gravity);

	//!	����
	//!	@return	�����W��
	f32 GetRestitution()const;
	//!	���C
	//!	@return	���C�W��
	f32 GetFriction()const;
	//!	������]
	//!	@return	������]����
	Vector3 GetTorque()const;
	//!	�����ړ�
	//!	@return	�����ړ��x�N�g��
	Vector3 GetForce()const;
	//!	�d��
	//!	@return	�d�̓x�N�g��
	Vector3 GetGravity()const;

	//!	�����ړ����Z�b�g
	void ClearForceTorque();

	Vector3 GetLinearVelocity()const;

	//!	�����蔻��
	//!	@retval	true	������
	bool IsContact()const
	{
		return !_contacts.empty();
	}

	//!	�ڐG���擾
	//!	@return	�ڐG���̔z��
	std::vector<ContactInfo> GetContacts()const
	{
		return _contacts;
	}

	//!	Bullet�̕����I�u�W�F�N�g
	//!	@return	Bullet�̕����I�u�W�F�N�g
	btRigidBody* GetbtRigidBody()
	{
		return _pBody;
	}

	//!	�A�N�e�B�u��
	//!	@param	[in]	forceActivation	�����A�N�e�B�u	default : false
	void Activate(bool forceActivation = false) const
	{
		if (_pBody)
		{
			_pBody->activate(forceActivation);
		}
	}

	//!	�R���W������Ԃ̐ݒ�
	//!	@param	[in]	flags
	void SetCollisionFlags(s32 flags)
	{
		_pBody->setCollisionFlags(flags);
	}

	//!	�R���W������Ԃ̎擾
	//!	@return	�R���W�����t���O
	s32 GetCollisionFlags()const
	{
		return _pBody->getCollisionFlags();
	}

	//!	�X�e�[�g�ݒ�
	//!	@param	[in]	newState	�X�e�[�g
	void SetActivationState(COLLISION_ACTIVATE newState)
	{
		if (_pBody)
		{
			_pBody->setActivationState((s32)newState);
		}
	}

	//!	�X�e�[�g�ݒ�(����)
	//!	@param	[in]	newState	�X�e�[�g
	void SetForceActivationState(COLLISION_ACTIVATE newState)
	{
		if (_pBody)
		{
			_pBody->forceActivationState((s32)newState);
		}
	}

	//!	�X�e�[�g�擾
	//!	@return	�X�e�[�g
	COLLISION_ACTIVATE GetActivationState()const
	{
		if (_pBody)
		{
			return (COLLISION_ACTIVATE)_pBody->getActivationState();
		}
		return COLLISION_ACTIVATE::STATE_EMPTY;
	}

	//!	���ʐݒ�
	//!	@param	[in]	mass	����
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
	//!	������
	//!	@param	[in]	desc	�������X�g
	bool InitBulletObject(const Desc& desc);


	btCollisionShape*	_pShape;		//!< �`��
	btRigidBody*		_pBody;			//!< ����

//	Vector3				_centerOffset;	//!< �R���W�������W�I�t�Z�b�g

	std::vector<ContactInfo> _contacts;	//!< �ڐG���
};


//-----------------------------------------------------------------------------
//!	�����I�u�W�F�N�g
//-----------------------------------------------------------------------------
class CollisionObject : public GameObjectBase
{
public:
	friend RigidBody;
	//!	�R���X�g���N�^
	CollisionObject();
	//!	�f�X�g���N�^
	virtual ~CollisionObject();

	//!	�����ݒ�
	//!	@param	[in]	desc	�������X�g
	//!	@param	[in]	tag		�R���|�[�l���g�ǉ��^�O	default:""
	bool SetRigidBody(const RigidBody::Desc& desc, const std::string& tag = "");
	//!	�����ݒ�
	//!	@param	[in]	desc	�������X�g
	//!	@param	[in]	myGroup	���g�̃O���[�v�r�b�g
	//!	@param	[in]	filter	�ڐG����O���[�v�r�b�g
	//!	@param	[in]	tag		�R���|�[�l���g�ǉ��^�O	default:""
	bool SetRigidBody(const RigidBody::Desc& desc, u16 myGroup, u16 filter, const std::string& tag = "");

	//!	�ڐG�֐�
	//!	@param	[in]	contact	�ڐG���
	virtual void Contact(const ContactInfo& contact){ UNUSED(contact); }

	//!	�����R���|�[�l���g�擾
	//!	@return	�����R���|�[�l���g
	RigidBody* GetRigidBody()
	{
		return _pRigidBody.get();
	}

	//!	����
	//!	@param	[in]	restitution	�����W��
	void SetRestitution(f32 restitution)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetRestitution(restitution);
	}

	//!	���C
	//!	@param	[in]	friction	���C�W��
	void SetFriction(f32 friction)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetFriction(friction);
	}
	//!	�����ړ�
	//!	@param	[in]	force	�ړ��x�N�g��
	void SetForce(const Vector3& force)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetForce(force);
	}
	//!	�����ړ�
	//!	@param	[in]	force	�ړ��x�N�g��
	void AddForce(const Vector3& force)
	{
		if (!_pRigidBody)return;
		_pRigidBody->AddForce(force);
	}
	//!	������]
	//!	@param	[in]	torque	������]
	void AddTorque(const Vector3& torque)
	{
		if (!_pRigidBody)return;
		_pRigidBody->AddTorque(torque);
	}
	//!	�d��
	//!	@param	[in]	gravity	�d�̓x�N�g��
	void SetGravity(const Vector3& gravity)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetGravity(gravity);
	}

	//!	�X�e�[�g�ݒ�
	//!	@param	[in]	newState	�X�e�[�g
	void SetActivationState(COLLISION_ACTIVATE newState)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetActivationState(newState);
	}

	//!	�X�e�[�g�ݒ�(����)
	//!	@param	[in]	newState	�X�e�[�g
	void SetForceActivationState(COLLISION_ACTIVATE newState)
	{
		if (!_pRigidBody)return;
		_pRigidBody->SetForceActivationState(newState);
	}

	//!	�R���W������Ԃ̐ݒ�
	//!	@param	[in]	flags	btCollisionObject::CF�n
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

	//!	����
	//!	@return	�����W��
	f32 GetRestitution()const
	{
		if (!_pRigidBody)return 0.f;
		return _pRigidBody->GetRestitution();
	}
	//!	���C
	//!	@return	���C�W��
	f32 GetFriction()const
	{
		if (!_pRigidBody)return 0.f;
		return _pRigidBody->GetFriction();
	}
	//!	�����ړ�
	//!	@return	�����ړ��x�N�g��
	Vector3 GetForce()const
	{
		if (!_pRigidBody)return Vector3::ZERO;
		return _pRigidBody->GetForce();
	}
	//!	������]
	//!	@return	������]����
	Vector3 GetTorque()const
	{
		if (!_pRigidBody)return Vector3::ZERO;
		return _pRigidBody->GetTorque();
	}
	//!	�d��
	//!	@return	�d�̓x�N�g��
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

	//!	�X�e�[�g�擾
	//!	@return	�X�e�[�g
	COLLISION_ACTIVATE GetActivationState()const
	{
		if (!_pRigidBody)return COLLISION_ACTIVATE::STATE_EMPTY;
		_pRigidBody->GetActivationState();
	}
	//!	�R���W������Ԃ̎擾
	//!	@return	btCollisionObject::CF�n
	s32 GetCollisionFlags()const
	{
		if (!_pRigidBody)return 0;
		return _pRigidBody->GetCollisionFlags();
	}


	//!	�A�N�e�B�u��
	//!	@param	[in]	forceActivation	�����A�N�e�B�u	default : false
	void Activate(bool forceActivation = false) const
	{
		if (!_pRigidBody)return;
		_pRigidBody->Activate(forceActivation);
	}

	//!	�����蔻��
	//!	@retval	true	������
	bool IsHit()const
	{
		return _pRigidBody ? _pRigidBody->IsContact() : false;
	}

	//!	�����폜
	void ResetRigidBody();

protected:
	RefPointer<RigidBody> _pRigidBody;
};

//-----------------------------------------------------------------------------
//!	Bullet�Ǘ��N���X
//-----------------------------------------------------------------------------
class CollisionManager : public ManagerBase
{
public:
	//!	�R���X�g���N�^
	CollisionManager();
	//!	�f�X�g���N�^
	virtual ~CollisionManager();

	//!	������
	void Init();
	//!	�X�V
	void Update();
	//!	�`��
	void Render();
	//!	�I��
	void Exit();

	btDiscreteDynamicsWorld* GetDynamicsWorld(){ return _pDynamicsWorld; }

	//!	�����R���W�����ǉ�
	//!	@param	[in]	pBody	�����R���|�[�l���g
	void AddRigidBody(RigidBody* pBody);

	//!	�����R���W�����ǉ�
	//!	@param	[in]	pBody	�����R���|�[�l���g
	//!	@param	[in]	myGroup	���g�̃O���[�v�r�b�g
	//!	@param	[in]	filter	�ڐG����O���[�v�r�b�g
	void AddRigidBody(RigidBody* pBody, u16 myGroup, u16 filter);

	//!	�����R���W�����폜
	//!	@param	[in]	pBody	�����R���|�[�l���g
	void RemoveRigidBody(RigidBody* pBody);

	//!	�O���[�o���ȏd�͐ݒ�
	//!	@param	[in]	gravity	�d�̓x�N�g��
	void SetGravity(const Vector3& gravity);

	//!	�f�o�b�O�\���ݒ�
	//!	@param	[in]	isDebugRender	�\���t���O
	void SetDebugRender(bool isDebugRender);
	//!	�f�o�b�O�\���擾
	//!	@return �\���t���O
	bool GetDebugRender()const{ return _isDebugRender; }

	//!	�R���W�����R���^�N�g�̃R�[���o�b�N�֐�
	static bool ContactProcessed(btManifoldPoint& p, void* a, void* b);

	//!	�����X�V�t���O�ݒ�
	//!	@param	[in]	�X�V�t���O
	void SetUpdateFlag(bool isUpdate)
	{
		_isUpdate = isUpdate;
	}

	//!	�X�V�t���O
	//!	@retval	false	�����̍X�V�����Ȃ�
	bool IsUpdate()const
	{
		return _isUpdate;
	}

private:
	btDefaultCollisionConfiguration*	 _pCollisionConfiguration;	//!< �Փˌ��o�X�^�b�N�A���P�[�^
	btCollisionDispatcher*				 _pDispatcher;				//!< �����x�̏Փˌ��m(�i���[�t�F�[�Y)
	btBroadphaseInterface*				 _pBroadphase;				//!< �ᐸ�x�̏Փˌ��m(�u���[�h�t�F�[�Y)
	btSequentialImpulseConstraintSolver* _pSolver;					//!< �W���C���g���Ǘ�
	btDiscreteDynamicsWorld*			 _pDynamicsWorld;			//!< �Փˌ��o�̂��߂̃C���^�t�F�[�X�ƃR���e(���[���h)
	btIDebugDraw*						 _pDebugDraw;				//!< �f�o�b�O�`��I�u�W�F�N�g

	std::vector<RigidBody*>				 _pRigidBodies;				//!< ���W�b�h�{�f�B

	bool _isUpdate;			//!< �X�V�t���O
	bool _isDebugRender;	//!< �f�o�b�O�`��t���O

};

MANAGER_INTERFACE(CollisionManager, Collision);