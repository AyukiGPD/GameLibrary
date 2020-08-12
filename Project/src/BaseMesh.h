//=============================================================================
//!
//!	@file	BaseMesh.h
//!	@brief	���b�V�����N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//-------------------------------------------------------------------
//! @brief  �}�e���A���J���[���[�h
//-------------------------------------------------------------------
enum class MATERIAL_COLOR
{
	VERTEX = 0,
	MATERIAL = 1,
	TEXTURE = 2
};

//-------------------------------------------------------------------
//! @brief  �}�e���A���f�[�^
//-------------------------------------------------------------------
struct MaterialData
{
	//! @brief  �R���X�g���N�^
	MaterialData()
	: _colorMode(MATERIAL_COLOR::MATERIAL)
	, _color	(128)
	, _roughness(0.0f)
	, _metalness(0.0f)
	, _emissiveColor(Vector3::ZERO)
	, _pTexture(nullptr)
	{
	}

	//! @brief  �R���X�g���N�^
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

	//! @brief  �f�X�g���N�^
	virtual ~MaterialData()
	{
	}

	//! @brief  �f�t�H���g�}�e���A�����
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
//! @brief  ���b�V�����N���X
//-------------------------------------------------------------------
class MeshBase : public BasicBase
{
public:
	//! @brief  �R���X�g���N�^
	MeshBase();

	//! @brief  �R���X�g���N�^
	//!	@param	[in]	pParent	�e�I�u�W�F�N�g
	MeshBase(GameObjectBase* pParent);

	//! @brief  �f�X�g���N�^
	virtual ~MeshBase();

	//! @brief  �e�I�u�W�F�N�g�ݒ�
	//!	@param	[in]	pParent	�e�I�u�W�F�N�g
	void SetParent(GameObjectBase* pParent);

	//! @brief  �`��
	virtual void Render() = 0;

	//! @brief  �}�e���A���擾
	//!	@param	[in]	index	�}�e���A���ԍ�
	MaterialData* GetMaterial(u32 index);

	//! @brief  �}�e���A���̐����擾
	//!	@return	�}�e���A���̐�
	size_t GetMaterialSize() const;

	//! @brief  �}�e���A���ǉ�
	//!	@param	[in]	material	�}�e���A��
	size_t AddMateiral(const MaterialData& material);

	//! @brief  �}�e���A���ݒ�(��������)
	//!	@param	[in]	index		�}�e���A���ԍ�
	//!	@param	[in]	material	�}�e���A��
	void SetMaterial(u32 index, const MaterialData& material);

	//! @brief  �}�e���A���̓K��
	//!	@param	[in]	index	�}�e���A���ԍ� default:0
	void BindMaterial(u32 index = 0);

	//! @brief  ���b�V���f�[�^�ݒ�
	//!	@param	[in]	pMeshData	���b�V���f�[�^
	void SetMeshData(MeshData* pMeshData);

	//! @brief  ���b�V���f�[�^�ݒ�
	//!	@param	[in]	pMesh	���b�V��
	void SetMeshData(MeshBase* pMesh);

protected:
	std::vector<MaterialData> _materials;		//!< �}�e���A��
	RefPointer<MeshData> _pMeshData;			//!< ���b�V���f�[�^
	GameObjectBase* _pParent;					//!< �e�I�u�W�F�N�g
};
