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
//  @brief  �R���X�g���N�^
//-----------------------------------------------------------------------------
Ibo::Desc::Desc()
: _indexStride	(0)
, _indexCount	(0)
, _pIndexData	(nullptr)
{
}

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//  @param	[in]	stride	�f�[�^����̃T�C�Y
//  @param	[in]	count	�z��T�C�Y
//  @param	[in]	pData	�f�[�^�|�C���^
//-----------------------------------------------------------------------------
Ibo::Desc::Desc(u32 stride, u32 count, const void* pData)
: _indexStride	(stride)
, _indexCount	(count)
, _pIndexData	(pData)
{
}

//-----------------------------------------------------------------------------
//  @brief  �C���f�b�N�X�ݒ�
//  @param	[in]	stride	�f�[�^����̃T�C�Y
//  @param	[in]	count	�z��T�C�Y
//  @param	[in]	pData	�f�[�^�|�C���^
//-----------------------------------------------------------------------------
void Ibo::Desc::SetIndexData(u32 stride, u32 count, const void* pData)
{
	_indexStride = stride;
	_indexCount = count;
	_pIndexData = pData;
}

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//-----------------------------------------------------------------------------
Ibo::Ibo()
: _iboId(0)
, _indexType(0)
, _indexCount(0)
{
}

//-----------------------------------------------------------------------------
//  @brief  �f�X�g���N�^
//-----------------------------------------------------------------------------
Ibo::~Ibo()
{
	glDeleteBuffers(1, &_iboId);
}

//-----------------------------------------------------------------------------
//  @brief  �f�[�^�쐬
//  @param	[in]	desc			�o�b�t�@�f�[�^
//  @param	[in]	indexType		�C���f�b�N�X�̌^
//-----------------------------------------------------------------------------
bool Ibo::Create(const Desc& desc, INDEX_TYPE indexType)
{
	if (!desc._pIndexData)
	{
		return false;
	}

	// Ibo�̍쐬
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
		/* ���w�� */
		_indexType = 0;
		return false;
	}

	//	�C���f�b�N�X���擾
	_indexCount = desc._indexCount;
	return true;
}


//-----------------------------------------------------------------------------
//  @brief  �o�b�t�@�L����
//-----------------------------------------------------------------------------
void Ibo::Bind()const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboId);
}

//-----------------------------------------------------------------------------
//  @brief  �o�b�t�@������
//-----------------------------------------------------------------------------
void Ibo::UnBind()const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


