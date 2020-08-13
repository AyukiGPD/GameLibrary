//=============================================================================
//!
//!	@file	BoxMesh.h
//!	@brief	�{�b�N�X
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//=============================================================================
//! @brief  �{�b�N�X
//=============================================================================
class BoxMesh : public MeshBase
{
	//!	�R���X�g���N�^
	BoxMesh();
	//!	�R���X�g���N�^
	//!	@param	[in]	pParent	�e�I�u�W�F�N�g
	BoxMesh(const MaterialData& material);
	//!	�f�X�g���N�^
	virtual ~BoxMesh();

	//!	�`��
	void Render();
private:
	static RefPointer<BufferObject::Vao>	_pStaticBoxMesh;
};


