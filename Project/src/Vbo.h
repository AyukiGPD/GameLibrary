//=============================================================================
//!
//!	@file	Vbo.h
//!	@brief	Vbo
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//!	Vbo Ibo Vao
namespace BufferObject
{

class Ibo;

//=============================================================================
//! @brief  VertexBufferObject
//=============================================================================
class Vbo
{
public:
	friend class Vao;	//	Vao�ň������߂Ƀt�����h�ɂ���

	//------------------------------------------------------------------
	//! @brief  �o�b�t�@�f�[�^
	//------------------------------------------------------------------
	struct Desc
	{
		//! @brief  �R���X�g���N�^
		Desc();

		//! @brief  �R���X�g���N�^
		//!	@param	[in]	stride	�f�[�^����̃T�C�Y
		//!	@param	[in]	count	�z��T�C�Y
		//!	@param	[in]	pData	�f�[�^�|�C���^
		//!	@param	[in]	list	�o�b�t�@��񃊃X�g
		Desc(u32 vertexStride,
			 u32 vertexCount,
			 const void* pVertexData,
			 const std::vector<Element>& list);

		//! @brief  ���_�ݒ�
		//!	@param	[in]	stride	�f�[�^����̃T�C�Y
		//!	@param	[in]	count	�z��T�C�Y
		//!	@param	[in]	pData	�f�[�^�|�C���^
		void SetVertexData(u32 stride, u32 count, const void* pData);

		//! @brief  �o�b�t�@���ǉ�
		//!	@param	[in]	pList			���X�g�f�[�^
		//!	@param	[in]	elementCount	�z��T�C�Y
		void SetElementList(const Element* pList, u32 elementCount);

		//! @brief  �o�b�t�@���ǉ�
		//!	@param	[in]	list	�o�b�t�@��񃊃X�g
		void SetElementList(const std::vector<Element>& list);

		u32						_vertexStride;	//!< �f�[�^��̃T�C�Y
		u32						_vertexCount;	//!< �f�[�^�T�C�Y
		const void*				_pVertexData;	//!< �f�[�^�̃|�C���^

		std::vector<Element>	_elementList;	//!< �o�b�t�@��񃊃X�g
	};

public:
	//! @brief  �R���X�g���N�^
	Vbo();
	//! @brief  �f�X�g���N�^
	virtual ~Vbo();

	//! @brief  �f�[�^�쐬
	//!	@param	[in]	desc			�o�b�t�@�f�[�^
	//!	@param	[in]	primitiveType	�`����@
	//!	@retval	true	����
	//!	@retval	false	���s
	bool Create(const Desc& desc, PRIMITIVE_TYPE primitiveType);

	//! @brief  �`����@�ݒ�
	//!	@param	[in]	type	�^�C�v
	//!	@retval	true	����
	//!	@retval	false	���s _primitiveType��0�ɂȂ�
	bool SetPrimitiveType(PRIMITIVE_TYPE type);

	//! @brief  �o�b�t�@�L����
	void Bind();

	//! @brief  �o�b�t�@������
	void UnBind();

	//! @brief  �����œ��̓��C�A�E�g���Ăяo��
	void CallInputLayout();

	//! @brief  id�擾
	//!	@return Vbo��ID
	u32 GetId(){ return _vboId; }

	//! @brief  �`��
	//!	@param	[in]	ibo	IndexBufferObject
	void Render(const Ibo& ibo);

	//! @brief  �ꕔ�`��
	//!	@param	[in]	ibo		IndexBufferObject
	//!	@param	[in]	start	�J�n���_
	//!	@param	[in]	count	�`�悷�钸�_��
	void Render(const Ibo& ibo, u32 start, u32 count);

	//! @brief  �`��
	//!	@param	[in]	ibo		IndexBufferObject
	//!	@param	[in]	start	�J�n���_
	//!	@param	[in]	count	�`�悷�钸�_��
	//!	@param	[in]	size	�`�悷�鐔
	void InstancedRender(const Ibo& ibo, u32 start, u32 count, u32 size);

private:
	u32						_vboId;			//!< VertexBufferObject ID
	u32						_vertexStride;	//!< �f�[�^��̃T�C�Y
	u32						_primitiveType;	//!< �`��^�C�v
	std::vector<Element>	_elementList;	//!< �o�b�t�@���
};

};	//	namespace BufferObject
