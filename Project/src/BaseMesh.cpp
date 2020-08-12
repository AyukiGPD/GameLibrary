//=============================================================================
//!
//!	@file	BaseMesh.cpp
//!	@brief	���b�V�����N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//  @brief  �f�t�H���g�}�e���A�����
const MaterialData MaterialData::DEFAULT = MaterialData(MATERIAL_COLOR::MATERIAL, Color(128), 0.0f, 0.0f, Vector3::ONE, nullptr);

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//-----------------------------------------------------------------------------
MeshBase::MeshBase()
:_pParent(nullptr)
{
}
//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//	@param	[in]	pParent	�e�I�u�W�F�N�g
//-----------------------------------------------------------------------------
MeshBase::MeshBase(GameObjectBase* pParent)
: _pParent(pParent)
{
}

//-----------------------------------------------------------------------------
//  @brief  �f�X�g���N�^
//-----------------------------------------------------------------------------
MeshBase::~MeshBase()
{
	for(auto& material : _materials)
	{
		SafeDelete(material._pTexture);
	}
	_materials.clear();
}

//-----------------------------------------------------------------------------
//  @brief  �e�I�u�W�F�N�g�ݒ�
// 	@param	[in]	pParent	�e�I�u�W�F�N�g
//-----------------------------------------------------------------------------
void MeshBase::SetParent(GameObjectBase* pParent)
{
	if(!pParent){ return; }
	_pParent = pParent;
}

//-----------------------------------------------------------------------------
//  @brief  �}�e���A���ǉ�
//  @param	[in]	material	�}�e���A��
//-----------------------------------------------------------------------------
size_t MeshBase::AddMateiral(const MaterialData& material)
{
	size_t result = _materials.size();
	_materials.push_back(material);
	return result;
}

//-----------------------------------------------------------------------------
//  @brief  �}�e���A���ݒ�(��������)
//  @param	[in]	index		�}�e���A���ԍ�
//  @param	[in]	material	�}�e���A��
//-----------------------------------------------------------------------------
void MeshBase::SetMaterial(u32 index, const MaterialData& material)
{
	MaterialData* pMaterial = GetMaterial(index);
	if (!pMaterial){ return; }
	_materials[index] = material;
}

/*!
	@todo
	�}�e���A���͔z��őS�ăV�F�[�_�[�ɓn�����Ƃɂ���
	�}�e���A���̑I���͔z��ԍ�(���̊֐���index)�őI��
	������ShaderManager
	�����ł�ShaderManager�Ƀ}�e���A���̔z��𑗂�
	���ł�UnBindMaterial�����

	�݌v�Ɏ��Ԃ������邩������Ȃ��̂Ō��
*/
//-----------------------------------------------------------------------------
//  @brief  �}�e���A���̓K��
//  @param	[in]	index	�}�e���A���ԍ� default:0
//-----------------------------------------------------------------------------
void MeshBase::BindMaterial(u32 index)
{
	MaterialData* pMaterial = GetMaterial(index);
	if (!pMaterial)
	{
		return;
	}
	auto* pShader = Manager::Shader();

	switch (pMaterial->_colorMode)
	{
	case MATERIAL_COLOR::MATERIAL:
		pShader->SetSubroutine(SHADER_TYPE::FS, "Material");
		break;
	case MATERIAL_COLOR::VERTEX:
		pShader->SetSubroutine(SHADER_TYPE::FS, "Vertex");
		break;
	case MATERIAL_COLOR::TEXTURE:
		pShader->SetSubroutine(SHADER_TYPE::FS, "Texture");
		break;
	}


	pShader->SetUniform(11, pMaterial->_color);
	pShader->SetUniform(12, pMaterial->_roughness);
	pShader->SetUniform(13, pMaterial->_metalness);

	pShader->SetUniform(14, pMaterial->_emissiveColor);
}

