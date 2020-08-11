//=============================================================================
//!
//!	@file	Filter.cpp
//!	@brief	フィルター
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"


//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
BlurFilter::BlurFilter()
: _pBlur(nullptr)
{
}
//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
BlurFilter::~BlurFilter()
{
	Clear();
}

//-----------------------------------------------------------------------------
//!	解放
//-----------------------------------------------------------------------------
void BlurFilter::Clear()
{
	SafeDelete(_pBlur);
}


//-----------------------------------------------------------------------------
//!	初期化
//!	@param	[in]	width	幅
//!	@param	[in]	height	高さ
//!	@param	[in]	param	テクスチャパラメータ
//-----------------------------------------------------------------------------
bool BlurFilter::Init(u32 width, u32 height, const Texture::Param& param)
{
	if (!ShaderLoad()){ return false; }

	if(!_pBlur)
	{
		FrameBuffer* pFrameBuffer = FrameBuffer::Create(width, height, param);
		if (pFrameBuffer == nullptr)
		{
			return false;
		}
		_pBlur = pFrameBuffer;
	}
	return true;
}

//-----------------------------------------------------------------------------
//!	テクスチャ描画
//!	@param	[in]	texture	テクスチャ
//-----------------------------------------------------------------------------
void BlurFilter::Render(const Texture* pTexture)
{
	if (!pTexture){ return; }
	ShaderManager* pShader = Manager::Shader();

	_pBlur->Begin();
	pShader->Begin(_shader);
	Texture::BindTexture(pTexture, 0);
	pShader->SetUniform(10, 0);
	pShader->SetUniform(11, 1.0f / pTexture->GetWidth());
	pShader->SetUniform(12, 1.0f / pTexture->GetHeight());
	Render2D::RenderBuffer(nullptr);
	_pBlur->End();
}

//-----------------------------------------------------------------------------
//!	シェーダー読み込み内部関数
//-----------------------------------------------------------------------------
bool BlurFilter::ShaderLoad()
{
	ShaderManager* pShader = Manager::Shader();
	_shader = pShader->Load("Gaussian", PROGRAM_TYPE::VS_FS);
	if (_shader.IsEmpty())
	{
		return false;
	}

	return true;
}



//!	コンストラクタ
EmissiveColorFilter::EmissiveColorFilter()
: _pEmissive(nullptr)
{
}
//!	デストラクタ
EmissiveColorFilter::~EmissiveColorFilter()
{
	SafeDelete(_pEmissive);
}

//!	初期化
//!	@param	[in]	width	幅
//!	@param	[in]	height	高さ
//!	@param	[in]	param	テクスチャパラメータ
//!	@retval	true	成功
//!	@retval	false	失敗
bool EmissiveColorFilter::Init(u32 width, u32 height, const Texture::Param& param)
{
	if (!ShaderLoad()){ return false; }

	if(!_pEmissive)
	{
		FrameBuffer* pFrameBuffer = FrameBuffer::Create(width, height, param);
		if (pFrameBuffer == nullptr)
		{
			return false;
		}
		_pEmissive = pFrameBuffer;
	}
	return true;
}

//!	テクスチャ描画
//!	@param	[in]	pTexEmissive	Emissiveテクスチャ
//!	@param	[in]	pTexDiffuse		Diffuseテクスチャ
void EmissiveColorFilter::Render(const Texture* pTexEmissive, const Texture* pTexDiffuse)
{
	if (!pTexEmissive || !pTexDiffuse){ return; }
	ShaderManager* pShader = Manager::Shader();

	_pEmissive->Begin();
	pShader->Begin(_shader);

	Texture::BindTexture(pTexEmissive, 0);
	Texture::BindTexture(pTexDiffuse, 1);
	pShader->SetUniform(10, 0);
	pShader->SetUniform(11, 1);


	Render2D::RenderBuffer(nullptr);

	Texture::UnBindTexture(1);
	Texture::UnBindTexture(0);

	_pEmissive->End();


}

