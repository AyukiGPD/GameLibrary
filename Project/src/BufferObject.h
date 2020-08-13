//=============================================================================
//!
//!	@file	BufferObject.h
//!	@brief	描画バッファ情報
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//!	Vbo Ibo Vao
namespace BufferObject
{

//-----------------------------------------------------------------------------
//! @brief  データ型
//-----------------------------------------------------------------------------
enum class ELEMENT_TYPE
{
	UNKNOWN	= 0,
	S8		= GL_BYTE,
	U8		= GL_UNSIGNED_BYTE,
	S16		= GL_SHORT,
	U16		= GL_UNSIGNED_SHORT,
	S32		= GL_INT,
	U32		= GL_UNSIGNED_INT,
	F32		= GL_FLOAT,
};

//-----------------------------------------------------------------------------
//! @brief  描画方法
//-----------------------------------------------------------------------------
enum class PRIMITIVE_TYPE
{
	UNKNOWN,
	POINTS,
	LINE,
	LINE_LOOP,
	LINE_STRIP,
	TRIANGLES,
	TRIANGLE_FAN,
	TRIANGLE_STRIP,
	POLYGON,
};

//-----------------------------------------------------------------------------
//! @brief  インデックスの型
//-----------------------------------------------------------------------------
enum class INDEX_TYPE
{
	TYPE_16,
	TYPE_32
};

//-----------------------------------------------------------------------------
//! @brief  バッファ情報
//-----------------------------------------------------------------------------
struct Element
{
	u32				_locationIndex;
	u32				_count;
	ELEMENT_TYPE	_elementType;
	bool			_isNormalize;
	size_t			_strideOffset;
};

};	//	namespace BufferObject

//! @brief  配列サイズ
#define ARRAY_COUNT(ARRAY_) (sizeof(ARRAY_) / sizeof(ARRAY_[0]))

//-----------------------------------------------------------------------------
//! @brief  頂点データ
//-----------------------------------------------------------------------------
struct Vertex
{
	//! @brief  コンストラクタ
	Vertex()
	: _position	(Vector3(0.0f,0.0f,0.0f))
	, _normal	(Vector3(0.0f, 0.0f, 0.0f))
	, _color	(Color(255))
	, _uv		(Vector2(-1.0f,-1.0f))
	{}

	//! @brief  コンストラクタ
	//!	@param	[in]	position	座標
	//!	@param	[in]	normal		法線
	//!	@param	[in]	color		色
	//!	@param	[in]	uv			UV座標
	Vertex(const Vector3& position, const Vector3& normal, const Color& color, const Vector2& uv)
	: _position(position)
	, _normal(normal)
	, _color(color)
	, _uv(uv)
	{}

	Vector3 _position;	//!< 頂点
	Vector3 _normal;	//!< 法線
	Color	_color;		//!< 色
	Vector2	_uv;		//!< UV座標
};


