//=============================================================================
//!
//!	@file	BaseMesh.h
//!	@brief	���b�V�����N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//!	�}�e���A���J���[���[�h
enum class MATERIAL_COLOR
{
	VERTEX = 0,
	MATERIAL = 1,
	TEXTURE = 2
};

//-------------------------------------------------------------------
//!	�}�e���A���f�[�^
//-------------------------------------------------------------------
struct MaterialData
{
	//!	�R���X�g���N�^
	MaterialData()
	: _colorMode(MATERIAL_COLOR::MATERIAL)
	, _color	(128)
	, _roughness(0.0f)
	, _metalness(0.0f)
	, _emissiveColor(Vector3::ZERO)
	, _pTexture(nullptr)
	{
	}

	//!	�R���X�g���N�^
	//!	@param	[in]	colorMode	�J���[���[�h
	//!	@param	[in]	color		�}�e���A���J���[
	//!	@param	[in]	roughness	���t
	//!	@param	[in]	metalness	���^��
	//!	@param	[in]	pTexture	�}�e���A���e�N�X�`��	default:nullptr
	MaterialData(MATERIAL_COLOR colorMode, const Color& color, f32 roughness, f32 metalness,const Vector3& emissiveColor = Vector3::ZERO, Texture* pTexture = nullptr)
	: _colorMode	(colorMode)
	, _color		(color)
	, _roughness	(roughness)
	, _metalness	(metalness)
	, _emissiveColor(emissiveColor)
	, _pTexture		(pTexture)
	{
	}

	//!	�f�X�g���N�^
	virtual ~MaterialData()
	{
	}

	static const MaterialData DEFAULT;

	MATERIAL_COLOR	_colorMode;		//!< �J���[���[�h
	Color			_color;			//!< �}�e���A���J���[
	f32				_roughness;		//!< ���t
	f32				_metalness;		//!< ���^��
	Vector3			_emissiveColor;	//!< �G�~�b�V�u�J���[
	Texture*		_pTexture;		//!< �}�e���A���e�N�X�`��

};


//!	@class GameObjectBase BaseObject.h "BaseObject.h"
class GameObjectBase;

//-------------------------------------------------------------------
//!	���b�V���f�[�^�I�u�W�F�N�g
//-------------------------------------------------------------------
class MeshData : public BasicBase
{
public:
	MeshData()
	{}
	virtual ~MeshData(){}

	virtual void Render() = 0;

protected:

};


//-------------------------------------------------------------------
//!	���b�V�����N���X
//-------------------------------------------------------------------
class MeshBase : public BasicBase
{
public:
	//!	�R���X�g���N�^
	MeshBase();
	//!	�R���X�g���N�^
	//!	@param	[in]	pParent	�e�I�u�W�F�N�g
	MeshBase(GameObjectBase* pParent);
	//!	�f�X�g���N�^
	virtual ~MeshBase();

	//!	�e�I�u�W�F�N�g�ݒ�
	//!	@param	[in]	pParent	�e�I�u�W�F�N�g
	void SetParent(GameObjectBase* pParent);

	//!	�`��
	virtual void Render() = 0;

	//!	�}�e���A���擾
	//!	@param	[in]	index	�}�e���A���ԍ�
	MaterialData*	GetMaterial(u32 index)
	{
		if (index >= _materials.size()) return nullptr;

		return &_materials[index];
	}

	//!	�}�e���A���̐����擾
	//!	@return	�}�e���A���̐�
	size_t GetMaterialSize() const
	{
		return _materials.size();
	}

	//!	�}�e���A���ǉ�
	//!	@param	[in]	material	�}�e���A��
	size_t AddMateiral(const MaterialData& material);
	//!	�}�e���A���ݒ�(��������)
	//!	@param	[in]	index		�}�e���A���ԍ�
	//!	@param	[in]	material	�}�e���A��
	void SetMaterial(u32 index, const MaterialData& material);

	//!	�}�e���A���̓K��
	//!	@param	[in]	index	�}�e���A���ԍ� default:0
	void BindMaterial(u32 index = 0);

	//!	���b�V���f�[�^�ݒ�
	//!	@param	[in]	pMeshData	���b�V���f�[�^
	void SetMeshData(MeshData* pMeshData)
	{
		_pMeshData.reset(pMeshData);
		_pMeshData->AddRef();
	}

	//!	���b�V���f�[�^�ݒ�
	//!	@param	[in]	pMesh	���b�V��
	void SetMeshData(MeshBase* pMesh)
	{
		_pMeshData.reset(pMesh->_pMeshData.get());
		_pMeshData->AddRef();
	}

protected:
	std::vector<MaterialData> _materials;		//!< �}�e���A��
	RefPointer<MeshData> _pMeshData;			//!< ���b�V���f�[�^
	GameObjectBase* _pParent;					//!< �e�I�u�W�F�N�g
};


//-------------------------------------------------------------------
//!	�R���|�[�l���g���N���X
//-------------------------------------------------------------------
class ProcessBase : public BasicBase
{
public:
	//!	�R���X�g���N�^
	ProcessBase();
	//!	�R���X�g���N�^
	//!	@param	[in]	pParent	�e�I�u�W�F�N�g
	ProcessBase(GameObjectBase* pParent);
	//!	�f�X�g���N�^
	virtual ~ProcessBase();

	//!	�e�I�u�W�F�N�g�ݒ�
	//!	@param	[in]	pParent	�e�I�u�W�F�N�g
	void SetParent(GameObjectBase* pParent);

	//!	�e�I�u�W�F�N�g�ݒ�
	//!	@param	[in]	pParent	�e�I�u�W�F�N�g
	GameObjectBase* GetParent(){ return _pParent; }

	//!	�X�V
	virtual void Update() = 0;

protected:
	GameObjectBase* _pParent;	//!< �e�I�u�W�F�N�g
};

