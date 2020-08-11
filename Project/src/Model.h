//=============================================================================
//!
//!	@file	Model.��
//!	@brief	���f���N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

namespace Model
{

//	������Model.cpp
class Object;			// ���f���I�u�W�F�N�g
class Face;				// �|���S��
}

//-------------------------------------------------------------------
//!	���f���̃f�[�^��ێ�����I�u�W�F�N�g
//!	���ݑΉ����Ă���̂�mqo�̂�
//-------------------------------------------------------------------
class ModelData : public MeshData
{
public:
	friend class ModelMesh;
	//!	�R���X�g���N�^
	ModelData();
	//!	�f�X�g���N�^
	virtual ~ModelData();

	//!	�`��
	void Render();

	//!	�ǂݍ���
	//!	@param	[in]	fileName	�t�@�C����
	//!	@param	[in]	scale		�X�P�[��
	//!	@retval	true	����
	//!	@retval	false	���s
	bool Load(const std::string& fileName, f32 scale);

	//!	�}�e���A���擾
	//!	@param	[in]	index	�ԍ�
	//!	@return	�}�e���A���ւ̃|�C���^
	//!	@retval	nullptr	�擾���s
	MaterialData* GetMaterial(u32 index)
	{
		if (index >= _modelMaterials.size())return nullptr;
		return &_modelMaterials[index];
	}

	//!	�}�e���A���S�擾
	//!	@return	�}�e���A����vector�z��
	std::vector<MaterialData> GetMaterials()const
	{
		return _modelMaterials;
	}

	//!	�t�@�C�����擾
	//!	@return	�t�@�C����
	std::string GetFileName()const
	{
		return _fileName;
	}

private:
	//! MQO�t�@�C���̓ǂݍ���
	//!	@param	[in]	fileName	MQO�t�@�C����
	//!	@param	[in]	scale		�g�嗦
	//!	@retval	true	����
	//!	@retval	false	���s
	bool	LoadMQO(const std::string& fileName, f32 scale = 0.1f);

private:
	//! ���f���̃}�e���A�� ���̃f�[�^�͎����Ă邾���Ŏg���ɂ͎��o���Ȃ���΂����Ȃ�
	std::vector<MaterialData> _modelMaterials;

	std::vector<Model::Object*>		_pObjects;		//!< ���f���I�u�W�F�N�g�z��
	std::string						_fileName;		//!< �t�@�C����
};
//-------------------------------------------------------------------
//!	���f�����b�V���N���X
//-------------------------------------------------------------------
class ModelMesh : public MeshBase
{
	friend class ModelData;
	friend class ModelObject;
public:
	//! �R���X�g���N�^
	ModelMesh();

	//! �f�X�g���N�^
	virtual ~ModelMesh();

	//!	���f���ǂݍ���
	//!	@param	[in]	fileName	�t�@�C����
	//!	@param	[in]	scale		�g�嗦
	//!	@retval	true	����
	//!	@retval	false	���s
	bool Load(const std::string& fileName, f32 scale = 0.1f);

	//! �`��
	void Render();


	//!	�`����@�ݒ�
	//!	@param	[in]	type	�`��^�C�v
	//!	@retval	true	����
	//!	@retval	false	���s
	bool SetRenderMode(BufferObject::PRIMITIVE_TYPE type);


	//!	���b�V���f�[�^�ݒ�
	//!	@param	[in]	pModelData	���b�V���̃|�C���^
	void SetModelMeshData(ModelData* pModelData);

	//! �I�u�W�F�N�g���擾
	//! @param	[in]	index	�I�u�W�F�N�g�ԍ�(0~)
	//!	@return	�I�u�W�F�N�g�̃|�C���^
	//!	@retval	nullptr	�擾���s
	Model::Object* GetObjectData(u32 index) const
	{
		ModelData* pModel = dynamic_cast<ModelData*>(_pMeshData.get());
		if(!pModel)return nullptr;
		if( index <                  0 ) return nullptr;
		if(index >= pModel->_pObjects.size()) return nullptr;

		return pModel->_pObjects[index];
	}

	//	�I�u�W�F�N�g�̐����擾
	//!	@return	�I�u�W�F�N�g�̐�
	u32 GetObjectSize() const
	{
		ModelData* pModel = dynamic_cast<ModelData*>(_pMeshData.get());
		return pModel->_pObjects.size();
	}

