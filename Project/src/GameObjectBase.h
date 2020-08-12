//=============================================================================
//!
//!	@file	GameObjectBase.h
//!	@brief	�I�u�W�F�N�g���N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

// TODO �L�[��}�E�X�Ȃǂ̃C�x���g�N���X������

//	�`��̓V�[���ɓo�^���Ă���ΑS�Ď����ɂȂ�
//	�q�I�u�W�F�N�g�A���g�̒ǉ����b�V���̓I�u�W�F�N�g�ɓo�^���Ă����Ύ����ŕ`�悷��
//	�R���|�[�l���g���I�u�W�F�N�g�ɓo�^���邱�ƂŎ����Ŏ��s����
//	�I�[�o�[���C�h�͊�{�I�ɂ�Update�݂̂ŏ������������K�v�ȏꍇ��Init��
//	Render�͊�{�I�ɂ͕K�v�Ȃ����L�q�͉\�Ȃ̂ŕK�v�ł���΃I�[�o�[���C�h����
//	Update,Render�͎����ŌĂяo�����
//-------------------------------------------------------------------
//! @brief  �I�u�W�F�N�g���N���X
//-------------------------------------------------------------------
class GameObjectBase : public BasicBase
{
	friend class SceneBase;

public:
	//! @brief  �R���X�g���N�^
	GameObjectBase();

	//! @brief  �R���X�g���N�^
	//!	@param	[in]	position	���W
	//!	@param	[in]	rotate		��]
	//!	@param	[in]	Scale		�T�C�Y	default:Vector3::ONE
	GameObjectBase(const Vector3& position, const Quaternion& rotate, const Vector3& scale = Vector3::ONE, bool enable = true);

	//! @brief  �f�X�g���N�^
	virtual ~GameObjectBase();

	//! @brief  �e�ݒ�
	//!	@param	[in]	pParentObject	�e�̃|�C���^
	void SetParentObject(GameObjectBase* pParentObject);

	//! @brief  �e�擾
	//!	@return	�e�I�u�W�F�N�g�|�C���^
	//!	@retval	nullptr	�e�Ȃ�
	GameObjectBase* GetParentObject()const;

	//! @brief  �q�ǉ�
	//!	@param	[in]	pChildObject	�q�̃|�C���^
	void AddChildObject(GameObjectBase* pChildObject);

	//! @brief  �q�폜
	//!	@param	[in]	pChildObject	�q�̃|�C���^
	//!	@return	�����̎Q�Ɛ� 0�Ȃ�폜�ς�
	s32 ReleaseChildObject(GameObjectBase* pChildObject);

	//! @brief  �e����̉��
	void ReleaseParent();

	//! @brief  �q�I�u�W�F�N�g�^�O����
	//!	@param	[in]	tag	�I�u�W�F�N�g�̃^�O
	//!	@return	�I�u�W�F�N�g�̃|�C���^
	//!	@retval	nullptr	�f�[�^�Ȃ�
	GameObjectBase* GetChildObject(const std::string& tag)const;

	//! @brief  �q�I�u�W�F�N�g�̉��
	void ReleaseChildObjects();

	//	�ݒ�֐�
	//! @brief  ���[���h�s��ݒ�
	//!	@param	[in]	matrix		�s��
	void SetWorldMatrix(const Matrix& matrix);

	//! @brief  ���[���h���W
	//!	@param	[in]	position	���W
	void SetWorldPosition(const Vector3& position);

	//! @brief  ���[���h��]
	//!	@param	[in]	rotate		��]
	void SetWorldRotate(const Quaternion& rotate);

	//! @brief  ���[���h�T�C�Y
	//!	@param	[in]	scale		�T�C�Y
	void SetWorldScale(const Vector3& scale);

	//! @brief  ���[���h��]
	//!	@param	[in]	rotate		��]
	void AddWorldPosition(const Vector3& position);

	//! @brief  ���[���h��]
	//!	@param	[in]	rotate		��]
	void AddWorldRotate(const Quaternion& rotate);

	//! @brief  ���[���h�T�C�Y
	//!	@param	[in]	scale		�T�C�Y
	void AddWorldScale(const Vector3& scale);

	//! @brief  ���[�J���s��
	//!	@param	[in]	matrix		�s��
	void SetLocalMatrix(const Matrix& matrix);

	//! @brief  ���[�J�����W
	//!	@param	[in]	position	���W
	void SetLocalPosition(const Vector3& position);

	//! @brief  ���[�J����]
	//!	@param	[in]	rotate		��]
	void SetLocalRotate(const Quaternion& rotate);

