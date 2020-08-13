//=============================================================================
//!
//!	@file	Vbo.cpp
//!	@brief	Vbo
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

using namespace BufferObject;

//-----------------------------------------------------------------------------
//  @brief  コンストラクタ
//-----------------------------------------------------------------------------
Vbo::Desc::Desc()
: _vertexStride	(0)
, _vertexCount	(0)
, _pVertexData	(nullptr)
{
}

//-----------------------------------------------------------------------------
//  @brief  コンストラクタ
//  @param	[in]	stride	データ一つ分のサイズ
//  @param	[in]	count	配列サイズ
//  @param	[in]	pData	データポインタ
//  @param	[in]	list	バッファ情報リスト
//-----------------------------------------------------------------------------
Vbo::Desc::Desc( u32 vertexStride,
				 u32 vertexCount,
				 const void* pVertexData,
				 const std::vector<Element>& list)
: _vertexStride	(vertexStride)
, _vertexCount	(vertexCount)
, _pVertexData	(pVertexData)
, _elementList	(list)
{
}

//-----------------------------------------------------------------------------
//  @brief  頂点設定
//	@param	[in]	stride	データ一つ分のサイズ
//	@param	[in]	count	配列サイズ
//	@param	[in]	pData	データポインタ
//-----------------------------------------------------------------------------
void Vbo::Desc::SetVertexData(u32 stride, u32 count, const void* pData)
{
	_vertexStride = stride;
	_vertexCount = count;
	_pVertexData = pData;
}

//-----------------------------------------------------------------------------
//  @brief  バッファ情報追加
//  @param	[in]	pList			リストデータ
//  @param	[in]	elementCount	配列サイズ
//-----------------------------------------------------------------------------
void Vbo::Desc::SetElementList(const Element* pList, u32 elementCount)
{
	if (pList == nullptr || elementCount == 0) return;

	_elementList.clear();
	_elementList.resize(0);
	for (u32 i = 0; i<elementCount; ++i)
	{
		_elementList.push_back(pList[i]);
	}
}

//-----------------------------------------------------------------------------
//  @brief  バッファ情報追加
//  @param	[in]	list	バッファ情報リスト
//-----------------------------------------------------------------------------
void Vbo::Desc::SetElementList(const std::vector<Element>& list)
{
	const size_t count = list.size();
	if (count == 0) return;

	_elementList.clear();
	_elementList.resize(0);
	for (size_t i = 0; i<count; ++i)
	{
		_elementList.push_back(list[i]);
	}
}

//-----------------------------------------------------------------------------
//  @brief  コンストラクタ
//-----------------------------------------------------------------------------
Vbo::Vbo()
: _vboId(0)
, _vertexStride(0)
, _primitiveType(0)
{
}

//-----------------------------------------------------------------------------
//  @brief  デストラクタ
//-----------------------------------------------------------------------------
Vbo::~Vbo()
{
	glDeleteBuffers(1, &_vboId);
}

