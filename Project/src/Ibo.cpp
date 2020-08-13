//=============================================================================
//!
//!	@file	Ibo.cpp
//!	@brief	Ibo
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

using namespace BufferObject;

//-----------------------------------------------------------------------------
//  @brief  コンストラクタ
//-----------------------------------------------------------------------------
Ibo::Desc::Desc()
: _indexStride	(0)
, _indexCount	(0)
, _pIndexData	(nullptr)
{
}

//-----------------------------------------------------------------------------
//  @brief  コンストラクタ
//  @param	[in]	stride	データ一つ分のサイズ
//  @param	[in]	count	配列サイズ
//  @param	[in]	pData	データポインタ
//-----------------------------------------------------------------------------
Ibo::Desc::Desc(u32 stride, u32 count, const void* pData)
: _indexStride	(stride)
, _indexCount	(count)
, _pIndexData	(pData)
{
}

//-----------------------------------------------------------------------------
//  @brief  インデックス設定
//  @param	[in]	stride	データ一つ分のサイズ
//  @param	[in]	count	配列サイズ
//  @param	[in]	pData	データポインタ
//-----------------------------------------------------------------------------
void Ibo::Desc::SetIndexData(u32 stride, u32 count, const void* pData)
{
	_indexStride = stride;
	_indexCount = count;
	_pIndexData = pData;
}

//-----------------------------------------------------------------------------
//  @brief  コンストラクタ
//-----------------------------------------------------------------------------
Ibo::Ibo()
: _iboId(0)
, _indexType(0)
, _indexCount(0)
{
}

//-----------------------------------------------------------------------------
//  @brief  デストラクタ
//-----------------------------------------------------------------------------
Ibo::~Ibo()
{
	glDeleteBuffers(1, &_iboId);
}

//-----------------------------------------------------------------------------
//  @brief  データ作成
//  @param	[in]	desc			バッファデータ
//  @param	[in]	indexType		インデックスの型
//-----------------------------------------------------------------------------
bool Ibo::Create(const Desc& desc, INDEX_TYPE indexType)
{
	if (!desc._pIndexData)
	{
		return false;
	}

	// Iboの作成
	{
		glGenBuffers(1, &_iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboId);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			desc._indexStride * desc._indexCount,
			desc._pIndexData,
			GL_STATIC_DRAW
			);
	}

	switch (indexType)
	{
	case INDEX_TYPE::TYPE_16:
		_indexType = GL_UNSIGNED_SHORT;
		break;
	case INDEX_TYPE::TYPE_32:
		_indexType = GL_UNSIGNED_INT;
		break;
	default:
		/* 未指定 */
		_indexType = 0;
		return false;
	}

	//	インデックス数取得
	_indexCount = desc._indexCount;
	return true;
}


//-----------------------------------------------------------------------------
//  @brief  バッファ有効化
//-----------------------------------------------------------------------------
void Ibo::Bind()const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboId);
}

//-----------------------------------------------------------------------------
//  @brief  バッファ無効化
//-----------------------------------------------------------------------------
void Ibo::UnBind()const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


