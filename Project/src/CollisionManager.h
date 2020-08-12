//=============================================================================
//!
//!	@file	CollisionManager.h
//!	@brief	�Փ˔���Ǘ�
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

//	http://bulletphysics.org/Bullet/BulletFull/classbtCollisionShape.html

//-----------------------------------------------------------------------------
//!	Bullet�Ǘ��N���X
//-----------------------------------------------------------------------------
class CollisionManager : public ManagerBase
{
public:
	//! @brief  �R���X�g���N�^
	CollisionManager();

	//! @brief  �f�X�g���N�^
	virtual ~CollisionManager();

	//! @brief  ������
	void Init();

	//! @brief  �X�V
	void Update();

	//! @brief  �`��
	void Render();

	//! @brief  �I��
	void Exit();

	//! @brief  �Փˌ��o�̂��߂̃C���^�t�F�[�X
	btDiscreteDynamicsWorld* GetDynamicsWorld();

	//! @brief  �����R���W�����ǉ�
	//!	@param	[in]	pBody	�����R���|�[�l���g
	void AddRigidBody(RigidBody* pBody);

	//! @brief  �����R���W�����ǉ�
	//!	@param	[in]	pBody	�����R���|�[�l���g
	//!	@param	[in]	myGroup	���g�̃O���[�v�r�b�g
	//!	@param	[in]	filter	�ڐG����O���[�v�r�b�g
	void AddRigidBody(RigidBody* pBody, u16 myGroup, u16 filter);

	//! @brief  �����R���W�����폜
	//!	@param	[in]	pBody	�����R���|�[�l���g
	void RemoveRigidBody(RigidBody* pBody);

	//! @brief  �O���[�o���ȏd�͐ݒ�
	//!	@param	[in]	gravity	�d�̓x�N�g��
	void SetGravity(const Vector3& gravity);

	//! @brief  �f�o�b�O�\���ݒ�
	//!	@param	[in]	isDebugRender	�\���t���O
	void SetDebugRender(bool isDebugRender);
	
	//! @brief  �f�o�b�O�\���擾
	//!	@return �\���t���O
	bool GetDebugRender()const;

	//! @brief  �R���W�����R���^�N�g�̃R�[���o�b�N�֐�
	static bool ContactProcessed(btManifoldPoint& p, void* a, void* b);

	//! @brief  �����X�V�t���O�ݒ�
	//!	@param	[in]	�X�V�t���O
	void SetUpdateFlag(bool isUpdate);

	//! @brief  �X�V�t���O
	//!	@retval	false	�����̍X�V�����Ȃ�
	bool IsUpdate()const;

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

// �}�l�[�W���擾�֐�����
MANAGER_INTERFACE(CollisionManager, Collision);
