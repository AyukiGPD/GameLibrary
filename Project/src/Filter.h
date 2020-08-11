//=============================================================================
//!
//!	@file	Filter.h
//!	@brief	フィルター
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//=============================================================================
//!	テクスチャエフェクト基底クラス
//=============================================================================
class FilterBase
{
public:
	FilterBase()
	{}
	virtual ~FilterBase(){}
	//!	解放
	virtual void Clear()
	{
	}
	//!	初期化
	//!	@param	[in]	width	幅
	//!	@param	[in]	height	高さ
	//!	@param	[in]	param	テクスチャパラメータ
	//!	@retval	true	成功
	//!	@retval	false	失敗
	virtual bool Init(u32 width, u32 height, const Texture::Param& param) = 0;

	//!	テクスチャ描画
	//!	@param	[in]	texture	テクスチャ
	virtual void Render(const Texture* pTexture)
	{
		UNUSED(pTexture);
	}

};


//static const u32 BLUR_PASS_SIZE = 2;

//=============================================================================
//!	ブラーエフェクトクラス
//=============================================================================
class BlurFilter : public FilterBase
{
public:
	//!	コンストラクタ
	BlurFilter();
	//!	デストラクタ
	virtual ~BlurFilter();
	//!	解放
	void Clear();
	//!	初期化
	//!	@param	[in]	width	幅
	//!	@param	[in]	height	高さ
	//!	@param	[in]	param	テクスチャパラメータ
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool Init(u32 width, u32 height, const Texture::Param& param);

	//!	テクスチャ描画
	//!	@param	[in]	texture	テクスチャ
	void Render(const Texture* pTexture);


	//!	テクスチャ取得
	//!	@param	[in]	index	テクスチャ番号	default:1
	//!	@return	テクスチャのポインタ 失敗時はnullptr;
	Texture* GetTexture()const
	{
		if(!_pBlur){ return nullptr; }
		return _pBlur->GetTexture(0);
	}


private:
	//!	シェーダー読み込み内部関数
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool ShaderLoad();
private:
	FrameBuffer* _pBlur;
	ShaderHundler _shader;
};

//=============================================================================
//!	エミッシブカラー補正フィルター
//=============================================================================
class EmissiveColorFilter : public FilterBase
{
public:
	//!	コンストラクタ
	EmissiveColorFilter();
	//!	デストラクタ
	virtual ~EmissiveColorFilter();

	//!	初期化
	//!	@param	[in]	width	幅
	//!	@param	[in]	height	高さ
	//!	@param	[in]	param	テクスチャパラメータ
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool Init(u32 width, u32 height, const Texture::Param& param);

	//!	テクスチャ描画
	//!	@param	[in]	pTexEmissive	Emissiveテクスチャ
	//!	@param	[in]	pTexDiffuse		Diffuseテクスチャ
	void Render(const Texture* pTexEmissive,const Texture* pTexDiffuse);

	//!	テクスチャ取得
	//!	@param	[in]	index	テクスチャ番号	default:1
	//!	@return	テクスチャのポインタ 失敗時はnullptr;
	Texture* GetTexture()const
	{
		if(!_pEmissive){ return nullptr; }
		return _pEmissive->GetTexture(0);
	}
private:
	//!	シェーダー読み込み内部関数
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool ShaderLoad();

private:
	FrameBuffer* _pEmissive;
	ShaderHundler _shader;
};


//=============================================================================
//!	トーンマッピングクラス
//=============================================================================
class ToneMappingFilter : public FilterBase
{
public:
	//!	コンストラクタ
	ToneMappingFilter();
	//!	デストラクタ
	virtual ~ToneMappingFilter();
	//!	解放
	void Clear();
	//!	初期化
	//!	@param	[in]	width	幅
	//!	@param	[in]	height	高さ
	//!	@param	[in]	param	テクスチャパラメータ
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool Init(u32 width, u32 height, const Texture::Param& param);

