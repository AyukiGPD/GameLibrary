//=============================================================================
//!
//!	@file	SceneBase.h
//!	@brief	�V�[���x�[�X
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

// TODO �L�[��}�E�X�Ȃǂ̃C�x���g�N���X������
//	�V�[���쐬���͂��̃N���X���p��������
//-------------------------------------------------------------------
//!	�V�[���x�[�X�N���X
//-------------------------------------------------------------------
class SceneBase
{
	friend class SceneManager;
public:
	//!	�R���X�g���N�^
	SceneBase();
	//!	�f�X�g���N�^
	virtual ~SceneBase();
	//!	������
	virtual void Init(){}
	//!	�X�V
	virtual void Update(){}
	//!	�`��
	virtual void Render(){}
	//!	�I��
	virtual void Exit(){}

	//!	�I�u�W�F�N�g�ǉ�
	//!	@param	[in]	pObject	�I�u�W�F�N�g�̃|�C���^
	void AddGameObject(GameObjectBase* pObject);

	//!	�I�u�W�F�N�g�폜
	//!	@param	[in]	pObject	�I�u�W�F�N�g�̃|�C���^
	//!	@return	�����̎Q�Ɛ�
	//!	@retval	0	�폜�ς�
	s32 ReleaseGameObject(GameObjectBase* pObject);

	//!	�J�����ݒ�
	//!	@param	[in]	pCamera	���C���ɂȂ�J����
	void SetSceneCamera(CameraObject* pCamera)
	{
		_pSceneCamera = pCamera;
	}

	//!	�ғ����̃J�����擾
	//!	@return	���C���̃J����
	CameraObject* GetSceneCamera()
	{
		return _pSceneCamera;
	}

	//!	@tparam	T	��������I�u�W�F�N�g�^
	//!	@return	�I�u�W�F�N�g�̌�������
	template <class T>
	std::vector<T*> GetObjects()
	{
		std::vector<T*> result;
		for(auto* pObject : _pObjectList)
		{
			T* p = dynamic_cast<T*>(pObject);
			if(!p)continue;
			result.push_back(p);
		}
		return result;
	}
protected:
	//!	�I�u�W�F�N�g�̍X�V
	void ObjectUpdate();
	//!	�I�u�W�F�N�g�̕`��
	void ObjectRender();
protected:
	bool _isObjectUpdate;	//!< �I�u�W�F�N�g�̎����X�V�̃t���O
private:
	std::list<GameObjectBase*> _pObjectList;	//!< �V�[�����ň�����I�u�W�F�N�g
	CameraObject* _pSceneCamera;				//!< �V�[�����œ������J����
};

//-------------------------------------------------------------------
//!	�Q�[���I�u�W�F�N�g�쐬
//!	@tparam	T		�쐬����Q�[���I�u�W�F�N�g
//!	@tparam	Args	�����^
//!	@param	[in]	args	����
//-------------------------------------------------------------------
template <typename T, typename... Args>
MY_INLINE
RefPointer<T>	CreateGameObject(Args&&... args)
{
	auto* pSceneManager = Manager::Scene();
	MY_ASSERT(pSceneManager != nullptr,"Not Created SceneManager");
	auto* pSceneObject = pSceneManager->GetNowScene();
	MY_ASSERT(pSceneObject != nullptr, "Not Created Scene");

	RefPointer<T> result = CreateRefPointer<T>(args...);
	pSceneObject->AddGameObject(result.get());
	return result;
}

//-------------------------------------------------------------------
//!	�Q�[���I�u�W�F�N�g�쐬(�������֐�)
//!	@tparam	T		�쐬����Q�[���I�u�W�F�N�g
//!	@tparam	Args	�����^
//!	@param	[in]	args	����
//-------------------------------------------------------------------
template <typename T, typename... Args>
MY_INLINE
RefPointer<T>	CreateGameObjectInit(Args&&... args)
{
	auto* pSceneManager = Manager::Scene();
	MY_ASSERT(pSceneManager != nullptr, "Not Created SceneManager");
	auto* pSceneObject = pSceneManager->GetNowScene();
	MY_ASSERT(pSceneObject != nullptr, "Not Created Scene");

	RefPointer<T> result = CreateRefPointerInit<T>(args...);
	pSceneObject->AddGameObject(result.get());
	return result;
}

//-------------------------------------------------------------------
//!	�Q�[���I�u�W�F�N�g�폜 (RefPointer)
//!	@tparam	T			�I�u�W�F�N�g�̌^
//!	@param	[in,out]	pRefObject	�I�u�W�F�N�g�̃|�C���^
//!	@param	[in]		forceDelete	�����폜 default:true
//!	@return	�c��̎Q�Ɛ�
//-------------------------------------------------------------------
template <typename T>
MY_INLINE
void DestroyObject(RefPointer<T>& pRefObject, bool forceDelete = true)
{
	if (!pRefObject)return;

	auto* pSceneObject = Manager::Scene()->GetNowScene();
	assert(pSceneObject != nullptr);

	T* pObject = pRefObject.get();

	GameObjectBase* pParent = pObject->GetParentObject();	//	�e������

	s32 count = 100;

	if(pParent)
	{
		pParent->ReleaseChildObject(pObject);
	}

	count = pSceneObject->ReleaseGameObject(pObject);	//	�V�[������폜

	if (count <= 0)return;

	count = pObject->Release();
	pRefObject.release();

	//	�����폜
	if(forceDelete)
	{
		if(count > 0)
		{
			SafeDelete(pObject);
			DEBUG_LOG("ForceDelete GameObject Count:" << count);
		}
	}
}

//-------------------------------------------------------------------
//!	�Q�[���I�u�W�F�N�g�폜 (const pointer)
//!	@tparam	T			�I�u�W�F�N�g�̌^
//!	@param	[in,out]	pRefObject	�I�u�W�F�N�g�̃|�C���^
//!	@param	[in]		forceDelete	�����폜 default:true
//!	@return	�c��̎Q�Ɛ�
//-------------------------------------------------------------------
template <typename T>
MY_INLINE
void DestroyObject(T* pObject, bool forceDelete = true)
{
	if (!pObject)return;

	auto* pSceneObject = Manager::Scene()->GetNowScene();
	assert(pSceneObject != nullptr);


	GameObjectBase* pParent = pObject->GetParentObject();	//	�e������

	if(pParent)
	{
		pParent->ReleaseChildObject(pObject);
	}

	s32 count = 100;
	count = pSceneObject->ReleaseGameObject(pObject);	//	�V�[������폜

	if (count <= 0)return;

	count = pObject->Release();

	//	�����폜
	if(forceDelete)
	{
		if(count > 0)
		{
			SafeDelete(pObject);
			DEBUG_LOG("ForceDelete GameObject Count:" << count);
		}
	}
}
