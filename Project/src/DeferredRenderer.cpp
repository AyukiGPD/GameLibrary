//=============================================================================
//!
//!	@file	DeferredRenderer.cpp
//!	@brief	ディファードレンダリング
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//#define LIGHT_COMPUTE_SHADER

//-----------------------------------------------------------------------------
//!	作成したバッファの表示
//-----------------------------------------------------------------------------
void BaseDeferredRenderer::RenderTexture()
{
	if (_pBufferTexture != nullptr)
	{
		Render2D::RenderBuffer(_pBufferTexture);
	}
}


//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
PhysicalBasedRendering::PhysicalBasedRendering()
: _pBloom(nullptr)
, _pEmissive(nullptr)
, _pToneMapping(nullptr)
, _index(0)
, _luminaceCount(0)
, _lightCount(0)
{
}
//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
PhysicalBasedRendering::~PhysicalBasedRendering()
{
	Clear();
}

//-----------------------------------------------------------------------------
//!	初期化
//!	@param	[in]	width	幅
//!	@param	[in]	height	高さ
//!	@retval	true	成功
//!	@retval	false	失敗
//-----------------------------------------------------------------------------
bool PhysicalBasedRendering::Init(u32 width, u32 height)
{
	try
	{
		if (!CreateGBuffer(width, height))
		{
			throw "Error Gbuffer";
		}
		if (!CreateLighting(width, height))
		{
			throw "Error Lighting";
		}
		if (!CreateFinalize(width, height))
		{
			throw "Error Finalize";
		}

		{
			Texture::Param param(Texture::FORMAT::RGB16F, Texture::FILTER::LINEAR_MIPMAP_LINEAR, Texture::FILTER::NEAREST, 1);
			auto pFilter = new BloomFilter;
			if (pFilter)
			{
				if (pFilter->Init(width, height, param))
				{
					_pBloom = pFilter;
				}
				else
				{
					throw "Error Bloom";
				}
			}
			else
			{
				throw "Error Bloom";
			}
		}

		{
			Texture::Param param(Texture::FORMAT::RGB32F);
			EmissiveColorFilter* pFilter = new EmissiveColorFilter;
			if (pFilter)
			{
				if (pFilter->Init(width, height, param))
				{
					_pEmissive = pFilter;
				}
				else
				{
					throw "Error Emissive";
				}
			}
			else
			{
				throw "Error Emissive";
			}
		}

		{
			Texture::Param param(Texture::FORMAT::SRGB8);
			ToneMappingFilter* pFilter = new ToneMappingFilter;
			if (pFilter)
			{
				if (pFilter->Init(width, height, param))
				{
					_pToneMapping = pFilter;
				}
				else
				{
					throw "Error ToneMapping";
				}
			}
			else
			{
				throw "Error ToneMapping";
			}
		}

		_shaderPass.resize(RENDER_PASS::MAX);

		ShaderManager* pShader = Manager::Shader();
		_shaderPass[RENDER_PASS::GBUFFER ] = pShader->Load("GBuffer" , PROGRAM_TYPE::VS_FS);

#ifdef LIGHT_COMPUTE_SHADER
		_shaderPass[RENDER_PASS::LIGHTING] = pShader->Load("Lighting", PROGRAM_TYPE::CS);
#else
		_shaderPass[RENDER_PASS::LIGHTING] = pShader->Load("Lighting", PROGRAM_TYPE::VS_FS);
#endif
		if (_shaderPass[RENDER_PASS::GBUFFER].IsEmpty() || 
			_shaderPass[RENDER_PASS::LIGHTING].IsEmpty() )
		{
			throw "Error Shader";
		}
		_renderMode = RENDER_MODE::TONEMAPPING;
	}
	catch (char* str)
	{
		ERROR_LOG(str);
		Clear();
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
//!	Gバッファ生成
//!	@param	[in]	width	横
//!	@param	[in]	height	縦
//!	@retval	true	成功
//!	@retval	false	失敗
//-----------------------------------------------------------------------------
bool PhysicalBasedRendering::CreateGBuffer(u32 width, u32 height)
{
	std::vector<FrameBuffer::Desc> desc = 
	{
		Texture::Param(Texture::FORMAT::RGBA32F),									// GBUFFER_01
		Texture::Param(Texture::FORMAT::RGBA32F),									// GBUFFER_02
		Texture::Param(Texture::FORMAT::RGBA8),										// GBUFFER_03
		Texture::Param(Texture::FORMAT::RGBA32F),									// GBUFFER_04
		{
			Texture::Param(Texture::FORMAT::DEPTH32, Texture::FILTER::NEAREST, Texture::FILTER::NEAREST),
			FrameBuffer::TARGET::DEPTH
		}	// GBUFFER_05
	};

	//	Gバッファ作成
	_pGbuffer = FrameBuffer::Create(width, height, desc);
	if (!_pGbuffer)
	{
		Clear();
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
//!	ライトバッファ生成
//!	@param	[in]	width	横
//!	@param	[in]	height	縦
//!	@retval	true	成功
//!	@retval	false	失敗
//-----------------------------------------------------------------------------
bool PhysicalBasedRendering::CreateLighting(u32 width, u32 height)
{
	std::vector<FrameBuffer::Desc> desc =
	{
		Texture::Param(Texture::FORMAT::RGBA32F),
		Texture::Param(Texture::FORMAT::RGBA32F),
	};

	//	ライティングバッファ作成
	_pLighting = FrameBuffer::Create(width, height, desc);
	if (!_pLighting)
	{
		Clear();
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
//!	最終合成バッファ生成
//!	@param	[in]	width	横
//!	@param	[in]	height	縦
//!	@retval	true	成功
//!	@retval	false	失敗
//-----------------------------------------------------------------------------
bool PhysicalBasedRendering::CreateFinalize(u32 width, u32 height)
{
	_pFinalize = FrameBuffer::Create(width, height, Texture::Param(Texture::FORMAT::RGB32F));
	if (!_pFinalize)
	{
		Clear();
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//!	解放
//-----------------------------------------------------------------------------
void PhysicalBasedRendering::Clear()
{
	SafeDelete(_pToneMapping);
	SafeDelete(_pEmissive);
	SafeDelete(_pBloom);
}

//-----------------------------------------------------------------------------
//!	デバッグ用更新
//-----------------------------------------------------------------------------
void PhysicalBasedRendering::Update()
{
#ifndef RELEASE_FINAL
	auto* pKeyInput = Manager::KeyInput();
	if (pKeyInput->GetButtons(KEYCODE::PAD_LCONTROL))
	{
		if (pKeyInput->GetTriggers(KEYCODE::PAD_LSHIFT))
		{
			_shaderPass[RENDER_PASS::LIGHTING].ReBuild();
		}
	}

	//	この機能は今後デバッグ用のクラスに移動予定
	if(IsWindowActive())
	{
		s32 luminaceSize = _pToneMapping->GetBufferSize();
		if(pKeyInput->GetTriggers(KEYCODE::PAD_NUMPAD8))
		{
			_index = min(_index + 1, BloomFilter::BLOOM_BUFFER_SIZE - 1);
			_luminaceCount = min(_luminaceCount + 1, luminaceSize - 1);
		}
		else if(pKeyInput->GetTriggers(KEYCODE::PAD_NUMPAD2))
		{
			_index = max(_index - 1, 0);
			_luminaceCount = max(_luminaceCount - 1, 0);
		}

		if(pKeyInput->GetTriggers(KEYCODE::PAD_NUMPAD4))
		{
			_renderMode = (RENDER_MODE)(max((s32)_renderMode - 1, 0));
		}
		else if(pKeyInput->GetTriggers(KEYCODE::PAD_NUMPAD6))
		{
			_renderMode = (RENDER_MODE)(min((s32)_renderMode + 1, (s32)RENDER_MODE::MAX - 1));
		}
	}
#endif
}



//-----------------------------------------------------------------------------
//! Gバッファ描画開始
//-----------------------------------------------------------------------------
void PhysicalBasedRendering::BeginGBuffer()
{
#ifndef RELEASE_FINAL
	_timeGbuffer.Start();
#endif

	_pGbuffer->Begin();
	Manager::Shader()->Begin(_shaderPass[RENDER_PASS::GBUFFER]);
	Manager::Renderer()->SetViewport(0, 0, _pGbuffer->GetWidth(), _pGbuffer->GetHeight());
}

//-----------------------------------------------------------------------------
//! Gバッファ描画終了
//-----------------------------------------------------------------------------
void PhysicalBasedRendering::EndGBuffer()
{
	_pGbuffer->End();
	Manager::Shader()->End();
#ifndef RELEASE_FINAL
	_timeGbuffer.Stop();
#endif

}

struct LightInfo
{
	Vector4			  _position;
	Vector3			  _color;
	f32				  _radius;
};

//-----------------------------------------------------------------------------
//! ライティング開始
//-----------------------------------------------------------------------------
void PhysicalBasedRendering::Lighting(const Matrix& matProj, const Matrix& matView)
{
	LightManager* pLight = Manager::Light();
	std::list<LightObject*> pLightList = pLight->GetLightList();
	u32 lightSize = pLightList.size();
	if (lightSize <= 0)
	{
		return;
	}

#ifndef RELEASE_FINAL
	_timeLightng.Start();
#endif

#ifdef LIGHT_COMPUTE_SHADER

	ShaderManager* pShader = Manager::Shader();
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	size_t size = sizeof(LightInfo);
	GLuint LightID;
	glGenBuffers(1, &LightID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, LightID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, lightSize * size, nullptr, GL_STATIC_DRAW);
	GLuint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

	LightInfo* lightinfoArr =
		(LightInfo*)glMapBufferRange(
									GL_SHADER_STORAGE_BUFFER,
									0,
									lightSize * size,
									bufMask
								);
	isErrorGL();


	_lightCount = 0;

	auto itr = pLightList.begin();
	for (u32 i = 0; itr != pLightList.end(); ++i, ++itr)
	{
		LightInfo& light = lightinfoArr[i];
		LightObject* pLight = (*itr);

		if (pLight->GetLightMode() == LIGHT_MODE::INVALID){ continue; }

		++_lightCount;

		light =
		{
			{ pLight->GetWorldPosition().x, pLight->GetWorldPosition().y, pLight->GetWorldPosition().z ,0.f},
			{ pLight->GetColor().GetColorFRGB().x, pLight->GetColor().GetColorFRGB().y, pLight->GetColor().GetColorFRGB().z },
			pLight->GetPower() * 100
		};
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	pShader->Begin(_shaderPass[RENDER_PASS::LIGHTING]);

	Texture::BindImageTexture(_pLighting->GetTexture(LIGHT_DIFFUSE), 0, Texture::ACCESS_TYPE::WRITE);
	Texture::BindImageTexture(_pLighting->GetTexture(LIGHT_SPECULAR), 1, Texture::ACCESS_TYPE::WRITE);

	Texture::BindImageTexture(_pGbuffer->GetTexture(GBUFFER_01), 2,Texture::ACCESS_TYPE::READ);
	Texture::BindImageTexture(_pGbuffer->GetTexture(GBUFFER_02), 3,Texture::ACCESS_TYPE::READ);
	Texture::BindImageTexture(_pGbuffer->GetTexture(GBUFFER_03), 4,Texture::ACCESS_TYPE::READ);


	Matrix invView = Matrix::Invert(matView);
	Vector3 eyepos = invView.GetVector();
	pShader->SetUniform(7, eyepos);


	pShader->SetUniform(8, matView);

	Matrix invProj = Matrix::Invert(matProj);
	pShader->SetUniform(9, invProj);

	pShader->SetUniform(10, SCRN_WIDTH);
	pShader->SetUniform(11, SCRN_HEIGHT);

//	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 12, LightID);





	glDispatchCompute(SCRN_WIDTH / 16, SCRN_HEIGHT / 16, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	pShader->End();

	glDeleteBuffers(1,&LightID);

#else

	UNUSED(matProj);

	RenderManager* pRenderer = Manager::Renderer();

	pRenderer->SetViewport(0, 0, _pLighting->GetWidth(), _pLighting->GetHeight() );
	ShaderManager* pShader = Manager::Shader();

	pShader->Begin(_shaderPass[RENDER_PASS::LIGHTING]);

	_pLighting->Begin();

	Matrix view = Matrix::Invert(matView);
	Vector3 eyepos = view.GetVector();

	pRenderer->EnableBlending(BLEND_MODE::ONE, BLEND_MODE::ONE);

	// テクスチャマッピングを有効にする
	Texture::BindTexture(_pGbuffer->GetTexture(GBUFFER_01), 0);
	Texture::BindTexture(_pGbuffer->GetTexture(GBUFFER_02), 1);
	Texture::BindTexture(_pGbuffer->GetTexture(GBUFFER_03), 2);


	pShader->SetUniform(10, 0);
	pShader->SetUniform(11, 1);
	pShader->SetUniform(12, 2);

	pShader->SetUniform(17, eyepos);
	_lightCount = 0;
	for(auto itr = pLightList.begin(); itr != pLightList.end(); ++itr)
	{
		const LightObject* pL = (*itr);

		switch(pL->GetLightMode())
		{
		case LIGHT_MODE::AMBIENT:
			pShader->SetSubroutine(SHADER_TYPE::FS, "AmbientLight");
			break;
		case LIGHT_MODE::DIRECTIONAL:
			pShader->SetSubroutine(SHADER_TYPE::FS, "DirectionalLight");
			break;
		case LIGHT_MODE::POINT:
			pShader->SetSubroutine(SHADER_TYPE::FS, "PointLight");
			break;
		case LIGHT_MODE::SPOT:
			pShader->SetSubroutine(SHADER_TYPE::FS, "SpotLight");
			break;
		default:
			continue;
			break;
		}

		s32 lightMode = (s32)pL->GetLightMode();
		if (lightMode < 0){ continue; }
		++_lightCount;
		Vector3 lightPos   = pL->GetWorldPosition();
		Color	lightColor = pL->GetColor();
		f32		lightPower = pL->GetPower();

		pShader->SetUniform(13, lightPos);			//	座標
		pShader->SetUniform(14, lightColor,false);	//	色
		pShader->SetUniform(15, lightPower);		//	強さ
	
		pShader->SetUniform(16, lightMode);			//	モード


		Render2D::RenderBuffer(nullptr);
	}
	_pLighting->End();
	pRenderer->DisableBlending();

	Texture::UnBindTexture(2);
	Texture::UnBindTexture(1);
	Texture::UnBindTexture(0);

	pShader->End();

	isErrorGL();


	//	emissive

	_pEmissive->Render(_pGbuffer->GetTexture(GBUFFER_04), _pLighting->GetTexture(LIGHT_DIFFUSE));


#endif

#ifndef RELEASE_FINAL
	_timeLightng.Stop();
#endif

}

//-----------------------------------------------------------------------------
//! 最終合成
//-----------------------------------------------------------------------------
void PhysicalBasedRendering::Finalize()
{
#ifndef RELEASE_FINAL
	_timeFinalize.Start();
#endif

	//	最終結果
	//	とりあえず画面サイズに合わせておく
	Manager::Renderer()->SetViewport(0, 0, SCRN_WIDTH, SCRN_HEIGHT);
	RenderManager* pRenderer = Manager::Renderer();

	//	ライティング結果合成
	_pFinalize->Begin();
	pRenderer->EnableBlending(BLEND_MODE::ONE, BLEND_MODE::ONE);
	Render2D::RenderBuffer(_pEmissive->GetTexture());
	Render2D::RenderBuffer(_pLighting->GetTexture(LIGHT_SPECULAR));
	pRenderer->DisableBlending();
	_pFinalize->End();

	_pBloom->Render(_pFinalize->GetTexture(0));

	_pFinalize->Begin(false);


	//	ブルーム合成
	pRenderer->EnableBlending(BLEND_MODE::ONE, BLEND_MODE::ONE);
	for(u32 i = 0; i < BloomFilter::BLOOM_BUFFER_SIZE; ++i)
	{
		Render2D::RenderBuffer(_pBloom->GetTexture(i));
	}
	pRenderer->DisableBlending();

	_pFinalize->End();

	//	トーンマッピング
	_pToneMapping->Render(_pFinalize->GetTexture(0));


	pRenderer->SetViewport(0, 0, SCRN_WIDTH, SCRN_HEIGHT);

	Texture* pTexture = nullptr;

#ifndef RELEASE_FINAL

	std::wstring wstr = L"";
	switch (_renderMode)
	{
	case RENDER_MODE::POSITION:
		//	座標
		pTexture = _pGbuffer->GetTexture(GBUFFER_01);
		wstr = L"RENDER_MODE_POSITION";
		break;
	case RENDER_MODE::NORMAL:
		//	法線
		pTexture = _pGbuffer->GetTexture(GBUFFER_02);
		wstr = L"RENDER_MODE_NORMAL";
		break;
	case RENDER_MODE::ALBEDO:
		//	反射色
		pTexture = _pGbuffer->GetTexture(GBUFFER_03);
		wstr = L"RENDER_MODE_ALBEDO";
		break;
	case RENDER_MODE::EMISSIVE:
		//	反射色
		pTexture = _pGbuffer->GetTexture(GBUFFER_04);
		wstr = L"RENDER_MODE_EMISSIVE";
		break;
	case RENDER_MODE::DIFFUSE:
		//	ディフューズ
		pTexture = _pLighting->GetTexture(LIGHT_DIFFUSE);
		wstr = L"RENDER_MODE_DIFFUSE";
		break;
	case RENDER_MODE::SPECULAR:
		//	スペキュラー
		pTexture = _pLighting->GetTexture(LIGHT_SPECULAR);
		wstr = L"RENDER_MODE_SPECULAR";
		break;
	case RENDER_MODE::BLOOM:
		pTexture = _pBloom->GetTexture(_index);
		{
			CREATE_WSTRING(L"RENDER_MODE_BLOOM Index:" << _index, wstr);
		}
		break;
	case RENDER_MODE::BRIGHTNESS:
		pTexture = _pBloom->GetBrightness();
		wstr = L"RENDER_MODE_BRIGHTNESS";
		break;
	case RENDER_MODE::LUMINANCE:
		pTexture = _pToneMapping->GetMinTexture(_luminaceCount);
		{
			f32 exposureBias = _pToneMapping->GetExposure();
			CREATE_WSTRING(L"RENDER_MODE_LUMINANCE Index:" << _luminaceCount << L" Exposure:" << exposureBias, wstr);
		}
		break;
	case RENDER_MODE::FINALIZE:
		pTexture = _pFinalize->GetTexture(0);
		wstr = L"RENDER_MODE_FINALIZE";
		break;
	case RENDER_MODE::TONEMAPPING:
		pTexture = _pToneMapping->GetTexture();
		{
			f32 exposureBias = _pToneMapping->GetExposure();
			CREATE_WSTRING(L"RENDER_MODE_TONEMAPPING" << L" Exposure:" << exposureBias,wstr);
		}
	default:
		break;
	}

#else
	pTexture = _pToneMapping->GetTexture();

#endif

	_pBufferTexture = pTexture;


#ifndef RELEASE_FINAL
	_timeFinalize.Stop();


	auto* pDebug = Manager::Debug();
	pDebug->SetDebugString(0, wstr, 255);

	CREATE_WSTRING(L"light:" << _lightCount,wstr);
	pDebug->SetDebugString(1, wstr, 255);

	CREATE_WSTRING(L"GBuffer:" << _timeGbuffer.GetTime(),wstr);
	pDebug->SetDebugString(2, wstr, 255);

	CREATE_WSTRING(L"Lighting:" << _timeLightng.GetTime(),wstr);
	pDebug->SetDebugString(3, wstr, 255);

	CREATE_WSTRING(L"Finalize:" << _timeFinalize.GetTime(), wstr);
	pDebug->SetDebugString(4, wstr, 255);

#endif
}
