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
//  @brief  �R���X�g���N�^
//-----------------------------------------------------------------------------
Vbo::Desc::Desc()
: _vertexStride	(0)
, _vertexCount	(0)
, _pVertexData	(nullptr)
{
}

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//  @param	[in]	stride	�f�[�^����̃T�C�Y
//  @param	[in]	count	�z��T�C�Y
//  @param	[in]	pData	�f�[�^�|�C���^
//  @param	[in]	list	�o�b�t�@��񃊃X�g
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
//  @brief  ���_�ݒ�
//	@param	[in]	stride	�f�[�^����̃T�C�Y
//	@param	[in]	count	�z��T�C�Y
//	@param	[in]	pData	�f�[�^�|�C���^
//-----------------------------------------------------------------------------
void Vbo::Desc::SetVertexData(u32 stride, u32 count, const void* pData)
{
	_vertexStride = stride;
	_vertexCount = count;
	_pVertexData = pData;
}

//-----------------------------------------------------------------------------
//  @brief  �o�b�t�@���ǉ�
//  @param	[in]	pList			���X�g�f�[�^
//  @param	[in]	elementCount	�z��T�C�Y
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
//  @brief  �o�b�t�@���ǉ�
//  @param	[in]	list	�o�b�t�@��񃊃X�g
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
//  @brief  �R���X�g���N�^
//-----------------------------------------------------------------------------
Vbo::Vbo()
: _vboId(0)
, _vertexStride(0)
, _primitiveType(0)
{
}

//-----------------------------------------------------------------------------
//  @brief  �f�X�g���N�^
//-----------------------------------------------------------------------------
Vbo::~Vbo()
{
	glDeleteBuffers(1, &_vboId);
}

//-----------------------------------------------------------------------------
//  @brief  �f�[�^�쐬
//  @param	[in]	desc			�o�b�t�@�f�[�^
//  @param	[in]	primitiveType	�`����@
//  @retval	true	����
//  @retval	false	���s
//-----------------------------------------------------------------------------
bool Vbo::Create(const Desc& desc, PRIMITIVE_TYPE primitiveType)
{
	if (!desc._pVertexData)
	{
		return false;
	}

	// Vbo�̍쐬
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
//  @brief  �`����@�ݒ�
//  @param	[in]	type	�^�C�v
//  @retval	true	����
//  @retval	false	���s
//-----------------------------------------------------------------------------
bool Vbo::SetPrimitiveType(PRIMITIVE_TYPE type)
{
	//	��Ɨp�^�C�v
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
		/* ���w�� */
		primitiveType = 0;
	}

	//	���s��
	if (primitiveType == 0)
	{
		return false;
	}

	//	�������Ă���΍X�V
	_primitiveType = primitiveType;
	return true;
}

//-----------------------------------------------------------------------------
//  @brief  �o�b�t�@�L����
//-----------------------------------------------------------------------------
void Vbo::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vboId);
}
//-----------------------------------------------------------------------------
//  @brief  �o�b�t�@������
//-----------------------------------------------------------------------------
void Vbo::UnBind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//-----------------------------------------------------------------------------
//  @brief  ���̓��C�A�E�g
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
//  @brief  �`��
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
//  @brief  �ꕔ�`�� �ꕔ�̕`��Ȃ̂�Bind�͂��Ȃ�
//  @param	[in]	ibo		IndexBufferObject
//  @param	[in]	start	�J�n���_
//  @param	[in]	count	�`�悷�钸�_��
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
//  @brief  �`��
//  @param	[in]	ibo		IndexBufferObject
//  @param	[in]	start	�J�n���_
//  @param	[in]	count	�`�悷�钸�_��
//  @param	[in]	size	�`�悷�鐔
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

