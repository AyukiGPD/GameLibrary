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
//  @brief  �R���X�g���N�^
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
//  @brief  ���_�ݒ�
//  @param	[in]	stride	�f�[�^����̃T�C�Y
//  @param	[in]	count	�z��T�C�Y
//  @param	[in]	pData	�f�[�^�|�C���^
//-----------------------------------------------------------------------------
void Vao::Desc::SetVertexData(u32 stride, u32 count, const void* pData)
{
	_vertexStride = stride;
	_vertexCount  = count;
	_pVertexData  = pData;
}

//-----------------------------------------------------------------------------
//  @brief  �C���f�b�N�X�ݒ�
//  @param	[in]	stride	�f�[�^����̃T�C�Y
//  @param	[in]	count	�z��T�C�Y
//  @param	[in]	pData	�f�[�^�|�C���^
//-----------------------------------------------------------------------------
void Vao::Desc::SetIndexData(u32 stride, u32 count, const void* pData)
{
	_indexStride = stride;
	_indexCount  = count;
	_pIndexData  = pData;
}

//-----------------------------------------------------------------------------
//  @brief  �o�b�t�@���ǉ�
//  @param	[in]	pList			���X�g�f�[�^
//  @param	[in]	elementCount	�z��T�C�Y
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
//  @brief  �o�b�t�@���ǉ�
//  @param	[in]	list	���X�g�f�[�^
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
//  @brief  �R���X�g���N�^
//-----------------------------------------------------------------------------
Vao::Vao()
: _vao(0)
, _pVbo(nullptr)
, _pIbo(nullptr)
{
	AddTag("Vao");
}

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//  @param	[in]	desc			�o�b�t�@�f�[�^
//  @param	[in]	primitiveType	�`����@
//  @param	[in]	indexType		�C���f�b�N�X�̌^
//-----------------------------------------------------------------------------
Vao::Vao(const Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType)
{
	Init(desc, primitiveType, indexType);
}
//-----------------------------------------------------------------------------
//  @brief  �f�X�g���N�^
//-----------------------------------------------------------------------------
Vao::~Vao()
{
	SafeDelete(_pVbo);
	SafeDelete(_pIbo);
	glDeleteVertexArrays(1,&_vao);
}


//-----------------------------------------------------------------------------
//  @brief  �f�[�^�쐬
//  @param	[in]	desc			�o�b�t�@�f�[�^
//  @param	[in]	primitiveType	�`����@
//  @param	[in]	indexType		�C���f�b�N�X�̌^
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
	// Vao�̍쐬
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
//  @brief  �f�[�^�쐬
//  @param	[in]	desc			�o�b�t�@�f�[�^
//  @param	[in]	primitiveType	�`����@
//  @param	[in]	indexType		�C���f�b�N�X�̌^
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
//  @brief  �`��
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


	// �`��
	glBindVertexArray(_vao);
	glDrawElements(primitiveType, indexCount, indexType, nullptr);
	isErrorGL();

	glBindVertexArray(0);

//	_pVbo->Render(*_pIbo);
}

//-----------------------------------------------------------------------------
//! @brief  Vbo�o�b�t�@�f�[�^
//-----------------------------------------------------------------------------
Vao::Desc::operator Vbo::Desc() const
{
	Vbo::Desc desc(_vertexStride, _vertexCount, _pVertexData, _elementList);
	return desc;
}

//-----------------------------------------------------------------------------
//! @brief  Ibo�o�b�t�@�f�[�^
//-----------------------------------------------------------------------------
Vao::Desc::operator Ibo::Desc() const
{
	Ibo::Desc desc(_indexStride, _indexCount, _pIndexData);
	return desc;
}
