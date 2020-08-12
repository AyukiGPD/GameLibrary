//=============================================================================
//!
//!	@file	BaseMesh.cpp
//!	@brief	メッシュ基底クラス
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//  @brief  デフォルトマテリアル情報
const MaterialData MaterialData::DEFAULT = MaterialData(MATERIAL_COLOR::MATERIAL, Color(128), 0.0f, 0.0f, Vector3::ONE, nullptr);

//-----------------------------------------------------------------------------
//  @brief  コンストラクタ
//-----------------------------------------------------------------------------
MeshBase::MeshBase()
:_pParent(nullptr)
{
}
//-----------------------------------------------------------------------------
//  @brief  コンストラクタ
//	@param	[in]	pParent	親オブジェクト
//-----------------------------------------------------------------------------
MeshBase::MeshBase(GameObjectBase* pParent)
: _pParent(pParent)
{
}

//-----------------------------------------------------------------------------
//  @brief  デストラクタ
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
//  @brief  親オブジェクト設定
// 	@param	[in]	pParent	親オブジェクト
//-----------------------------------------------------------------------------
void MeshBase::SetParent(GameObjectBase* pParent)
{
	if(!pParent){ return; }
	_pParent = pParent;
}

//-----------------------------------------------------------------------------
//  @brief  マテリアル追加
//  @param	[in]	material	マテリアル
//-----------------------------------------------------------------------------
size_t MeshBase::AddMateiral(const MaterialData& material)
{
	size_t result = _materials.size();
	_materials.push_back(material);
	return result;
}

//-----------------------------------------------------------------------------
//  @brief  マテリアル設定(書き換え)
//  @param	[in]	index		マテリアル番号
//  @param	[in]	material	マテリアル
//-----------------------------------------------------------------------------
void MeshBase::SetMaterial(u32 index, const MaterialData& material)
{
	MaterialData* pMaterial = GetMaterial(index);
	if (!pMaterial){ return; }
	_materials[index] = material;
}

/*!
	@todo
	マテリアルは配列で全てシェーダーに渡すことにする
	マテリアルの選択は配列番号(この関数のindex)で選択
	実装はShaderManager
	ここではShaderManagerにマテリアルの配列を送る
	ついでにUnBindMaterialも作る

	設計に時間がかかるかもしれないので後回し
*/
//-----------------------------------------------------------------------------
//  @brief  マテリアルの適応
//  @param	[in]	index	マテリアル番号 default:0
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

