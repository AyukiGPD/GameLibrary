//=============================================================================
//!
//!	@file	RendererBuffer.cpp
//!	@brief	描画バッファ
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

using namespace BufferObject;

//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
Vbo::Desc::Desc()
: _vertexStride	(0)
, _vertexCount	(0)
, _pVertexData	(nullptr)
{
}

//-----------------------------------------------------------------------------
//!	コンストラクタ
//!	@param	[in]	stride	データ一つ分のサイズ
//!	@param	[in]	count	配列サイズ
//!	@param	[in]	pData	データポインタ
//!	@param	[in]	list	バッファ情報リスト
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
//!	頂点設定
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
//!	バッファ情報追加
//!	@param	[in]	pList			リストデータ
//!	@param	[in]	elementCount	配列サイズ
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
//!	バッファ情報追加
//!	@param	[in]	list	バッファ情報リスト
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
//!	コンストラクタ
//-----------------------------------------------------------------------------
Vbo::Vbo()
: _vboId(0)
, _vertexStride(0)
, _primitiveType(0)
{
}
//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
Vbo::~Vbo()
{
	glDeleteBuffers(1, &_vboId);
}

//-----------------------------------------------------------------------------
//!	データ作成
//!	@param	[in]	desc			バッファデータ
//!	@param	[in]	primitiveType	描画方法
//!	@retval	true	成功
//!	@retval	false	失敗
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
//!	描画方法設定
//!	@param	[in]	type	タイプ
//!	@retval	true	成功
//!	@retval	false	失敗
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
//!	バッファ有効化
//-----------------------------------------------------------------------------
void Vbo::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vboId);
}
//-----------------------------------------------------------------------------
//!	バッファ無効化
//-----------------------------------------------------------------------------
void Vbo::UnBind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//-----------------------------------------------------------------------------
//!	入力レイアウト
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
//!	描画
//!	@param	[in]	ibo	IndexBufferObject
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
//! 一部描画
//!	一部の描画なのでBindはしない
//!	@param	[in]	ibo		IndexBufferObject
//!	@param	[in]	start	開始頂点
//!	@param	[in]	count	描画する頂点数
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
//! 描画
//!	@param	[in]	ibo		IndexBufferObject
//!	@param	[in]	start	開始頂点
//!	@param	[in]	count	描画する頂点数
//!	@param	[in]	size	描画する数
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

//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
Ibo::Desc::Desc()
: _indexStride	(0)
, _indexCount	(0)
, _pIndexData	(nullptr)
{
}

//-----------------------------------------------------------------------------
//!	コンストラクタ
//!	@param	[in]	stride	データ一つ分のサイズ
//!	@param	[in]	count	配列サイズ
//!	@param	[in]	pData	データポインタ
//-----------------------------------------------------------------------------
Ibo::Desc::Desc(u32 stride, u32 count, const void* pData)
: _indexStride	(stride)
, _indexCount	(count)
, _pIndexData	(pData)
{
}

//-----------------------------------------------------------------------------
//!	インデックス設定
//!	@param	[in]	stride	データ一つ分のサイズ
//!	@param	[in]	count	配列サイズ
//!	@param	[in]	pData	データポインタ
//-----------------------------------------------------------------------------
void Ibo::Desc::SetIndexData(u32 stride, u32 count, const void* pData)
{
	_indexStride = stride;
	_indexCount = count;
	_pIndexData = pData;
}

//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
Ibo::Ibo()
: _iboId(0)
, _indexType(0)
, _indexCount(0)
{
}
//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
Ibo::~Ibo()
{
	glDeleteBuffers(1, &_iboId);
}

//-----------------------------------------------------------------------------
//!	データ作成
//!	@param	[in]	desc			バッファデータ
//!	@param	[in]	indexType		インデックスの型
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
//!	バッファ有効化
//-----------------------------------------------------------------------------
void Ibo::Bind()const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboId);
}

//-----------------------------------------------------------------------------
//!	バッファ無効化
//-----------------------------------------------------------------------------
void Ibo::UnBind()const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


//-----------------------------------------------------------------------------
//!	コンストラクタ
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
//!	頂点設定
//!	@param	[in]	stride	データ一つ分のサイズ
//!	@param	[in]	count	配列サイズ
//!	@param	[in]	pData	データポインタ
//-----------------------------------------------------------------------------
void Vao::Desc::SetVertexData(u32 stride, u32 count, const void* pData)
{
	_vertexStride = stride;
	_vertexCount  = count;
	_pVertexData  = pData;
}

//-----------------------------------------------------------------------------
//!	インデックス設定
//!	@param	[in]	stride	データ一つ分のサイズ
//!	@param	[in]	count	配列サイズ
//!	@param	[in]	pData	データポインタ
//-----------------------------------------------------------------------------
void Vao::Desc::SetIndexData(u32 stride, u32 count, const void* pData)
{
	_indexStride = stride;
	_indexCount  = count;
	_pIndexData  = pData;
}