	//! @brief  ���[�J���T�C�Y
	//!	@param	[in]	scale		�T�C�Y
	void SetLocalScale(const Vector3& scale);

	//! @brief  ���[�J�����W
	//!	@param	[in]	position	���W
	void AddLocalPosition(const Vector3& position);

	//! @brief  ���[�J����]
	//!	@param	[in]	rotate		��]
	void AddLocalRotate(const Quaternion& rotate);

	//! @brief  ���[�J���T�C�Y
	//!	@param	[in]	scale		�T�C�Y
	void AddLocalScale(const Vector3& scale);

	//! @brief  �e���[�J�����W
	//!	@param	[in]	position	���W
	void SetRelativePosition(const Vector3& position);

	//! @brief  �e���[�J����]
	//!	@param	[in]	rotate		��]
	void SetRelativeRotate(const Quaternion& rotate);

	//! @brief  �e���[�J�����W
	//!	@param	[in]	position	���W
	void AddRelativePosition(const Vector3& position);

	//! @brief  �e���[�J����]
	//!	@param	[in]	rotate		��]
	void AddRelativeRotate(const Quaternion& rotate);

	//! @brief  ���[���h�s��쐬
	//!	@param	[in]	myMatrix	�|�������s��
	//!	@param	[in]	pParent		�e�I�u�W�F�N�g�̃|�C���^ default:nullptr
	Matrix  CreateWorldMatrix(const Matrix& myMatrix, GameObjectBase* pParent = nullptr) const;

	//	�擾�֐�
	//! @brief  ���[�J���s��
	//!	@return �s��
	Matrix  GetLocalMatrix()const;

	//! @brief  ���[�J�����W
	//!	@return ���W
	Vector3 GetLocalPosition()const;

	//! @brief  ���[�J����]
	//!	@return ��]
	Quaternion GetLocalRotate()const;

	//! @brief  ���[�J���T�C�Y
	//!	@return �T�C�Y
	Vector3 GetLocalScale()const;

	//! @brief  ���[���h�s��
	//!	@return	�s��
	Matrix  GetWorldMatrix()const;

	//! @brief  �e�̃��[���h�s��
	//!	@return	�s��
	Matrix  GetWorldParentMatrix()const;

	//! @brief  ���[���h���W
	//!	@return	���W
	Vector3 GetWorldPosition()const;

	//! @brief  ���[���h��]
	//!	@return	��]
	Quaternion GetWorldRotate()const;

	//! @brief  ���[���h�T�C�Y
	//!	@return	�T�C�Y
	Vector3 GetWorldScale()const;

	//! @brief  �I�u�W�F�N�g���[���hX�x�N�g��
	//!	@return	�I�u�W�F�N�g���[���hX�x�N�g��
	Vector3 GetWorldXVector()const;

	//! @brief  �I�u�W�F�N�g���[���hY�x�N�g��
	//!	@return	�I�u�W�F�N�g���[���hY�x�N�g��
	Vector3 GetWorldYVector()const;

	//! @brief  �I�u�W�F�N�g���[���hZ�x�N�g��
	//!	@return	�I�u�W�F�N�g���[���hZ�x�N�g��
	Vector3 GetWorldZVector()const;

	//! @brief  Process�I�u�W�F�N�g�ǉ�
	//!	@param	[in]	pProcess	Process�I�u�W�F�N�g�̃|�C���^
	void AddProcess(ProcessBase* pProcess);

	//! @brief  Mesh�I�u�W�F�N�g�ǉ�
	//!	@param	[in]	pProcess	Mesh�I�u�W�F�N�g�̃|�C���^
	void AddMesh(MeshBase* pMesh);

	//! @brief  �s��̃o�C���h
	void BindWorldMatrix();

	//! @brief  Process�I�u�W�F�N�g�̃^�O����
	//!	@param	[in]	tag	�^�O
	//!	@return	��������
	std::vector<ProcessBase*> GetProcesss(const std::string& tag);

	//! @brief  Process�I�u�W�F�N�g�̃^�O����
	//!	@param	[in]	tags	�^�O
	//!	@return	��������
	std::vector<ProcessBase*> GetProcesss(const std::vector<std::string>& tags);

	//! @brief  Mesh�I�u�W�F�N�g�̃^�O����
	//!	@param	[in]	tag	�^�O
	//!	@return	��������
	std::vector<MeshBase*> GetMeshs(const std::string& tag);

	//! @brief  Mesh�I�u�W�F�N�g�̃^�O����
	//!	@param	[in]	tags	�^�O
	//!	@return	��������
	std::vector<MeshBase*> GetMeshs(const std::vector<std::string>& tags);

