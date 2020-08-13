//=============================================================================
//!
//!	@file	VaoObject.cpp
//!	@brief	Vaoオブジェクト
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

using namespace BufferObject;

//-----------------------------------------------------------------------------
//  @brief  コンストラクタ
//-----------------------------------------------------------------------------
VaoObject::VaoObject()
:_pVao(nullptr)
{
	AddTag("VaoObject");
}

//-----------------------------------------------------------------------------
//  @brief  コンストラクタ
//  @param	[in]	desc			バッファデータ
//  @param	[in]	primitiveType	描画方法
//  @param	[in]	indexType		インデックスの型
//-----------------------------------------------------------------------------
VaoObject::VaoObject(const Vao::Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType, const MaterialData& material)
:_pVao(nullptr)
{
	_pVao = new Vao();
	if(!_pVao->Create(desc, primitiveType, indexType))
	{
		SafeRelease(_pVao);
	}
	else
	{
		_pVao->AddMateiral(material);
	}
	AddTag("VaoObject");
}

//-----------------------------------------------------------------------------
//  @brief  デストラクタ
//-----------------------------------------------------------------------------
VaoObject::~VaoObject()
{
	Clear();
}

//-----------------------------------------------------------------------------
//  @brief  解放
//-----------------------------------------------------------------------------
void VaoObject::Clear()
{
	SafeRelease(_pVao);
}

//-----------------------------------------------------------------------------
//  @brief  データ作成
//  @param	[in]	desc			バッファデータ
//  @param	[in]	primitiveType	描画方法
//  @param	[in]	indexType		インデックスの型
//-----------------------------------------------------------------------------
bool VaoObject::Init(const Vao::Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType, const MaterialData& materiial)
{
	_pVao = Vao::Create(desc, primitiveType, indexType);
	if (!_pVao)
	{
		return false;
	}
	AddMesh(_pVao);
	_pVao->AddMateiral(materiial);
	return true;
}

//-----------------------------------------------------------------------------
//  @brief  データ作成
//  @param	[in]	desc			バッファデータ
//  @param	[in]	primitiveType	描画方法
//  @param	[in]	indexType		インデックスの型
//-----------------------------------------------------------------------------
VaoObject* VaoObject::Create(const Vao::Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType, const MaterialData& materiial)
{
	VaoObject* result = nullptr;
	result = new VaoObject;
	if (!result->Init(desc, primitiveType, indexType,materiial))
	{
		SafeRelease(result);
	}
	return result;
}

//-----------------------------------------------------------------------------
//  @brief  描画
//-----------------------------------------------------------------------------
void VaoObject::Render()
{
}

//-----------------------------------------------------------------------------
//  @brief  データ有無
//-----------------------------------------------------------------------------
bool VaoObject::IsEmpty()
{
	return _pVao == nullptr;
}

//-----------------------------------------------------------------------------
//  @brief  Vao取得
//  @return	Vaoのポインタ
//-----------------------------------------------------------------------------
Vao* VaoObject::GetVao()
{
	return _pVao;
}


