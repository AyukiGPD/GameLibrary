//=============================================================================
//!
//!	@file	Texture.cpp
//!	@brief	画像描画クラス
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
//!	コンストラクタ
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
//!	デストラクタ
//-----------------------------------------------------------------------------
FrameBuffer::~FrameBuffer()
{
	Clear();
}

//-----------------------------------------------------------------------------
//!	生成
//!	@param	[in]	width	幅
//!	@param	[in]	height	高さ
//!	@param	[in]	params	テクスチャパラメータリスト
//!	@return	new生成したFrameBuffer	失敗時はnullptr
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
//!	生成	テクスチャ一つのみ
//!	@param	[in]	width	幅
//!	@param	[in]	height	高さ
//!	@param	[in]	param	テクスチャパラメータ
//!	@return	new生成したFrameBuffer	失敗時はnullptr
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
//!	初期化
//!	@param	[in]	width	幅
//!	@param	[in]	height	高さ
//!	@param	[in]	params	テクスチャパラメータリスト
//!	@retval	true	成功
//!	@retval	false	失敗
//-----------------------------------------------------------------------------
bool FrameBuffer::Init(u32 width, u32 height, const std::vector<Desc>& descs)
{
	if (_fboId != 0){ return false; }	//	既に作成されている

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
//!	初期化	テクスチャ一つのみ
//!	@param	[in]	width	幅
//!	@param	[in]	height	高さ
//!	@param	[in]	param	テクスチャパラメータ
//!	@retval	true	成功
//!	@retval	false	失敗
//-----------------------------------------------------------------------------
bool FrameBuffer::Init(u32 width, u32 height, const Desc& desc)
{
	if (_fboId != 0){ return false; }	//	既に作成されている

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
//!	解放
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
//!	開始
//!	@param	[in]	isClear	バッファクリア	default:true
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
//!	終了
//-----------------------------------------------------------------------------
void FrameBuffer::End()
{
	UnBind();
	
	//	ミップマップ更新
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
//!	有効化
//-----------------------------------------------------------------------------
void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
}

//-----------------------------------------------------------------------------
//!	無効化
//-----------------------------------------------------------------------------
void FrameBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//-----------------------------------------------------------------------------
//!	レンダーターゲット指定
//-----------------------------------------------------------------------------
void FrameBuffer::SetTargetBuffer()
{
	if (_run)
	{
		u32 size = _pRenderTargets.size();
		if (size <= 0){ return; }
		// レンダーターゲットを指定する
		glDrawBuffers(size, &gAttachBuffers[0]);
	}
}

void FrameBuffer::BufferClear()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



