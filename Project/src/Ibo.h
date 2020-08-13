//=============================================================================
//!
//!	@file	Ibo.h
//!	@brief	Ibo
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//!	Vbo Ibo Vao
namespace BufferObject
{

//=============================================================================
//! @brief  IndexBufferObject
//=============================================================================
class Ibo
{
public:
	friend class Vao;	//	Vao�ň������߂Ƀt�����h�ɂ���
	friend class Vbo;	//	Vbo�ň������߂Ƀt�����h�ɂ���

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
		Desc(u32 stride, u32 count, const void* pData);

		//! @brief  �C���f�b�N�X�ݒ�
		//!	@param	[in]	stride	�f�[�^����̃T�C�Y
		//!	@param	[in]	count	�z��T�C�Y
		//!	@param	[in]	pData	�f�[�^�|�C���^
		void SetIndexData(u32 stride, u32 count, const void* pData);

		u32						_indexStride;	//!< �f�[�^��̃T�C�Y
		u32						_indexCount;	//!< �f�[�^�T�C�Y
		const void*				_pIndexData;	//!< �f�[�^�̃|�C���^
	};

public:
	//! @brief  �R���X�g���N�^
	Ibo();

	//! @brief  �f�X�g���N�^
	virtual ~Ibo();

	//! @brief  �f�[�^�쐬
	//!	@param	[in]	desc			�o�b�t�@�f�[�^
	//!	@param	[in]	indexType		�C���f�b�N�X�̌^
	bool Create(const Desc& desc, INDEX_TYPE indexType);

	//! @brief  Ibo��ID�擾
	//!	@return	IBO��ID
	u32 GetId(){ return _iboId; }

	//! @brief  �o�b�t�@�L����
	void Bind()const;

	//! @brief  �o�b�t�@������
	void UnBind()const;
private:
	u32		_iboId;			//!< IndexBufferObject ID
	u32		_indexType;		//!< �C���f�b�N�X�^�C�v
	u32		_indexCount;	//!< �C���f�b�N�X�T�C�Y
};

};	//	namespace BufferObject