//-----------------------------------------------------------------------------
//  @brief  データ作成
//  @param	[in]	desc			バッファデータ
//  @param	[in]	primitiveType	描画方法
//  @retval	true	成功
//  @retval	false	失敗
//-----------------------------------------------------------------------------
bool Vbo::Create(const Desc& desc, PRIMITIVE_TYPE primitiveType)
{
	if (!desc._pVertexData)
	{
		return false;
	}

	// Vboの作成
	{
		glGenBuffers(1, &_vboId);
		glBindBuffer(GL_ARRAY_BUFFER, _vboId);
		glBufferData(
			GL_ARRAY_BUFFER,
			desc._vertexStride * desc._vertexCount,
			desc._pVertexData,
			GL_STATIC_DRAW
			);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	_vertexStride = desc._vertexStride;
	_elementList = desc._elementList;

	SetPrimitiveType(primitiveType);


	return true;
}

//-----------------------------------------------------------------------------
//  @brief  描画方法設定
//  @param	[in]	type	タイプ
//  @retval	true	成功
//  @retval	false	失敗
//-----------------------------------------------------------------------------
bool Vbo::SetPrimitiveType(PRIMITIVE_TYPE type)
{
	//	作業用タイプ
	u32 primitiveType = 0;

	switch (type)
	{
	case PRIMITIVE_TYPE::POINTS:
		primitiveType = GL_POINTS;
		break;
	case PRIMITIVE_TYPE::LINE:
		primitiveType = GL_LINES;
		break;
	case PRIMITIVE_TYPE::LINE_LOOP:
		primitiveType = GL_LINE_LOOP;
		break;
	case PRIMITIVE_TYPE::LINE_STRIP:
		primitiveType = GL_LINE_STRIP;
		break;
	case PRIMITIVE_TYPE::TRIANGLES:
		primitiveType = GL_TRIANGLES;
		break;
	case PRIMITIVE_TYPE::TRIANGLE_FAN:
		primitiveType = GL_TRIANGLE_FAN;
		break;
	case PRIMITIVE_TYPE::TRIANGLE_STRIP:
		primitiveType = GL_TRIANGLE_STRIP;
		break;
	case PRIMITIVE_TYPE::POLYGON:
		primitiveType = GL_POLYGON;
		break;
	case PRIMITIVE_TYPE::UNKNOWN:
	default:
		/* 未指定 */
		primitiveType = 0;
	}

	//	失敗時
	if (primitiveType == 0)
	{
		return false;
	}

	//	成功していれば更新
	_primitiveType = primitiveType;
	return true;
}

//-----------------------------------------------------------------------------
//  @brief  バッファ有効化
//-----------------------------------------------------------------------------
void Vbo::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vboId);
}
//-----------------------------------------------------------------------------
//  @brief  バッファ無効化
//-----------------------------------------------------------------------------
void Vbo::UnBind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//-----------------------------------------------------------------------------
//  @brief  入力レイアウト
//-----------------------------------------------------------------------------
void Vbo::CallInputLayout()
{
	const size_t count = _elementList.size();
	for (size_t i = 0; i<count; ++i)
	{
		const Element& p = _elementList[i];

		glVertexAttribPointer(
			p._locationIndex,
			p._count,
			(GLenum)p._elementType,
			p._isNormalize ? GL_TRUE : GL_FALSE,
			_vertexStride,
			(GLvoid*)p._strideOffset
			);

		glEnableVertexAttribArray(p._locationIndex);
	}
}

//-----------------------------------------------------------------------------
//  @brief  描画
//  @param	[in]	ibo	IndexBufferObject
//-----------------------------------------------------------------------------
void Vbo::Render(const Ibo& ibo)
{
	if (_primitiveType == 0)
	{
		return;
	}
	if (ibo._indexCount == 0 || ibo._indexType == 0)
	{
		return;
	}

	Bind();
	ibo.Bind();

	CallInputLayout();

	glDrawElements(_primitiveType, ibo._indexCount, ibo._indexType, nullptr);

	ibo.UnBind();
	UnBind();
}

//-----------------------------------------------------------------------------
//  @brief  一部描画 一部の描画なのでBindはしない
//  @param	[in]	ibo		IndexBufferObject
//  @param	[in]	start	開始頂点
//  @param	[in]	count	描画する頂点数
//-----------------------------------------------------------------------------
void Vbo::Render(const Ibo& ibo,u32 start, u32 count)
{
	if(_primitiveType == 0)
	{
		return;
	}
	if(ibo._indexCount == 0 || ibo._indexType == 0)
	{
		return;
	}

	glDrawRangeElementsBaseVertex(
		_primitiveType,
		start,
		start + count,
		count,
		ibo._indexType,
		nullptr,
		start
		);
}

//-----------------------------------------------------------------------------
//  @brief  描画
//  @param	[in]	ibo		IndexBufferObject
//  @param	[in]	start	開始頂点
//  @param	[in]	count	描画する頂点数
//  @param	[in]	size	描画する数
//-----------------------------------------------------------------------------
void Vbo::InstancedRender(const Ibo& ibo, u32 start, u32 count, u32 size)
{
	if(_primitiveType == 0)
	{
		return;
	}
	if(ibo._indexCount == 0 || ibo._indexType == 0)
	{
		return;
	}

	//	https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDrawElementsInstancedBaseVertexBaseInstance.xml
	//	https://www.opengl.org/wiki/GLAPI/glDrawElementsInstancedBaseVertexBaseInstance

	glDrawElementsInstancedBaseVertexBaseInstance(
		_primitiveType,
		count,
		ibo._indexType,
		nullptr,
		start + count,
		start,
		size
		);
}

