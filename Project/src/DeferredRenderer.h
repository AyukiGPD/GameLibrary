//=============================================================================
//!
//!	@file	DeferredRenderer.h
//!	@brief	ディファードレンダリング
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once
//static const u32 BLOOM_BUFFER_SIZE = 4;

class BaseDeferredRenderer
{
public:
	//!	コンストラクタ
	BaseDeferredRenderer()
	: _pGbuffer(nullptr)
	, _pLighting(nullptr)
	, _pFinalize(nullptr)
	, _pBufferTexture(nullptr)
	{};
	//!	デストラクタ
	virtual ~BaseDeferredRenderer(){ Clear(); };
public:
	//!	初期化
	//!	@param	[in]	width	幅
	//!	@param	[in]	height	高さ
	//!	@retval	true	成功
	//!	@retval	false	失敗
	virtual bool Init(u32 width, u32 height) = 0;
	//!	解放
	virtual void Clear()
	{
		SafeDelete(_pGbuffer);
		SafeDelete(_pLighting);
		SafeDelete(_pFinalize);
	};

	//!	更新
	virtual void Update(){};

	//! Gバッファ描画開始
	virtual void BeginGBuffer() = 0;

	//! Gバッファ描画終了
	virtual void EndGBuffer() = 0;

	//! ライティング
	virtual void Lighting(const Matrix& matProj, const Matrix& matView) = 0;

	//! 最終合成
	virtual void Finalize() = 0;

	//!	作成したバッファの表示
	void RenderTexture();

	//!	描画するテクスチャ
	//!	@return	描画するテクスチャのポインタ
	Texture* GetBufferTexture()const
	{
		return _pBufferTexture;
	}

protected:
	std::vector<ShaderHundler> _shaderPass;		//!< シェーダー

	FrameBuffer*	_pGbuffer;		//!< GバッファFBO
	FrameBuffer*	_pLighting;		//!< ライティングFBO
	FrameBuffer*	_pFinalize; 

	Texture* _pBufferTexture;

private:

};

//=============================================================================
//!	ディファードレンダリング
//=============================================================================
class PhysicalBasedRendering : public BaseDeferredRenderer
{
public:

	//!	Gバッファの種類
	enum GBUFFER_TYPE
	{
		GBUFFER_01,
		GBUFFER_02,
		GBUFFER_03,
		GBUFFER_04,
		GBUFFER_05,
		/*
			   R     G     B      A
			+-----+-----+-----+-------+
			| ワールド座標XYZ | rough |
			+-----+-----+-----+-------+
			| ワールド法線XYZ | metal |
			+-----+-----+-----+-------+
			|  R  |  G  |  B  |   A   |
			+-----+-----+-----+-------+
		*/

		//----
		GBUFFER_COUNT_MAX,	// 最大個数
	};

	//! ライティング
	enum LIGHT_TYPE
	{
		LIGHT_DIFFUSE,
		LIGHT_SPECULAR,

		LIGHT_COUNT_MAX,
	};

	enum class DIFFUESE_MODE
	{
		LAMBERT,
		OREN_NAYER,
		NONE
	};
	enum class SPECULAR_MODE
	{
		BLINN_PHONG,
		GGX,
		COOK_TORRANCE,
		NONE
	};

	//!	パスの種類
	enum RENDER_PASS
	{
		GBUFFER,
//		GBUFFER_ARRAY,
		LIGHTING,
		MAX,
	};


	//	描画モード
	enum class RENDER_MODE
	{
		POSITION,
		NORMAL,
		ALBEDO,
		EMISSIVE,
		DIFFUSE,
		SPECULAR,
		BLOOM,
		BRIGHTNESS,
		LUMINANCE,
		FINALIZE,
		TONEMAPPING,
		MAX,
	};
public:
	//!	コンストラクタ
	PhysicalBasedRendering();
	//!	デストラクタ
	virtual ~PhysicalBasedRendering();

	//!	初期化
	//!	@param	[in]	width	幅
	//!	@param	[in]	height	高さ
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool Init(u32 width, u32 height);
	//!	解放
	void Clear();

	void Update();

	//! Gバッファ描画開始
	void BeginGBuffer();

	//! Gバッファ描画終了
	void EndGBuffer();

	//! ライティング
	void Lighting(const Matrix& matProj, const Matrix& matView);

	//! 最終合成
	void Finalize();

	//	取得関数

	//!	テクスチャ
	//!	@param	[in]	type	バッファの種類
	//!	@return	Gバッファのポインター
	RenderTarget* GetGbufferTexture(GBUFFER_TYPE type)
	{
		if (type == GBUFFER_COUNT_MAX){ return nullptr; }
		return _pGbuffer->GetTexture(type);
	}
	//!	テクスチャ
	//!	@param	[in]	type	バッファの種類
	//!	@return	ライトテクスチャ
	RenderTarget* GetLightTexture(LIGHT_TYPE type)
	{
		if (type == LIGHT_COUNT_MAX){ return nullptr; }
		return _pLighting->GetTexture(type);
	}

	//!	ブルームフィルター取得
	//!	@return	フィルターのポインタ
	BloomFilter* GetBloomFilter()
	{
		return _pBloom;
	}

	//!	トーンマッピングフィルター取得
	//!	@return	フィルターのポインタ
	ToneMappingFilter* GeToneMappingFilter()
	{
		return _pToneMapping;
	}

private:
	//!	Gバッファ生成
	//!	@param	[in]	width	横
	//!	@param	[in]	height	縦
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool CreateGBuffer(u32 width, u32 height);
	//!	ライトバッファ生成
	//!	@param	[in]	width	横
	//!	@param	[in]	height	縦
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool CreateLighting(u32 width, u32 height);
	//!	最終合成バッファ生成
	//!	@param	[in]	width	横
	//!	@param	[in]	height	縦
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool CreateFinalize(u32 width, u32 height);

private:
	BloomFilter*	_pBloom;	//!< ブルーム
	EmissiveColorFilter*	_pEmissive;	//!< ブルーム

	ToneMappingFilter* _pToneMapping;	//!< トーンマッピング

	RENDER_MODE _renderMode;		//!< 描画モード	

	s32 _index;				//!< デバッグ描画用バッファインデックス
	u32 _lightCount;		//!< ライトのカウンタ

	Timer _timeGbuffer;		//!< 時間計測
	Timer _timeLightng;		//!< 時間計測
	Timer _timeFinalize;	//!< 時間計測

	s32 _luminaceCount;
};





