//=============================================================================
//!
//!	@file	CollisionManager.cpp
//!	@brief	�Փ˔���Ǘ�
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