	//!	テクスチャ描画
	//!	@param	[in]	texture	テクスチャ
	void Render(const Texture* pTexture);


	//!	テクスチャ取得
	//!	@param	[in]	index	テクスチャ番号	default:0
	//!	@return	テクスチャのポインタ 失敗時はnullptr;
	Texture* GetTexture()const
	{
		if(!_pTonemapping){ return nullptr; }
		return _pTonemapping->GetTexture(0);
	}

	//!	テクスチャ取得
	//!	@param	[in]	index	テクスチャ番号	default:0
	//!	@return	テクスチャのポインタ 失敗時はnullptr;
	Texture* GetMinTexture(u32 index)const
	{
		if(index >= _pMinBuffer.size()){ return nullptr; }
		if (!_pMinBuffer[index]){ return nullptr; }
		return _pMinBuffer[index]->GetTexture(0);
	}

	//!	最大輝度設定
	void SetExposureMax(f32 exposureBiasMax)
	{
		_exposureBiasMax = exposureBiasMax;
	}
	//!	最小輝度設定
	void SetExposureMin(f32 exposureBiasMin)
	{
		_exposureBiasMin = exposureBiasMin;
	}

	//!	最大輝度取得
	f32 GetExposureMax()const
	{
		return _exposureBiasMax;
	}
	//!	最小輝度取得
	f32 GetExposureMin()const
	{
		return _exposureBiasMin;
	}


	//!	平均輝度
	f32 GetExposure()const
	{
		return _exposureBias;
	}

	//!	輝度取得用縮小バッファ
	size_t GetBufferSize()const
	{
		return _pMinBuffer.size();
	}

private:
	//!	シェーダー読み込み内部関数
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool ShaderLoad();

private:

	FrameBuffer*				_pTonemapping;	//!< トーンマッピングFB
	std::vector<FrameBuffer*>	_pMinBuffer;	//!< 輝度取得用縮小バッファ

	ShaderHundler _shaderLuminance;			//!< 輝度取得シェーダー
	ShaderHundler _shaderToneMapping;		//!< トーンマッピングシェーダー

	f32 _exposureBias;			//!< 平均輝度
	f32 _exposureOffset;		//!< 輝度差分

	f32 _exposureBiasMax;
	f32 _exposureBiasMin;

	u32 _minBufferCount;				//!< バッファカウント
};


//=============================================================================
//!	ブルームフィルター
//=============================================================================
class BloomFilter : public FilterBase
{
public:
	//!	コンストラクタ
	BloomFilter();
	//!	デストラクタ
	virtual ~BloomFilter();

	//!	解放
	void Clear();

	//!	初期化
	//!	@param	[in]	width	幅
	//!	@param	[in]	height	高さ
	//!	@param	[in]	param	テクスチャパラメータ
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool Init(u32 width, u32 height, const Texture::Param& param);

	//!	テクスチャ描画
	//!	@param	[in]	texture	テクスチャ
	void Render(const Texture* pTexture);

	//!	輝度取得
	//!	@return	輝度テクスチャ
	Texture* GetBrightness()const
	{
		return _pBrightness->GetTexture(0);
	}
	
	//!	ブルームバッファ取得
	//!	@param	[in]	index	バッファインデックス
	//!	@return	テクスチャ
	Texture* GetTexture(u32 index)const
	{
		if(index >= BLOOM_BUFFER_SIZE){ return nullptr; }
		if(!_pBlooms[index]){ return nullptr; }
		return _pBlooms[index]->GetTexture();
	}

	static const u32 BLOOM_BUFFER_SIZE = 7;	//!< ブルームバッファサイズ

private:
	FrameBuffer*	_pBrightness;				//!< 輝度
	std::array<BlurFilter*, BLOOM_BUFFER_SIZE>		_pBlooms;	//!< ブラーフィルター
	ShaderHundler	_shader;					//!< シェーダー番号
};