//!	シェーダー読み込み内部関数
//!	@retval	true	成功
//!	@retval	false	失敗
bool EmissiveColorFilter::ShaderLoad()
{
	ShaderManager* pShader = Manager::Shader();
	_shader = pShader->Load("Emissive", PROGRAM_TYPE::VS_FS);
	if (_shader.IsEmpty())
	{
		return false;
	}

	return true;
}



//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
ToneMappingFilter::ToneMappingFilter()
: _pTonemapping(nullptr)
, _shaderLuminance()
, _shaderToneMapping()
, _exposureBias(3.f)
, _exposureOffset(1.f)
, _minBufferCount(0)
, _exposureBiasMax(200.0f)
, _exposureBiasMin(1.0f)
{
}
//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
ToneMappingFilter::~ToneMappingFilter()
{
	Clear();
}
//-----------------------------------------------------------------------------
//!	解放
//-----------------------------------------------------------------------------
void ToneMappingFilter::Clear()
{
	SafeDelete(_pTonemapping);
	SafeDeleteArray(_pMinBuffer);
}
//-----------------------------------------------------------------------------
//!	初期化
//!	@param	[in]	width	幅
//!	@param	[in]	height	高さ
//!	@param	[in]	param	テクスチャパラメータ
//!	@retval	true	成功
//!	@retval	false	失敗
//-----------------------------------------------------------------------------
bool ToneMappingFilter::Init(u32 width, u32 height, const Texture::Param& param)
{
	if (!ShaderLoad()){ return false; }
	u32 a = 2;
	u32 minWidth  = width  / a;
	u32 minHeight = height / a;
	while (minWidth > a || minHeight > a)
	{
		Texture::Param minParam(Texture::FORMAT::RED16F);
		FrameBuffer* pBuffer = FrameBuffer::Create(minWidth, minHeight, minParam);
		if (pBuffer)
		{
			_pMinBuffer.push_back(pBuffer);
		}
		else
		{
			return false;
		}
		minWidth	= minWidth  / a;
		minHeight	= minHeight / a;
	}
	{
		Texture::Param minParam(Texture::FORMAT::RED16F);
		FrameBuffer* pBuffer = FrameBuffer::Create(1, 1, minParam);
		if (pBuffer)
		{
			_pMinBuffer.push_back(pBuffer);
		}
		else
		{
			return false;
		}
	}


	FrameBuffer* pBuffer = FrameBuffer::Create(width, height, param);
	if (pBuffer)
	{
		_pTonemapping = pBuffer;
	}
	else
	{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//!	テクスチャ描画
//!	@param	[in]	texture	テクスチャ
//-----------------------------------------------------------------------------
void ToneMappingFilter::Render(const Texture* pTexture)
{
	ShaderManager* pShader = Manager::Shader();

	pShader->Begin(_shaderLuminance);

	auto size = _pMinBuffer.size();
	{
		u32 i = _minBufferCount;
		_pMinBuffer[i]->Begin();
		if (i == 0)
		{
			Texture::BindTexture(pTexture, 0);
		}
		else
		{
			Texture::BindTexture(_pMinBuffer[i - 1]->GetTexture(0), 0);
		}
		Render2D::RenderBuffer(nullptr);
		_pMinBuffer[i]->End();
		//if (_minBufferCount == size - 1)
		//{
		//	f32 buf;
		//	_pMinBuffer[size - 1]->Bind();
		//	glReadPixels(0, 0, 1, 1, GL_RED, GL_FLOAT, &buf);
		//	_pMinBuffer[size - 1]->UnBind();
		//	f32 luminance = 1.0f / (buf + FLT_EPSILON);
		//	_exposureOffset = luminance;
		//}

		_minBufferCount = (_minBufferCount + 1) % size;
	}
//	_exposureBias = min(_exposureBias + ((_exposureOffset - _exposureBias + (_exposureBiasMin - 1.f)) * GetDeltaTime()), _exposureBiasMax);
	_exposureBias = 3.f;
	pShader->End();

	_pTonemapping->Begin();
	pShader->Begin(_shaderToneMapping);
	Texture::BindTexture(pTexture, 0);
	pShader->SetUniform(0, _exposureBias);
	pShader->SetUniform(10, 0);
	Render2D::RenderBuffer(nullptr);
	Texture::UnBindTexture(0);
	pShader->End();
	_pTonemapping->End();


}

//-----------------------------------------------------------------------------
//!	シェーダー読み込み内部関数
//!	@retval	true	成功
//!	@retval	false	失敗
//-----------------------------------------------------------------------------
bool ToneMappingFilter::ShaderLoad()
{
	ShaderManager* pShader = Manager::Shader();

	_shaderLuminance = pShader->Load("Luminance", PROGRAM_TYPE::VS_FS);
	if (_shaderLuminance.IsEmpty())
	{
		return false;
	}

	_shaderToneMapping = pShader->Load("ToneMapping", PROGRAM_TYPE::VS_FS);
	if (_shaderToneMapping.IsEmpty())
	{
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
BloomFilter::BloomFilter()
: _pBrightness(nullptr)
, _shader()
{
	for (auto& pBloom : _pBlooms)
	{
		pBloom = nullptr;
	}
	//_pBlooms.assign(nullptr);
}

//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
BloomFilter::~BloomFilter()
{
	Clear();
}

//-----------------------------------------------------------------------------
//!	解放
//-----------------------------------------------------------------------------
void BloomFilter::Clear()
{
	for(auto* pBloom : _pBlooms)
	{
		SafeDelete(pBloom);
	}
	SafeDelete(_pBrightness);

}
//-----------------------------------------------------------------------------
//!	初期化
//!	@param	[in]	width	幅
//!	@param	[in]	height	高さ
//!	@param	[in]	param	テクスチャパラメータ
//!	@retval	true	成功
//!	@retval	false	失敗
//-----------------------------------------------------------------------------
bool BloomFilter::Init(u32 width, u32 height, const Texture::Param& param)
{
	s32 w = width;
	s32 h = height;
	for(u32 i = 0; i < BLOOM_BUFFER_SIZE; ++i)
	{
		w = s32(max(w * 0.5f,1));
		h = s32(max(h * 0.5f,1));

		//	ここにとりあえず記述
		BlurFilter* pBlur = new BlurFilter;
		if(pBlur)
		{
			Texture::Param blurParam(Texture::FORMAT::RGB8, Texture::FILTER::LINEAR_MIPMAP_LINEAR, Texture::FILTER::LINEAR, 1);
			if(pBlur->Init(w, h, blurParam))
			{
				_pBlooms[i] = pBlur;
			}
			else
			{
				Clear();
				SafeDelete(pBlur);
				return false;
			}
		}
		else
		{
			Clear();
			return false;
		}
	}

	FrameBuffer* pBuffer = FrameBuffer::Create(width, height, param);
	if(pBuffer)
	{
		_pBrightness = pBuffer;
	}
	else
	{
		return false;
	}

	_shader = Manager::Shader()->Load("Brightness", PROGRAM_TYPE::VS_FS);
	if(_shader.IsEmpty())
	{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//!	テクスチャ描画
//!	@param	[in]	texture	テクスチャ
//-----------------------------------------------------------------------------
void BloomFilter::Render(const Texture* pTexture)
{
	ShaderManager* pShader = Manager::Shader();

	//	輝度取得
	_pBrightness->Begin();
	pShader->Begin(_shader);
	Texture::BindTexture(pTexture);
	pShader->SetUniform(10, 0);
	Render2D::RenderBuffer(nullptr);
	pShader->End();
	_pBrightness->End();

	_pBlooms[0]->Render(_pBrightness->GetTexture(0));
	for(u32 i = 1; i < BLOOM_BUFFER_SIZE; ++i)
	{
		_pBlooms[i]->Render(_pBlooms[i - 1]->GetTexture());
	}
}