	//! @brief  �L�����t���O�ݒ�
	//!	@param	[in]	enable	�L����
	void Enable(bool enable);

	//! @brief  �L�����t���O�擾
	//!	@retval	true	�L��
	//!	@retval	false	����
	bool IsEnable()const;

public:
	////! @brief  Process�I�u�W�F�N�g�쐬
	////!	@tparam	T		�I�u�W�F�N�g�^
	////!	@tparam	Args	�������X�g�^
	////!	@param	args	�������X�g
	////!	@return	Process�I�u�W�F�N�g
	//template <typename T, typename... Args>
	//RefPointer<T> CreateProcess(Args&&... args);

	//! @brief  Process�I�u�W�F�N�g�쐬 (Init)
	//!	@tparam	T		�I�u�W�F�N�g�^
	//!	@tparam	Args	�������X�g�^
	//!	@param	args	�������X�g
	//!	@return	Process�I�u�W�F�N�g
	template <typename T, typename... Args>
	RefPointer<T> CreateProcessInit(Args&&... args);

	//! @brief  Mesh�I�u�W�F�N�g�쐬
	//!	@tparam	T		�I�u�W�F�N�g�^
	//!	@tparam	Args	�������X�g�^
	//!	@param	args	�������X�g
	//!	@return	Mesh�I�u�W�F�N�g
	template <typename T, typename... Args>
	RefPointer<T> CreateMesh(Args&&... args);

	//! @brief  Mesh�I�u�W�F�N�g�쐬 (Init)
	//!	@tparam	T		�I�u�W�F�N�g�^
	//!	@tparam	Args	�������X�g�^
	//!	@param	args	�������X�g
	//!	@return	Mesh�I�u�W�F�N�g
	template <typename T, typename... Args>
	RefPointer<T> CreateMeshInit(Args&&... args);

	//! @brief  �q�I�u�W�F�N�g�쐬
	//!	@tparam	T		�I�u�W�F�N�g�^
	//!	@tparam	Args	�������X�g�^
	//!	@param	args	�������X�g
	//!	@return	�q�I�u�W�F�N�g
	template <typename T, typename... Args>
	RefPointer<T> CreateChildObject(Args&&... args);

	//! @brief  �q�I�u�W�F�N�g�쐬 (Init)
	//!	@tparam	T		�I�u�W�F�N�g�^
	//!	@tparam	Args	�������X�g�^
	//!	@param	args	�������X�g
	//!	@return	�q�I�u�W�F�N�g
	template <typename T, typename... Args>
	RefPointer<T> CreateChildObjectInit(Args&&... args);

	//! @brief  Process�I�u�W�F�N�g����
	//!	@tparam	T	��������Process�I�u�W�F�N�g�̌^
	//!	@return	��������
	template <class T>
	std::vector<T*> GetProcesss();

	//! @brief  Mesh�I�u�W�F�N�g����
	//!	@tparam	T	��������Mesh�I�u�W�F�N�g�̌^
	//!	@return	��������
	template <class T>
	std::vector<T*> GetMeshs();

protected:
	//! @brief  �s��̍X�V
	void MatrixUpdate();

	//! @brief  �v���Z�X���
	//!	@param	[in]	pProcess	�v���Z�X�̃|�C���^
	void ReleaseProcess(ProcessBase* pProcess);

	//! @brief  ���b�V�����
	//!	@param	[in]	pMesh		���b�V���̃|�C���^
	void ReleaseMesh(MeshBase* pMesh);

private:
	//! @brief  �q�I�u�W�F�N�g�̍X�V
	void ChildrenUpdate();

	//! @brief  �q�I�u�W�F�N�g�̕`��
	void ChildrenRender();

	//! @brief  �v���Z�X�X�V
	void ProcessUpdate();

	//! @brief  ���b�V���`��
	void MeshRender();

protected:
	GameObjectBase*				_pParentObject;		//!< �e�I�u�W�F�N�g
	std::list<GameObjectBase*>	_pChildrenObjects;	//!< �q�I�u�W�F�N�g

	std::list<MeshBase*>		_pMeshs;			//!< ���b�V��
	std::list<ProcessBase*>		_pProcesss;			//!< �v���Z�X

	Vector3		_localPosition;	//!< ���W
	Quaternion	_localRotate;	//!< ��]����
	Vector3		_localScale;	//!< �T�C�Y
	Matrix		_localMatrix;	//!< �}�g���b�N�X

	Matrix		_worldMatrix;	//!< �}�g���b�N�X(���[���h)

	bool _enable;	//!< �L���t���O
};

