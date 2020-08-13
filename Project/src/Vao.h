//=============================================================================
//!
//!	@file	Vao.h
//!	@brief	Vao
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//!	Vbo Ibo Vao
namespace BufferObject
{

//=============================================================================
//! @brief  VertexArrayObject
//=============================================================================
class Vao : public MeshBase
{
	friend class VaoObject;
public:
	//------------------------------------------------------------------
	//! @brief  �o�b�t�@�f�[�^
	//------------------------------------------------------------------
	struct Desc
	{
		//! @brief  �R���X�g���N�^
		Desc();

		//! @brief  ���_�ݒ�
		//!	@param	[in]	stride	�f�[�^����̃T�C�Y
		//!	@param	[in]	count	�z��T�C�Y
		//!	@param	[in]	pData	�f�[�^�|�C���^
		void SetVertexData(u32 stride, u32 count, const void* pData);

		//! @brief  �C���f�b�N�X�ݒ�
		//!	@param	[in]	stride	�f�[�^����̃T�C�Y
		//!	@param	[in]	count	�z��T�C�Y
		//!	@param	[in]	pData	�f�[�^�|�C���^
		void SetIndexData(u32 stride, u32 count, const void* pData);

		//! @brief  �o�b�t�@���ǉ�
		//!	@param	[in]	pList			���X�g�f�[�^
		//!	@param	[in]	elementCount	�z��T�C�Y
		void SetElementList(const Element* pList, u32 elementCount);

		//! @brief  �o�b�t�@���ǉ�
		//!	@param	[in]	list	�o�b�t�@��񃊃X�g
		void SetElementList(const std::vector<Element>& list);

		//! @brief  Vbo�o�b�t�@�f�[�^
		operator Vbo::Desc()const;

		//! @brief  Ibo�o�b�t�@�f�[�^
		operator Ibo::Desc()const;

		u32						_vertexStride;	//!< �f�[�^��̃T�C�Y
		u32						_vertexCount;	//!< �f�[�^�T�C�Y
		const void*				_pVertexData;	//!< �f�[�^�̃|�C���^

		u32						_indexStride;	//!< �f�[�^��̃T�C�Y
		u32						_indexCount;	//!< �f�[�^�T�C�Y
		const void*				_pIndexData;	//!< �f�[�^�̃|�C���^

		std::vector<Element>	_elementList;	//!< �o�b�t�@��񃊃X�g
	};

public:
	//! @brief  �R���X�g���N�^
	Vao();

	//! @brief  �R���X�g���N�^
	//!	@param	[in]	desc			�o�b�t�@�f�[�^
	//!	@param	[in]	primitiveType	�`����@
	//!	@param	[in]	indexType		�C���f�b�N�X�̌^
	Vao(const Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType);

	//! @brief  �f�X�g���N�^
	virtual ~Vao();

	//! @brief  �f�[�^�쐬
	//!	@param	[in]	desc			�o�b�t�@�f�[�^
	//!	@param	[in]	primitiveType	�`����@
	//!	@param	[in]	indexType		�C���f�b�N�X�̌^
	bool Init(const Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType);

	//! @brief  �f�[�^�쐬
	//!	@param	[in]	desc			�o�b�t�@�f�[�^
	//!	@param	[in]	primitiveType	�`����@
	//!	@param	[in]	indexType		�C���f�b�N�X�̌^
	static Vao* Create(const Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType);

	// @brief  �`��
	void Render();

private:
	Vbo*	_pVbo;			//!< Vertex Buffer Object
	Ibo*	_pIbo;			//!< Index  Buffer Object
	u32		_vao;			//!< Vertex Array  Object ID
};

};	//	namespace BufferObject
