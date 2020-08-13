//=============================================================================
//!
//!	@file	Vao.cpp
//!	@brief	Vao
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

using namespace BufferObject;

//-----------------------------------------------------------------------------
//  @brief  コンストラクタ
//-----------------------------------------------------------------------------
Vao::Desc::Desc()
: _vertexStride	(0)
, _vertexCount	(0)
, _pVertexData	(nullptr)
, _indexStride	(0)
, _indexCount	(0)
, _pIndexData	(nullptr)
{
	_elementList.resize(0);
}

//-----------------------------------------------------------------------------
//  @brief  頂点設定
//  @param	[in]	stride	データ一つ分のサイズ
//  @param	[in]	count	配列サイズ
//  @param	[in]	pData	データポインタ
//-----------------------------------------------------------------------------
void Vao::Desc::SetVertexData(u32 stride, u32 count, const void* pData)
{
	_vertexStride = stride;
	_vertexCount  = count;
	_pVertexData  = pData;
}

//-----------------------------------------------------------------------------
//  @brief  インデックス設定
//  @param	[in]	stride	データ一つ分のサイズ
//  @param	[in]	count	配列サイズ
//  @param	[in]	pData	データポインタ
//-----------------------------------------------------------------------------
void Vao::Desc::SetIndexData(u32 stride, u32 count, const void* pData)
{
	_indexStride = stride;
	_indexCount  = count;
	_pIndexData  = pData;
}

//-----------------------------------------------------------------------------
//  @brief  バッファ情報追加
//  @param	[in]	pList			リストデータ
//  @param	[in]	elementCount	配列サイズ
//-----------------------------------------------------------------------------
void Vao::Desc::SetElementList(const Element* pList, u32 elementCount)
{
	if( pList == nullptr || elementCount == 0 ) return;

	_elementList.clear();
	_elementList.resize(0);
	for(u32 i=0; i<elementCount; ++i)
	{
		_elementList.push_back(pList[i]);
	}
}

//-----------------------------------------------------------------------------
//  @brief  バッファ情報追加
//  @param	[in]	list	リストデータ
//-----------------------------------------------------------------------------
void Vao::Desc::SetElementList(const std::vector<Element>& list)
{
	const size_t count = list.size();
	if( count == 0 ) return;

	_elementList.clear();
	_elementList.resize(0);
	for (size_t i = 0; i < count; ++i)
	{
		_elementList.push_back(list[i]);
	}
}

//-----------------------------------------------------------------------------
//  @brief  コンストラクタ
//-----------------------------------------------------------------------------
Vao::Vao()
: _vao(0)
, _pVbo(nullptr)
, _pIbo(nullptr)
{
	AddTag("Vao");
}

//-----------------------------------------------------------------------------
//  @brief  コンストラクタ
//  @param	[in]	desc			バッファデータ
//  @param	[in]	primitiveType	描画方法
//  @param	[in]	indexType		インデックスの型
//-----------------------------------------------------------------------------
Vao::Vao(const Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType)
{
	Init(desc, primitiveType, indexType);
}
//-----------------------------------------------------------------------------
//  @brief  デストラクタ
//-----------------------------------------------------------------------------
Vao::~Vao()
{
	SafeDelete(_pVbo);
	SafeDelete(_pIbo);
	glDeleteVertexArrays(1,&_vao);
}


//-----------------------------------------------------------------------------
//  @brief  データ作成
//  @param	[in]	desc			バッファデータ
//  @param	[in]	primitiveType	描画方法
//  @param	[in]	indexType		インデックスの型
//-----------------------------------------------------------------------------
bool Vao::Init(const Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType)
{
	if (!desc._pVertexData)
	{
		return false;
	}
	if (!desc._pIndexData)
	{
		return false;
	}
	// Vaoの作成
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	bool result = true;
	Vbo* pVbo = new Vbo();
	if (pVbo)
	{
		if (!pVbo->Create((Vbo::Desc)desc, primitiveType))
		{
			result = false;
		}
	}
	else{ result = false; }

	Ibo* pIbo = new Ibo();
	if (pIbo)
	{
		if (!pIbo->Create((Ibo::Desc)desc, indexType))
		{
			result = false;
		}
	}
	else
	{
		result = false;
	}
	if (result)
	{
		_pVbo = pVbo;

		_pVbo->Bind();
		_pVbo->CallInputLayout();
		_pVbo->UnBind();

		_pIbo = pIbo;
	}
	else
	{
		SafeDelete(pVbo);
		SafeDelete(pIbo);
	}
	glBindVertexArray(0);

	return result;
}

//-----------------------------------------------------------------------------
//  @brief  データ作成
//  @param	[in]	desc			バッファデータ
//  @param	[in]	primitiveType	描画方法
//  @param	[in]	indexType		インデックスの型
//-----------------------------------------------------------------------------
Vao* Vao::Create(const Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType)
{
	Vao* result = nullptr;
	result = new Vao;

	if (!result->Init(desc, primitiveType, indexType))
	{
		SafeRelease(result);
	}

	return result;
}

//-----------------------------------------------------------------------------
//  @brief  描画
//-----------------------------------------------------------------------------
void Vao::Render()
{
	if (!_pVbo || !_pIbo)
	{
		return;
	}

	BindMaterial();

	u32 primitiveType = _pVbo->_primitiveType;
	u32 indexCount = _pIbo->_indexCount;
	u32 indexType = _pIbo->_indexType;

	if (primitiveType == 0 || indexType == 0 || indexCount == 0)
	{
		return;
	}


	// 描画
	glBindVertexArray(_vao);
	glDrawElements(primitiveType, indexCount, indexType, nullptr);
	isErrorGL();

	glBindVertexArray(0);

//	_pVbo->Render(*_pIbo);
}

//-----------------------------------------------------------------------------
//! @brief  Vboバッファデータ
//-----------------------------------------------------------------------------
Vao::Desc::operator Vbo::Desc() const
{
	Vbo::Desc desc(_vertexStride, _vertexCount, _pVertexData, _elementList);
	return desc;
}

//-----------------------------------------------------------------------------
//! @brief  Iboバッファデータ
//-----------------------------------------------------------------------------
Vao::Desc::operator Ibo::Desc() const
{
	Ibo::Desc desc(_indexStride, _indexCount, _pIndexData);
	return desc;
}