////-----------------------------------------------------------------------------
////  @brief  Process�I�u�W�F�N�g�쐬
////  @tparam	T		�I�u�W�F�N�g�^
////  @tparam	Args	�������X�g�^
////  @param	args	�������X�g
////  @return	Process�I�u�W�F�N�g
////-----------------------------------------------------------------------------
//template<typename T, typename ...Args>
//inline RefPointer<T> GameObjectBase::CreateProcess(Args && ...args)
//{
//	RefPointer<T> result = CreateRefPointer<T>(args...);
//	AddProcess(result.get());
//	return result;
//}

//-----------------------------------------------------------------------------
//  @brief  Process�I�u�W�F�N�g�쐬 (Init)
//  @tparam	T		�I�u�W�F�N�g�^
//  @tparam	Args	�������X�g�^
//  @param	args	�������X�g
//  @return	Process�I�u�W�F�N�g
//-----------------------------------------------------------------------------
template<typename T, typename ...Args>
inline RefPointer<T> GameObjectBase::CreateProcessInit(Args && ...args)
{
	RefPointer<T> result = CreateRefPointerInit<T>(args...);
	AddProcess(result.get());
	return result;
}

//-----------------------------------------------------------------------------
//  @brief  Mesh�I�u�W�F�N�g�쐬
//  @tparam	T		�I�u�W�F�N�g�^
//  @tparam	Args	�������X�g�^
//  @param	args	�������X�g
//  @return	Mesh�I�u�W�F�N�g
//-----------------------------------------------------------------------------
template<typename T, typename ...Args>
inline RefPointer<T> GameObjectBase::CreateMesh(Args && ...args)
{
	RefPointer<T> result = CreateRefPointer<T>(args...);
	AddMesh(result.get());
	return result;
}

//-----------------------------------------------------------------------------
//  @brief  Mesh�I�u�W�F�N�g�쐬 (Init)
//  @tparam	T		�I�u�W�F�N�g�^
//  @tparam	Args	�������X�g�^
//  @param	args	�������X�g
//  @return	Mesh�I�u�W�F�N�g
//-----------------------------------------------------------------------------
template<typename T, typename ...Args>
inline RefPointer<T> GameObjectBase::CreateMeshInit(Args && ...args)
{
	RefPointer<T> result = CreateRefPointerInit<T>(args...);
	AddMesh(result.get());
	return result;
}

//-----------------------------------------------------------------------------
//  @brief  �q�I�u�W�F�N�g�쐬
//  @tparam	T		�I�u�W�F�N�g�^
//  @tparam	Args	�������X�g�^
//  @param	args	�������X�g
//  @return	�q�I�u�W�F�N�g
//-----------------------------------------------------------------------------
template<typename T, typename ...Args>
inline RefPointer<T> GameObjectBase::CreateChildObject(Args && ...args)
{
	RefPointer<T> result = CreateGameObject<T>(args...);
	AddChildObject(result.get());
	return result;
}

//-----------------------------------------------------------------------------
//  @brief  �q�I�u�W�F�N�g�쐬 (Init)
//  @tparam	T		�I�u�W�F�N�g�^
//  @tparam	Args	�������X�g�^
//  @param	args	�������X�g
//  @return	�q�I�u�W�F�N�g
//-----------------------------------------------------------------------------
template<typename T, typename ...Args>
inline RefPointer<T> GameObjectBase::CreateChildObjectInit(Args && ...args)
{
	RefPointer<T> result = CreateGameObjectInit<T>(args...);
	AddChildObject(result.get());
	return result;
}

//-----------------------------------------------------------------------------
//  @brief  Process�I�u�W�F�N�g����
//  @tparam	T	��������Process�I�u�W�F�N�g�̌^
//  @return	��������
//-----------------------------------------------------------------------------
template<class T>
inline std::vector<T*> GameObjectBase::GetProcesss()
{
	std::vector<T*> result;
	for (auto* pProcess : _pProcesss)
	{
		T* p = dynamic_cast<T*>(pProcess);
		if (p)
		{
			result.push_back(p);
		}
	}
	return result;
}

//-----------------------------------------------------------------------------
//  @brief  Mesh�I�u�W�F�N�g����
//  @tparam	T	��������Mesh�I�u�W�F�N�g�̌^
//  @return	��������
//-----------------------------------------------------------------------------
template<class T>
inline std::vector<T*> GameObjectBase::GetMeshs()
{
	std::vector<T*> result;
	for (auto* pMesh : _pMeshs)
	{
		T* p = dynamic_cast<T*>(pMesh);
		if (p)
		{
			result.push_back(p);
		}
	}
	return result;
}
