//=============================================================================
//!
//!	@file	Texture.cpp
//!	@brief	�摜�`��N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

static const u32 gAttachBuffers[16]
{
	GL_COLOR_ATTACHMENT0,
	GL_COLOR_ATTACHMENT1,
	GL_COLOR_ATTACHMENT2,
	GL_COLOR_ATTACHMENT3,
	GL_COLOR_ATTACHMENT4,
	GL_COLOR_ATTACHMENT5,
	GL_COLOR_ATTACHMENT6,
	GL_COLOR_ATTACHMENT7,
	GL_COLOR_ATTACHMENT8,
	GL_COLOR_ATTACHMENT9,
	GL_COLOR_ATTACHMENT10,
	GL_COLOR_ATTACHMENT11,
	GL_COLOR_ATTACHMENT12,
	GL_COLOR_ATTACHMENT13,
	GL_COLOR_ATTACHMENT14,
	GL_COLOR_ATTACHMENT15,
};
//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
FrameBuffer::FrameBuffer()
: _fboId(0)
, _width(0)
, _height(0)
, _run(false)
, _pDepthTarget(nullptr)
, _pStencilTarget(nullptr)
{
}
//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
FrameBuffer::~FrameBuffer()
{
	Clear();
}

//-----------------------------------------------------------------------------
//!	����
//!	@param	[in]	width	��
//!	@param	[in]	height	����
//!	@param	[in]	params	�e�N�X�`���p�����[�^���X�g
//!	@return	new��������FrameBuffer	���s����nullptr
//-----------------------------------------------------------------------------
FrameBuffer* FrameBuffer::Create(u32 width, u32 height, const std::vector<Desc>& descs)
{
	if(width <= 0 || height <= 0 || descs.empty())
	{
		return nullptr;
	}
	FrameBuffer* pFrameBuffer = nullptr;
	pFrameBuffer= new FrameBuffer();
	if(!pFrameBuffer->Init(width, height, descs))
	{
		SafeDelete(pFrameBuffer);
	}

	return pFrameBuffer;
}

//-----------------------------------------------------------------------------
//!	����	�e�N�X�`����̂�
//!	@param	[in]	width	��
//!	@param	[in]	height	����
//!	@param	[in]	param	�e�N�X�`���p�����[�^
//!	@return	new��������FrameBuffer	���s����nullptr
//-----------------------------------------------------------------------------
FrameBuffer* FrameBuffer::Create(u32 width, u32 height, const Desc& desc)
{
	if (width <= 0 || height <= 0)
	{
		return nullptr;
	}
	FrameBuffer* pFrameBuffer = nullptr;
	pFrameBuffer = new FrameBuffer();
	if(!pFrameBuffer->Init(width, height, desc))
	{
		SafeDelete(pFrameBuffer);
	}

	return pFrameBuffer;
}

//-----------------------------------------------------------------------------
//!	������
//!	@param	[in]	width	��
//!	@param	[in]	height	����
//!	@param	[in]	params	�e�N�X�`���p�����[�^���X�g
//!	@retval	true	����
//!	@retval	false	���s
//-----------------------------------------------------------------------------
bool FrameBuffer::Init(u32 width, u32 height, const std::vector<Desc>& descs)
{
	if (_fboId != 0){ return false; }	//	���ɍ쐬����Ă���

	glGenFramebuffers(1, &_fboId);
	u32 size = descs.size();
	for (u32 i = 0; i < size; ++i)
	{
		RenderTarget::Desc texDesc(width, height, descs[i]._param);
		RenderTarget* pRenderTarget = RenderTarget::Create(texDesc);
		if (pRenderTarget == nullptr)
		{
			Clear();
			return false;
		}
		switch(descs[i]._target)
		{
		case TARGET::COLOR:
			_pRenderTargets.push_back(pRenderTarget);
			break;
		case TARGET::DEPTH:
			SafeDelete(_pDepthTarget);
			_pDepthTarget = pRenderTarget;
			break;
		case TARGET::STENCIL:
			SafeDelete(_pStencilTarget);
			_pStencilTarget = pRenderTarget;
			break;
		default:
			break;
		}
	}
	_width = width;
	_height = height;


	Begin();
	u32 texId = 0;
	for (u32 i = 0; i < _pRenderTargets.size(); ++i)
	{
		texId = _pRenderTargets[i]->GetTextureID();
		glFramebufferTexture2D(GL_FRAMEBUFFER, gAttachBuffers[i], GL_TEXTURE_2D, texId, 0);
	}
	if(_pDepthTarget)
	{
		texId = _pDepthTarget->GetTextureID();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texId, 0);
	}
	if(_pStencilTarget)
	{
		texId = _pDepthTarget->GetTextureID();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texId, 0);
	}
	End();


	return true;
}

