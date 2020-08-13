//=============================================================================
//!
//!	@file	VaoObject.cpp
//!	@brief	Vao�I�u�W�F�N�g
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

namespace BufferObject
{

//-----------------------------------------------------------------------------
//! @brief  Vao�I�u�W�F�N�g
//-----------------------------------------------------------------------------
class VaoObject : public GameObjectBase
{
public:
	friend Vao;
	//! @brief  �R���X�g���N�^
	VaoObject();

	//! @brief  �R���X�g���N�^
	//!	@param	[in]	desc			�o�b�t�@�f�[�^
	//!	@param	[in]	primitiveType	�`����@
	//!	@param	[in]	indexType		�C���f�b�N�X�̌^
	VaoObject(const Vao::Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType, const MaterialData& material);

	//! @brief  �f�X�g���N�^
	virtual ~VaoObject();

	//! @brief  ���
	void Clear();

	//! @brief  �f�[�^�쐬
	//!	@param	[in]	desc			�o�b�t�@�f�[�^
	//!	@param	[in]	primitiveType	�`����@
	//!	@param	[in]	indexType		�C���f�b�N�X�̌^
	bool Init(const Vao::Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType, const MaterialData& materiial);

	//! @brief  �f�[�^�쐬
	//!	@param	[in]	desc			�o�b�t�@�f�[�^
	//!	@param	[in]	primitiveType	�`����@
	//!	@param	[in]	indexType		�C���f�b�N�X�̌^
	static VaoObject* Create(const Vao::Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType, const MaterialData& materiial);

	//! @brief  �`��
	void Render();

	//! @brief  �f�[�^�L��
	bool IsEmpty();

	//! @brief  Vao�擾
	//!	@return	Vao�̃|�C���^
	Vao* GetVao();

private:
	Vao*	_pVao;
};

};	//	namespace BufferObject

