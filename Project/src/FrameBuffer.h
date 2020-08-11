//=============================================================================
//!
//!	@file	Texture.h
//!	@brief	�摜�`��N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//=============================================================================
//!	FrameBufferObject
//=============================================================================
class FrameBuffer
{
public:
	enum class TARGET
	{
		COLOR,
		DEPTH,
		STENCIL
	};

	struct Desc
	{
		Desc(Texture::Param param, TARGET target = TARGET::COLOR)
		: _param(param)
		, _target(target)
		{
		}
		Texture::Param _param;
		TARGET _target;
	};
	//!	�R���X�g���N�^
	FrameBuffer();
	//!	�f�X�g���N�^
	virtual ~FrameBuffer();

	//!	����
	//!	@param	[in]	width	��
	//!	@param	[in]	height	����
	//!	@param	[in]	params	�e�N�X�`���p�����[�^���X�g
	//!	@return	new��������FrameBuffer	���s����nullptr
	static FrameBuffer* Create(u32 width, u32 height, const std::vector<Desc>& descs);

	//!	����	�e�N�X�`����̂�
	//!	@param	[in]	width	��
	//!	@param	[in]	height	����
	//!	@param	[in]	param	�e�N�X�`���p�����[�^
	//!	@return	new��������FrameBuffer	���s����nullptr
	static FrameBuffer* Create(u32 width, u32 height, const Desc& desc);

	//!	������
	//!	@param	[in]	width	��
	//!	@param	[in]	height	����
	//!	@param	[in]	params	�e�N�X�`���p�����[�^���X�g
	//!	@retval	true	����
	//!	@retval	false	���s
	bool Init(u32 width, u32 height, const std::vector<Desc>& descs);

	//!	������	�e�N�X�`����̂�
	//!	@param	[in]	width	��
	//!	@param	[in]	height	����
	//!	@param	[in]	param	�e�N�X�`���p�����[�^
	//!	@retval	true	����
	//!	@retval	false	���s
	bool Init(u32 width, u32 height, const Desc& desc);
	//!	���
	void Clear();

	//!	�J�n
	//!	@param	[in]	isClear	�o�b�t�@�N���A	default:true
	void Begin(bool isClear = true);
	//!	�I��
	void End();

	//!	�L����
	void Bind();
	//!	������
	static void UnBind();

	//!	�����_�[�^�[�Q�b�g�w��
	void SetTargetBuffer();

	void BufferClear();

	//	�擾�֐�

	//!	FBO ID
	u32 GetId()const				{ return _fboId; }
	//!	��
	u32 GetWidth()const				{ return _width; }
	//!	����
	u32 GetHeight()const			{ return _height; }
	//!	�o�b�t�@��
	size_t GetTextureSize()const		{ return _pRenderTargets.size(); }
	//!	�����Ă�
	bool IsRun()const				{ return _run; }

	//!	�e�N�X�`���擾
	//!	@return	�e�N�X�`���|�C���^
	RenderTarget* GetTexture(u32 index)
	{
		if (index >= _pRenderTargets.size()){ return nullptr; }
		return _pRenderTargets[index];
	}

private:
	u32 _fboId;				//!< ID
	u32 _width;				//!< ��
	u32 _height;			//!< ����
	bool _run;
	std::vector<RenderTarget*> _pRenderTargets;
	RenderTarget* _pDepthTarget;
	RenderTarget* _pStencilTarget;
};