//-----------------------------------------------------------------------------
//!	������	�e�N�X�`����̂�
//!	@param	[in]	width	��
//!	@param	[in]	height	����
//!	@param	[in]	param	�e�N�X�`���p�����[�^
//!	@retval	true	����
//!	@retval	false	���s
//-----------------------------------------------------------------------------
bool FrameBuffer::Init(u32 width, u32 height, const Desc& desc)
{
	if (_fboId != 0){ return false; }	//	���ɍ쐬����Ă���

	glGenFramebuffers(1, &_fboId);
	RenderTarget::Desc texDesc(width, height, desc._param);
	RenderTarget* pRenderTarget = RenderTarget::Create(texDesc);
	if (pRenderTarget == nullptr)
	{
		Clear();
		return false;
	}
	switch(desc._target)
	{
	case TARGET::COLOR:
		_pRenderTargets.push_back(pRenderTarget);
		break;
	case TARGET::DEPTH:
		_pDepthTarget = pRenderTarget;
		break;
	case TARGET::STENCIL:
		_pStencilTarget = pRenderTarget;
		break;
	default:
		break;
	}
	_width = width;
	_height = height;


	Begin();
	u32 texId = 0;
	for (u32 i = 0; i < _pRenderTargets.size(); ++i)
	{
		texId = _pRenderTargets[i]->GetTextureID();
		glFramebufferTexture2D(GL_FRAMEBUFFER, gAttachBuffers[i], GL_TEXTURE_2D, texId, 0);
	}
	if(_pDepthTarget)
	{
		texId = _pDepthTarget->GetTextureID();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texId, 0);
	}
	if(_pStencilTarget)
	{
		texId = _pDepthTarget->GetTextureID();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texId, 0);
	}
	End();


	return true;
}



//-----------------------------------------------------------------------------
//!	���
//-----------------------------------------------------------------------------
void FrameBuffer::Clear()
{
	glDeleteFramebuffers(1, &_fboId);
	SafeDeleteArray(_pRenderTargets);
	SafeDelete(_pDepthTarget);
	SafeDelete(_pStencilTarget);
	_width  = 0;
	_height = 0;
	_run = false;
}

//-----------------------------------------------------------------------------
//!	�J�n
//!	@param	[in]	isClear	�o�b�t�@�N���A	default:true
//-----------------------------------------------------------------------------
void FrameBuffer::Begin(bool isClear)
{
	if (_fboId != 0)
	{
		Manager::Renderer()->SetViewport(0, 0, _width, _height);
		Bind();
		_run = true;
		SetTargetBuffer();
		if(isClear)
		{
			BufferClear();
		}
	}
}

//-----------------------------------------------------------------------------
//!	�I��
//-----------------------------------------------------------------------------
void FrameBuffer::End()
{
	UnBind();
	
	//	�~�b�v�}�b�v�X�V
	for(u32 i = 0; i < _pRenderTargets.size(); ++i)
	{
		RenderTarget*& pTarget = _pRenderTargets[i];
		if(pTarget->GetMipLevel() > 0)
		{
			pTarget->BindTexture();
			glGenerateMipmap(GL_TEXTURE_2D);
			pTarget->UnBindTexture();

		}

	}
	_run = false;
}

//-----------------------------------------------------------------------------
//!	�L����
//-----------------------------------------------------------------------------
void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
}

//-----------------------------------------------------------------------------
//!	������
//-----------------------------------------------------------------------------
void FrameBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//-----------------------------------------------------------------------------
//!	�����_�[�^�[�Q�b�g�w��
//-----------------------------------------------------------------------------
void FrameBuffer::SetTargetBuffer()
{
	if (_run)
	{
		u32 size = _pRenderTargets.size();
		if (size <= 0){ return; }
		// �����_�[�^�[�Q�b�g���w�肷��
		glDrawBuffers(size, &gAttachBuffers[0]);
	}
}

void FrameBuffer::BufferClear()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



