//=============================================================================
//!
//!	@file	Collision.cpp
//!	@brief	�����蔻��
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
//!	�f�o�b�O�`��N���X
//=============================================================================
class MyBulletDebugDraw : public btIDebugDraw
{
private:
	//�f�o�b�O���[�h
	s32 m_DebugMode;
public:
	//�R�X�g���N�^
	MyBulletDebugDraw()
		: m_DebugMode(1)
	{}

	//	���C���̕`��
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		Color lineColor;
		lineColor.SetFloat(color.x(), color.y(), color.z());
		Render3D::Line(from, to, lineColor);
	}

	//	���C���̕`��
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
	{
		Color fromLineColor;
		Color toLineColor;
		fromLineColor.SetFloat(fromColor.x(), fromColor.y(), fromColor.z());
		toLineColor.SetFloat(toColor.x(), toColor.y(), toColor.z());
		Render3D::Line(from, to, fromLineColor, toLineColor);
	}


	//	�ڐG�_�̕`��
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
	{
		UNUSED(lifeTime);
		Vector3 to = PointOnB + normalOnB * distance;
		const Vector3& from = PointOnB;
		Color lineColor;
		lineColor.SetFloat(color.x(), color.y(), color.z());
		Render3D::Line(from, to, lineColor);
	}

	//	�G���[�̕\��
	void reportErrorWarning(const char* warningString)
	{
		DEBUG_LOG(warningString);
	}

	//	������
	//	�R�c�e�L�X�g�̕`��
	void draw3dText(const btVector3& location, const char* textString)
	{
		UNUSED(location);
		UNUSED(textString);
	}

	//	�f�o�b�O���[�h�̐ݒ�
	void setDebugMode(int debugMode)
	{
		m_DebugMode = debugMode;
	}

	//	�f�o�b�O���[�h�̎擾
	int getDebugMode() const
	{
		return m_DebugMode;
	}
};

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



//=============================================================================
//!	�����I�u�W�F�N�g
//=============================================================================
//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
CollisionObject::CollisionObject()
{
}

//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
CollisionObject::~CollisionObject()
{
}

//-----------------------------------------------------------------------------
//!	�����ݒ�
//!	@param	[in]	desc	�������X�g
//!	@param	[in]	tag		�R���|�[�l���g�ǉ��^�O	default:""
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
//!	�����ݒ�
//!	@param	[in]	desc	�������X�g
//!	@param	[in]	myGroup	���g�̃O���[�v�r�b�g
//!	@param	[in]	filter	�ڐG����O���[�v�r�b�g
//!	@param	[in]	tag		�R���|�[�l���g�ǉ��^�O	default:""
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
//!	�����폜
//-----------------------------------------------------------------------------
void CollisionObject::ResetRigidBody()
{
	ReleaseProcess(_pRigidBody.get());
	_pRigidBody.reset();
}

//=============================================================================
//!	Bullet�Ǘ��N���X
//=============================================================================
//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
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
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
CollisionManager::~CollisionManager()
{
	Exit();
}

//! �R���^�N�g�̃R�[���o�b�N
extern ContactProcessedCallback gContactProcessedCallback;

//-----------------------------------------------------------------------------
//!	������
//-----------------------------------------------------------------------------
void CollisionManager::Init()
{
	// �Փˌ��o�X�^�b�N�A���P�[�^
	_pCollisionConfiguration = new btDefaultCollisionConfiguration();
	// �����x�̏Փˌ��m(�i���[�t�F�[�Y)
	_pDispatcher = new btCollisionDispatcher(_pCollisionConfiguration);
	// �ᐸ�x�̏Փˌ��m(�u���[�h�t�F�[�Y)
	_pBroadphase = new btDbvtBroadphase();
	// �W���C���g���Ǘ�
	_pSolver = new btSequentialImpulseConstraintSolver;
	// �Փˌ��o�̂��߂̃C���^�t�F�[�X�ƃR���e(���[���h)
	_pDynamicsWorld = new btDiscreteDynamicsWorld(_pDispatcher, _pBroadphase, _pSolver, _pCollisionConfiguration);
	// Debug�`��ݒ�
	_pDebugDraw = new MyBulletDebugDraw();
	_pDynamicsWorld->setDebugDrawer(_pDebugDraw);
	// �d�͂̐ݒ�
	_pDynamicsWorld->setGravity(btVector3(0, -9.8f, 0));
	gContactProcessedCallback = CollisionManager::ContactProcessed;
}

//-----------------------------------------------------------------------------
//!	�X�V
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
//!	�`��
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
//!	�I��
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
//!	�����R���W�����ǉ�
//!	@param	[in]	pBody	�����R���|�[�l���g
//-----------------------------------------------------------------------------
void CollisionManager::AddRigidBody(RigidBody* pBody)
{
	if (!pBody) return;
	_pDynamicsWorld->addRigidBody(pBody->_pBody);
	_pRigidBodies.push_back(pBody);
}

//-----------------------------------------------------------------------------
//!	�����R���W�����ǉ�
//!	@param	[in]	pBody	�����R���|�[�l���g
//!	@param	[in]	myGroup	���g�̃O���[�v�r�b�g
//!	@param	[in]	filter	�ڐG����O���[�v�r�b�g
//-----------------------------------------------------------------------------
void CollisionManager::AddRigidBody(RigidBody* pBody, u16 myGroup, u16 filter)
{
	if (!pBody) return;
	_pDynamicsWorld->addRigidBody(pBody->_pBody, myGroup, filter);
	_pRigidBodies.push_back(pBody);
}

//-----------------------------------------------------------------------------
//!	�����R���W�����폜
//!	@param	[in]	pBody	�����R���|�[�l���g
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
//!	�O���[�o���ȏd�͐ݒ�
//!	@param	[in]	gravity	�d�̓x�N�g��
//-----------------------------------------------------------------------------
void CollisionManager::SetGravity(const Vector3& gravity)
{
	_pDynamicsWorld->setGravity(gravity);
}

//	@bug Contact�̐�Ŏ��E�������2�ڂ̃|�C���^����ɂȂ�
//-----------------------------------------------------------------------------
//!	�R���W�����R���^�N�g�̃R�[���o�b�N�֐�
//-----------------------------------------------------------------------------
bool CollisionManager::ContactProcessed(btManifoldPoint& p, void* a, void* b)
{
	btRigidBody* pBody0 = (btRigidBody*)a;
	btRigidBody* pBody1 = (btRigidBody*)b;

	RigidBody* pRigid0 = (RigidBody*)pBody0->getUserPointer();
	RigidBody* pRigid1 = (RigidBody*)pBody1->getUserPointer();

	//! @TODO ��肪�킩��Ȃ����ߍ���C��
	//	�Ƃ肠�����̏���
	//	���������Ƃ̃I�u�W�F�N�g�炵�����̂�0xfeeefeee�ɂȂ��Ă���̂�
	//	�Ȃ����̂Ƃ���null�ɂ���
	//pRigid0 = u32(pRigid0) == 0xfeeefeee ? nullptr : pRigid0;
	//pRigid1 = u32(pRigid1) == 0xfeeefeee ? nullptr : pRigid1;

	//	�ڐG�����֐����Ăяo��
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
//!	�f�o�b�O�\���ݒ�
//!	@param	[in]	isDebugRender	�\���t���O
//-----------------------------------------------------------------------------
void CollisionManager::SetDebugRender(bool isDebugRender)
{
	_isDebugRender = isDebugRender;
}
