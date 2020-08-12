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
//!	�I�u�W�F�N�g���N���X
//-------------------------------------------------------------------
class GameObjectBase : public BasicBase
{
	friend class SceneBase;
public:
	//!	�R���X�g���N�^
	GameObjectBase();
	//!	�R���X�g���N�^
	//!	@param	[in]	position	���W
	//!	@param	[in]	rotate		��]
	//!	@param	[in]	Scale		�T�C�Y	default:Vector3::ONE
	GameObjectBase(const Vector3& position, const Quaternion& rotate, const Vector3& scale = Vector3::ONE, bool enable = true);
	//!	�f�X�g���N�^
	virtual ~GameObjectBase();

	//!	�e�ݒ�
	//!	@param	[in]	pParentObject	�e�̃|�C���^
	void SetParentObject(GameObjectBase* pParentObject);

	//!	�e�擾
	//!	@return	�e�I�u�W�F�N�g�|�C���^
	//!	@retval	nullptr	�e�Ȃ�
	GameObjectBase* GetParentObject()const;


	//!	�q�ǉ�
	//!	@param	[in]	pChildObject	�q�̃|�C���^
	void AddChildObject(GameObjectBase* pChildObject);
	//!	�q�폜
	//!	@param	[in]	pChildObject	�q�̃|�C���^
	//!	@return	�����̎Q�Ɛ� 0�Ȃ�폜�ς�
	s32 ReleaseChildObject(GameObjectBase* pChildObject);

	//!	�e����̉��
	void ReleaseParent();

	//!	�q�I�u�W�F�N�g�^�O����
	//!	@param	[in]	tag	�I�u�W�F�N�g�̃^�O
	//!	@return	�I�u�W�F�N�g�̃|�C���^
	//!	@retval	nullptr	�f�[�^�Ȃ�
	GameObjectBase* GetChildObject(const std::string& tag)const;

	//!	�q�I�u�W�F�N�g�̉��
	void ReleaseChildObjects();

	//	�ݒ�֐�
	//!	���[���h�s��ݒ�
	//!	@param	[in]	matrix		�s��
	void SetWorldMatrix(const Matrix& matrix);
	//!	���[���h���W
	//!	@param	[in]	position	���W
	void SetWorldPosition(const Vector3& position);
	//!	���[���h��]
	//!	@param	[in]	rotate		��]
	void SetWorldRotate(const Quaternion& rotate);
	//!	���[���h�T�C�Y
	//!	@param	[in]	scale		�T�C�Y
	void SetWorldScale(const Vector3& scale);

	//!	���[���h��]
	//!	@param	[in]	rotate		��]
	void AddWorldPosition(const Vector3& position);
	//!	���[���h��]
	//!	@param	[in]	rotate		��]
	void AddWorldRotate(const Quaternion& rotate);
	//!	���[���h�T�C�Y
	//!	@param	[in]	scale		�T�C�Y
	void AddWorldScale(const Vector3& scale);

	//!	���[�J���s��
	//!	@param	[in]	matrix		�s��
	void SetLocalMatrix(const Matrix& matrix);

	//!	���[�J�����W
	//!	@param	[in]	position	���W
	void SetLocalPosition(const Vector3& position);
	//!	���[�J����]
	//!	@param	[in]	rotate		��]
	void SetLocalRotate(const Quaternion& rotate);
	//!	���[�J���T�C�Y
	//!	@param	[in]	scale		�T�C�Y
	void SetLocalScale(const Vector3& scale);

	//!	���[�J�����W
	//!	@param	[in]	position	���W
	void AddLocalPosition(const Vector3& position);
	//!	���[�J����]
	//!	@param	[in]	rotate		��]
	void AddLocalRotate(const Quaternion& rotate);
	//!	���[�J���T�C�Y
	//!	@param	[in]	scale		�T�C�Y
	void AddLocalScale(const Vector3& scale);

	//!	�e���[�J�����W
	//!	@param	[in]	position	���W
	void SetRelativePosition(const Vector3& position);
	//!	�e���[�J����]
	//!	@param	[in]	rotate		��]
	void SetRelativeRotate(const Quaternion& rotate);

	//!	�e���[�J�����W
	//!	@param	[in]	position	���W
	void AddRelativePosition(const Vector3& position);
	//!	�e���[�J����]
	//!	@param	[in]	rotate		��]
	void AddRelativeRotate(const Quaternion& rotate);

	//!	���[���h�s��쐬
	//!	@param	[in]	myMatrix	�|�������s��
	//!	@param	[in]	pParent		�e�I�u�W�F�N�g�̃|�C���^ default:nullptr
	Matrix  CreateWorldMatrix(const Matrix& myMatrix, GameObjectBase* pParent = nullptr) const;


	//	�擾�֐�

	//!	���[�J���s��
	//!	@return �s��
	Matrix  GetLocalMatrix()const		{ return _localMatrix; }
	//!	���[�J�����W
	//!	@return ���W
	Vector3 GetLocalPosition()const		{ return _localPosition; }
	//!	���[�J����]
	//!	@return ��]
	Quaternion GetLocalRotate()const	{ return _localRotate; }
	//!	���[�J���T�C�Y
	//!	@return �T�C�Y
	Vector3 GetLocalScale()const		{ return _localScale; }


	//!	���[���h�s��
	//!	@return	�s��
	Matrix  GetWorldMatrix()const
	{
		return _worldMatrix;
	}

	//!	�e�̃��[���h�s��
	//!	@return	�s��
	Matrix  GetWorldParentMatrix()const
	{
		if (_pParentObject)
		{
			return _pParentObject->_worldMatrix;
		}
		return Matrix::IDENTITY;
	}

	//!	���[���h���W
	//!	@return	���W
	Vector3 GetWorldPosition()const
	{
		return GetWorldMatrix().GetPosition();
	}
	//!	���[���h��]
	//!	@return	��]
	Quaternion GetWorldRotate()const
	{
		return GetWorldMatrix().GetRotateQuaternion();
	}
	//!	���[���h�T�C�Y
	//!	@return	�T�C�Y
	Vector3 GetWorldScale()const
	{
		return GetWorldMatrix().GetScale();
	}
	//!	�I�u�W�F�N�g���[���hX�x�N�g��
	//!	@return	�I�u�W�F�N�g���[���hX�x�N�g��
	Vector3 GetWorldXVector()const
	{
		return GetWorldMatrix().GetXVector();
	}
	//!	�I�u�W�F�N�g���[���hY�x�N�g��
	//!	@return	�I�u�W�F�N�g���[���hY�x�N�g��
	Vector3 GetWorldYVector()const
	{
		return GetWorldMatrix().GetYVector();
	}
	//!	�I�u�W�F�N�g���[���hZ�x�N�g��
	//!	@return	�I�u�W�F�N�g���[���hZ�x�N�g��
	Vector3 GetWorldZVector()const
	{
		return GetWorldMatrix().GetZVector();
	}

	//!	Process�I�u�W�F�N�g�쐬
	//!	@tparam	T		�I�u�W�F�N�g�^
	//!	@tparam	Args	�������X�g�^
	//!	@param	args	�������X�g
	//!	@return	Process�I�u�W�F�N�g
	template <typename T, typename... Args>
	RefPointer<T> CreateProcess(Args&&... args)
	{
		RefPointer<T> result = CreateRefPointer<T>(args...);
		AddProcess(result.get());
		return result;
	}

	//!	Process�I�u�W�F�N�g�쐬 (Init)
	//!	@tparam	T		�I�u�W�F�N�g�^
	//!	@tparam	Args	�������X�g�^
	//!	@param	args	�������X�g
	//!	@return	Process�I�u�W�F�N�g
	template <typename T, typename... Args>
	RefPointer<T> CreateProcessInit(Args&&... args)
	{
		RefPointer<T> result = CreateRefPointerInit<T>(args...);
		AddProcess(result.get());
		return result;
	}

	//!	Mesh�I�u�W�F�N�g�쐬
	//!	@tparam	T		�I�u�W�F�N�g�^
	//!	@tparam	Args	�������X�g�^
	//!	@param	args	�������X�g
	//!	@return	Mesh�I�u�W�F�N�g
	template <typename T, typename... Args>
	RefPointer<T> CreateMesh(Args&&... args)
	{
		RefPointer<T> result = CreateRefPointer<T>(args...);
		AddMesh(result.get());
		return result;
	}

	//!	Mesh�I�u�W�F�N�g�쐬 (Init)
	//!	@tparam	T		�I�u�W�F�N�g�^
	//!	@tparam	Args	�������X�g�^
	//!	@param	args	�������X�g
	//!	@return	Mesh�I�u�W�F�N�g
	template <typename T, typename... Args>
	RefPointer<T> CreateMeshInit(Args&&... args)
	{
		RefPointer<T> result = CreateRefPointerInit<T>(args...);
		AddMesh(result.get());
		return result;
	}

	//!	�q�I�u�W�F�N�g�쐬
	//!	@tparam	T		�I�u�W�F�N�g�^
	//!	@tparam	Args	�������X�g�^
	//!	@param	args	�������X�g
	//!	@return	�q�I�u�W�F�N�g
	template <typename T, typename... Args>
	RefPointer<T> CreateChildObject(Args&&... args)
	{
		RefPointer<T> result = CreateGameObject<T>(args...);
		AddChildObject(result.get());
		return result;
	}

	//!	�q�I�u�W�F�N�g�쐬 (Init)
	//!	@tparam	T		�I�u�W�F�N�g�^
	//!	@tparam	Args	�������X�g�^
	//!	@param	args	�������X�g
	//!	@return	�q�I�u�W�F�N�g
	template <typename T, typename... Args>
	RefPointer<T> CreateChildObjectInit(Args&&... args)
	{
		RefPointer<T> result = CreateGameObjectInit<T>(args...);
		AddChildObject(result.get());
		return result;
	}