//-----------------------------------------------------------------------------
//!	バッファ情報追加
//!	@param	[in]	pList			リストデータ
//!	@param	[in]	elementCount	配列サイズ
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
//!	バッファ情報追加
//!	@param	[in]	list	リストデータ
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
//!	コンストラクタ
//-----------------------------------------------------------------------------
Vao::Vao()
: _vao(0)
, _pVbo(nullptr)
, _pIbo(nullptr)
{
	AddTag("Vao");
}
//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
Vao::~Vao()
{
	SafeDelete(_pVbo);
	SafeDelete(_pIbo);
	glDeleteVertexArrays(1,&_vao);
}


//-----------------------------------------------------------------------------
//!	データ作成
//!	@param	[in]	desc			バッファデータ
//!	@param	[in]	primitiveType	描画方法
//!	@param	[in]	indexType		インデックスの型
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
//!	データ作成
//!	@param	[in]	desc			バッファデータ
//!	@param	[in]	primitiveType	描画方法
//!	@param	[in]	indexType		インデックスの型
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
//!	描画
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
//!	コンストラクタ
//-----------------------------------------------------------------------------
VaoObject::VaoObject()
:_pVao(nullptr)
{
	AddTag("VaoObject");
}

//-----------------------------------------------------------------------------
//!	コンストラクタ
//!	@param	[in]	desc			バッファデータ
//!	@param	[in]	primitiveType	描画方法
//!	@param	[in]	indexType		インデックスの型
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
//!	デストラクタ
//-----------------------------------------------------------------------------
VaoObject::~VaoObject()
{
	Clear();
}

//-----------------------------------------------------------------------------
//!	解放
//-----------------------------------------------------------------------------
void VaoObject::Clear()
{
	SafeRelease(_pVao);
}


//-----------------------------------------------------------------------------
//!	データ作成
//!	@param	[in]	desc			バッファデータ
//!	@param	[in]	primitiveType	描画方法
//!	@param	[in]	indexType		インデックスの型
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
//!	データ作成
//!	@param	[in]	desc			バッファデータ
//!	@param	[in]	primitiveType	描画方法
//!	@param	[in]	indexType		インデックスの型
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
//!	描画
//-----------------------------------------------------------------------------
void VaoObject::Render()
{
}

static const f32 gBoxSize = 1.0f;

static const Vertex gBoxVertices[] =
{
	// 前
	{ Vector3(-gBoxSize, +gBoxSize, +gBoxSize), Vector3(0, 0, +1), Color(255), Vector2(0, 1) },
	{ Vector3(+gBoxSize, +gBoxSize, +gBoxSize), Vector3(0, 0, +1), Color(255), Vector2(1, 1) },
	{ Vector3(-gBoxSize, -gBoxSize, +gBoxSize), Vector3(0, 0, +1), Color(255), Vector2(0, 0) },
	{ Vector3(+gBoxSize, -gBoxSize, +gBoxSize), Vector3(0, 0, +1), Color(255), Vector2(1, 0) },

	// 後
	{ Vector3(+gBoxSize, +gBoxSize, -gBoxSize), Vector3(0, 0, -1), Color(255), Vector2(1, 1) },
	{ Vector3(-gBoxSize, +gBoxSize, -gBoxSize), Vector3(0, 0, -1), Color(255), Vector2(0, 1) },
	{ Vector3(+gBoxSize, -gBoxSize, -gBoxSize), Vector3(0, 0, -1), Color(255), Vector2(1, 0) },
	{ Vector3(-gBoxSize, -gBoxSize, -gBoxSize), Vector3(0, 0, -1), Color(255), Vector2(0, 0) },

	// 左
	{ Vector3(-gBoxSize, +gBoxSize, -gBoxSize), Vector3(-1, 0, 0), Color(255), Vector2(1, 0) },
	{ Vector3(-gBoxSize, +gBoxSize, +gBoxSize), Vector3(-1, 0, 0), Color(255), Vector2(1, 1) },
	{ Vector3(-gBoxSize, -gBoxSize, -gBoxSize), Vector3(-1, 0, 0), Color(255), Vector2(0, 0) },
	{ Vector3(-gBoxSize, -gBoxSize, +gBoxSize), Vector3(-1, 0, 0), Color(255), Vector2(0, 1) },

	// 右
	{ Vector3(+gBoxSize, +gBoxSize, +gBoxSize), Vector3(+1, 0, 0), Color(255), Vector2(1, 1) },
	{ Vector3(+gBoxSize, +gBoxSize, -gBoxSize), Vector3(+1, 0, 0), Color(255), Vector2(1, 0) },
	{ Vector3(+gBoxSize, -gBoxSize, +gBoxSize), Vector3(+1, 0, 0), Color(255), Vector2(0, 1) },
	{ Vector3(+gBoxSize, -gBoxSize, -gBoxSize), Vector3(+1, 0, 0), Color(255), Vector2(0, 0) },

	// 上
	{ Vector3(-gBoxSize, +gBoxSize, -gBoxSize), Vector3(0, +1, 0), Color(255), Vector2(0, 0) },
	{ Vector3(+gBoxSize, +gBoxSize, -gBoxSize), Vector3(0, +1, 0), Color(255), Vector2(1, 0) },
	{ Vector3(-gBoxSize, +gBoxSize, +gBoxSize), Vector3(0, +1, 0), Color(255), Vector2(0, 1) },
	{ Vector3(+gBoxSize, +gBoxSize, +gBoxSize), Vector3(0, +1, 0), Color(255), Vector2(1, 1) },

	// 下
	{ Vector3(-gBoxSize, -gBoxSize, +gBoxSize), Vector3(0, -1, 0), Color(255), Vector2(0, 1) },
	{ Vector3(+gBoxSize, -gBoxSize, +gBoxSize), Vector3(0, -1, 0), Color(255), Vector2(1, 1) },
	{ Vector3(-gBoxSize, -gBoxSize, -gBoxSize), Vector3(0, -1, 0), Color(255), Vector2(0, 0) },
	{ Vector3(+gBoxSize, -gBoxSize, -gBoxSize), Vector3(0, -1, 0), Color(255), Vector2(1, 0) },
};
static const u32 gBoxIndices[] =
{
	0,  1,  2,  1,  3,  2,
	4,  5,  6,  5,  7,  6,
	8,  9, 10,  9, 11, 10,
	12, 13, 14, 13, 15, 14,
	16, 17, 18, 17, 19, 18,
	20, 21, 22, 21, 23, 22, 
};

