//=============================================================================
//!
//!	@file	VaoObject.cpp
//!	@brief	Vao�I�u�W�F�N�g
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

using namespace BufferObject;

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//-----------------------------------------------------------------------------
VaoObject::VaoObject()
:_pVao(nullptr)
{
	AddTag("VaoObject");
}

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//  @param	[in]	desc			�o�b�t�@�f�[�^
//  @param	[in]	primitiveType	�`����@
//  @param	[in]	indexType		�C���f�b�N�X�̌^
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
//  @brief  �f�X�g���N�^
//-----------------------------------------------------------------------------
VaoObject::~VaoObject()
{
	Clear();
}

//-----------------------------------------------------------------------------
//  @brief  ���
//-----------------------------------------------------------------------------
void VaoObject::Clear()
{
	SafeRelease(_pVao);
}

//-----------------------------------------------------------------------------
//  @brief  �f�[�^�쐬
//  @param	[in]	desc			�o�b�t�@�f�[�^
//  @param	[in]	primitiveType	�`����@
//  @param	[in]	indexType		�C���f�b�N�X�̌^
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
//  @brief  �f�[�^�쐬
//  @param	[in]	desc			�o�b�t�@�f�[�^
//  @param	[in]	primitiveType	�`����@
//  @param	[in]	indexType		�C���f�b�N�X�̌^
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
//  @brief  �`��
//-----------------------------------------------------------------------------
void VaoObject::Render()
{
}

//-----------------------------------------------------------------------------
//  @brief  �f�[�^�L��
//-----------------------------------------------------------------------------
bool VaoObject::IsEmpty()
{
	return _pVao == nullptr;
}

//-----------------------------------------------------------------------------
//  @brief  Vao�擾
//  @return	Vao�̃|�C���^
//-----------------------------------------------------------------------------
Vao* VaoObject::GetVao()
{
	return _pVao;
}


