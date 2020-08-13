//=============================================================================
//!
//!	@file	BufferObject.h
//!	@brief	�`��o�b�t�@���
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//!	Vbo Ibo Vao
namespace BufferObject
{

//-----------------------------------------------------------------------------
//! @brief  �f�[�^�^
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
//! @brief  �`����@
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
//! @brief  �C���f�b�N�X�̌^
//-----------------------------------------------------------------------------
enum class INDEX_TYPE
{
	TYPE_16,
	TYPE_32
};

//-----------------------------------------------------------------------------
//! @brief  �o�b�t�@���
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

//! @brief  �z��T�C�Y
#define ARRAY_COUNT(ARRAY_) (sizeof(ARRAY_) / sizeof(ARRAY_[0]))

//-----------------------------------------------------------------------------
//! @brief  ���_�f�[�^
//-----------------------------------------------------------------------------
struct Vertex
{
	//! @brief  �R���X�g���N�^
	Vertex()
	: _position	(Vector3(0.0f,0.0f,0.0f))
	, _normal	(Vector3(0.0f, 0.0f, 0.0f))
	, _color	(Color(255))
	, _uv		(Vector2(-1.0f,-1.0f))
	{}

	//! @brief  �R���X�g���N�^
	//!	@param	[in]	position	���W
	//!	@param	[in]	normal		�@��
	//!	@param	[in]	color		�F
	//!	@param	[in]	uv			UV���W
	Vertex(const Vector3& position, const Vector3& normal, const Color& color, const Vector2& uv)
	: _position(position)
	, _normal(normal)
	, _color(color)
	, _uv(uv)
	{}

	Vector3 _position;	//!< ���_
	Vector3 _normal;	//!< �@��
	Color	_color;		//!< �F
	Vector2	_uv;		//!< UV���W
};