	//!	Process�I�u�W�F�N�g�ǉ�
	//!	@param	[in]	pProcess	Process�I�u�W�F�N�g�̃|�C���^
	void AddProcess(ProcessBase* pProcess);

	//!	Mesh�I�u�W�F�N�g�ǉ�
	//!	@param	[in]	pProcess	Mesh�I�u�W�F�N�g�̃|�C���^
	void AddMesh(MeshBase* pMesh);

	//!	Process�I�u�W�F�N�g����
	//!	@tparam	T	��������Process�I�u�W�F�N�g�̌^
	//!	@return	��������
	template <class T>
	std::vector<T*> GetProcesss()
	{
		std::vector<T*> result;
		for(auto* pProcess : _pProcesss)
		{
			T* p = dynamic_cast<T*>(pProcess);
			if(p)
			{
				result.push_back(p);
			}
		}
		return result;
	}

	//!	Mesh�I�u�W�F�N�g����
	//!	@tparam	T	��������Mesh�I�u�W�F�N�g�̌^
	//!	@return	��������
	template <class T>
	std::vector<T*> GetMeshs()
	{
		std::vector<T*> result;
		for(auto* pMesh : _pMeshs)
		{
			T* p = dynamic_cast<T*>(pMesh);
			if(p)
			{
				result.push_back(p);
			}
		}
		return result;
	}

	//!	�s��̃o�C���h
	void BindWorldMatrix();


	//!	Process�I�u�W�F�N�g�̃^�O����
	//!	@param	[in]	tag	�^�O
	//!	@return	��������
	std::vector<ProcessBase*> GetProcesss(const std::string& tag);

	//!	Process�I�u�W�F�N�g�̃^�O����
	//!	@param	[in]	tags	�^�O
	//!	@return	��������
	std::vector<ProcessBase*> GetProcesss(const std::vector<std::string>& tags);

	//!	Mesh�I�u�W�F�N�g�̃^�O����
	//!	@param	[in]	tag	�^�O
	//!	@return	��������
	std::vector<MeshBase*> GetMeshs(const std::string& tag);
	//!	Mesh�I�u�W�F�N�g�̃^�O����
	//!	@param	[in]	tags	�^�O
	//!	@return	��������
	std::vector<MeshBase*> GetMeshs(const std::vector<std::string>& tags);


	//!	�L�����t���O�ݒ�
	//!	@param	[in]	enable	�L����
	void Enable(bool enable)
	{
		_enable = enable;
	}

	//!	�L�����t���O�擾
	//!	@retval	true	�L��
	//!	@retval	false	����
	bool IsEnable()const
	{
		return _enable;
	}
protected:
	//!	�s��̍X�V
	void MatrixUpdate()
	{
		_localMatrix = Matrix::Affine(_localPosition, _localRotate, _localScale);
		if(_pParentObject)
		{
			_worldMatrix = _localMatrix * _pParentObject->_worldMatrix;
		}
		else
		{
			_worldMatrix = _localMatrix;
		}
		for(auto* pChild : _pChildrenObjects)
		{
			pChild->MatrixUpdate();
		}
	}

private:
	//!	�q�I�u�W�F�N�g�̍X�V
	void ChildrenUpdate()
	{
		for (auto& pObject : _pChildrenObjects)
		{
			if(pObject && pObject->IsEnable())
			{
				pObject->Update();
				if (!pObject)continue;	//	�����Ă��玟�̃I�u�W�F�N�g��
				pObject->ProcessUpdate();
				if (!pObject)continue;	//	�����Ă��玟�̃I�u�W�F�N�g��
				pObject->ChildrenUpdate();
			}
		}

		//	�������I�u�W�F�N�g�����X�g�������
		auto itr = _pChildrenObjects.begin();
		while (itr != _pChildrenObjects.end())
		{
			if (!(*itr))
			{
				itr = _pChildrenObjects.erase(itr);
				continue;
			}
			++itr;
		}

	}
	//!	�q�I�u�W�F�N�g�̕`��
	void ChildrenRender()
	{
		auto itr = _pChildrenObjects.begin();
		while(itr != _pChildrenObjects.end())
		{
			GameObjectBase* p = (*itr);
			if(p && p->IsEnable())
			{
				p->BindWorldMatrix();
				p->Render();
				p->MeshRender();
				p->ChildrenRender();
			}
			++itr;
		}
	}

	//!	�v���Z�X�X�V
	void ProcessUpdate();
	//!	���b�V���`��
	void MeshRender();

protected:
	//!	�v���Z�X���
	//!	@param	[in]	pProcess	�v���Z�X�̃|�C���^
	void ReleaseProcess(ProcessBase* pProcess);
	//!	���b�V�����
	//!	@param	[in]	pMesh		���b�V���̃|�C���^
	void ReleaseMesh(MeshBase* pMesh);

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