//	バッファ情報
BufferObject::Element gBoxElements[] =
{
	{ 0, 3, BufferObject::ELEMENT_TYPE::F32, false, offsetof(Vertex, _position) },
	{ 1, 3, BufferObject::ELEMENT_TYPE::F32, false, offsetof(Vertex, _normal) },
	{ 2, 4, BufferObject::ELEMENT_TYPE::U8,  true , offsetof(Vertex, _color) },
	{ 3, 2, BufferObject::ELEMENT_TYPE::F32, false, offsetof(Vertex, _uv) },
};

RefPointer<BufferObject::Vao> BoxMesh::_pStaticBoxMesh = nullptr;

//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
BoxMesh::BoxMesh()
{
	if (!_pStaticBoxMesh)
	{
		const int vCount = ARRAY_COUNT(gBoxVertices);
		const int iCount = ARRAY_COUNT(gBoxIndices);
		const int eCount = ARRAY_COUNT(gBoxElements);

		BufferObject::Vao::Desc desc;
		desc.SetVertexData(sizeof(Vertex), vCount, &gBoxVertices[0]);
		desc.SetIndexData(sizeof(u32), iCount, &gBoxIndices[0]);
		desc.SetElementList(gBoxElements, eCount);

		_pStaticBoxMesh = CreateRefPointerInit<BufferObject::Vao>(
				desc,
				BufferObject::PRIMITIVE_TYPE::TRIANGLES,
				BufferObject::INDEX_TYPE::TYPE_32
			);
	}
}

//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
BoxMesh::~BoxMesh()
{
}

//-----------------------------------------------------------------------------
//!	コンストラクタ
//!	@param	[in]	pParent	親オブジェクト
//-----------------------------------------------------------------------------
BoxMesh::BoxMesh(const MaterialData& material)
{
	if (!_pStaticBoxMesh)
	{
		const int vCount = ARRAY_COUNT(gBoxVertices);
		const int iCount = ARRAY_COUNT(gBoxIndices);
		const int eCount = ARRAY_COUNT(gBoxElements);

		BufferObject::Vao::Desc desc;
		desc.SetVertexData(sizeof(Vertex), vCount, &gBoxVertices[0]);
		desc.SetIndexData(sizeof(u32), iCount, &gBoxIndices[0]);
		desc.SetElementList(gBoxElements, eCount);

		_pStaticBoxMesh = CreateRefPointerInit<BufferObject::Vao>(
			desc,
			BufferObject::PRIMITIVE_TYPE::TRIANGLES,
			BufferObject::INDEX_TYPE::TYPE_32
			);
	}

	SetMaterial(0, material);
}

//-----------------------------------------------------------------------------
//!	描画
//-----------------------------------------------------------------------------
void BoxMesh::Render()
{
	if (!_pStaticBoxMesh){ assert(false); }

	BindMaterial(0);
	_pStaticBoxMesh->Render();

}