	//!	���f���ێ�
	//!	@retval	false	�ێ����Ă��Ȃ�
	bool IsEmpty()const
	{
		ModelData* pModel = dynamic_cast<ModelData*>(_pMeshData.get());
		return pModel->_pObjects.empty();
	}

	//!	�t�@�C�����擾
	//!	@return	�t�@�C����
	std::string GetFileName()const
	{
		ModelData* pModel = dynamic_cast<ModelData*>(_pMeshData.get());
		return pModel->_fileName;
	}

private:

};

//-------------------------------------------------------------------
//!	���f���I�u�W�F�N�g
//-------------------------------------------------------------------
class ModelObject : public GameObjectBase
{
public:
	//!	�R���X�g���N�^
	ModelObject();
	//!	�f�X�g���N�^
	virtual ~ModelObject();

	//!	���f���ǂݍ���
	//!	@param	[in]	fileName	�t�@�C����
	//!	@param	[in]	scale		�g�嗦
	//!	@retval	true	����
	//!	@retval	false	���s
	bool Load(const std::string& fileName, f32 scale = 0.1f);

	//! �`��
	void Render();

	//!	���f���ݒ�
	//!	@param	[in]	pModel	���b�V��
	void SetModel(ModelMesh* pModel);

	//!	���f���ݒ�
	//!	@param	[in]	model	���f���I�u�W�F�N�g
	void SetModel(const ModelObject& model);

	//!	�`����@�ݒ�
	//!	@param	[in]	type	�`��^�C�v
	//!	@retval	true	����
	//!	@retval	false	���s
	bool SetRenderMode(BufferObject::PRIMITIVE_TYPE type)
	{
		return _pModelInstance->SetRenderMode(type);
	}

	//! �}�e���A�����擾
	//! @param	[in]	�}�e���A���ԍ�(0~)
	//!	@return	�}�e���A���̃|�C���^	���s����nullptr
	MaterialData* GetMaterial(u32 index)
	{
		if(_pModelInstance)
		{
			return _pModelInstance->GetMaterial(index);
		}
		return nullptr;
	}

	//! �I�u�W�F�N�g���擾
	//! @param	[in]	index	�I�u�W�F�N�g�ԍ�(0~)
	//!	@return	�I�u�W�F�N�g�̃|�C���^	���s����nullptr
	Model::Object* GetObjectData(u32 index) const
	{
		if(_pModelInstance)
		{
			_pModelInstance->GetObjectData(index);
		}
		return nullptr;
	}

	//!	�}�e���A���̐����擾
	//!	@return	�}�e���A���̐�
	u32 GetMaterialSize() const
	{
		return _pModelInstance->GetMaterialSize();
	}
	//	�I�u�W�F�N�g�̐����擾
	//!	@return	�I�u�W�F�N�g�̐�
	u32 GetObjectSize() const
	{
		return _pModelInstance->GetObjectSize();
	}

	//!	���f���ێ�
	//!	@retval	false	�ێ����Ă��Ȃ�
	bool IsEmpty()const
	{
		if(_pModelInstance)
		{
			return _pModelInstance->IsEmpty();
		}
		return false;
	}

	//!	�C�R�[�����Z�q
	ModelObject& operator = (const ModelObject& p)
	{
		_pModelInstance.reset(p._pModelInstance.get());
		_pModelInstance->AddRef();
		return *this;
	}

private:
	RefPointer<ModelMesh> _pModelInstance;	//!< ���f�����b�V���f�[�^
};

//-------------------------------------------------------------------
//!	���f���Ǘ��N���X
//-------------------------------------------------------------------
class ModelManager : public ManagerBase
{
public:
	//!	�I��
	void Exit();

	//!	���f������
	//!	@param	[in]	fileName	�t�@�C����
	//!	@return	���f���f�[�^�̃|�C���^
	//!	@retval	nullptr	�擾���s
	ModelData* Search(std::string fileName);

	//!	���f���ǉ�
	//!	@param	[in]	pModel	���f���f�[�^
	void AddModel(ModelData* pModel);


	//!	���f���ǂݍ���
	//!	@param	[in]	fileName	�t�@�C����
	//!	@param	[in]	scale		�g�嗦
	//!	@return	���f���f�[�^�̃|�C���^
	//!	@retval	nullptr	�ǂݍ��ݎ��s
	ModelData*  LoadMesh(const std::string& fileName, f32 scale = 0.1f);

private:
	std::list<ModelData*> _pModels;		//!< ���f���f�[�^
};


MANAGER_INTERFACE(ModelManager, Model);

