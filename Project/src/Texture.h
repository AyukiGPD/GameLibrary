//=============================================================================
//!
//!	@file	Texture.h
//!	@brief	画像描画クラス
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//-------------------------------------------------------------------
//! テクスチャ
//-------------------------------------------------------------------
class Texture
{
public:
	//	https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml
	//!	テクスチャフォーマット
	enum class FORMAT
	{
		UNKNOWN,

		RED8,
		RED16F,
		RED32F,
		//	RGB
		RGB8,

		RGB10A2,

		RGB16F,
		RGB32F,

		//	RGBA
		RGBA8,
		RGBA16F,
		RGBA32F,

		//	sRGB
		SRGB8,
		SRGBA8,


		DEPTH16,
		DEPTH32,
		DEPTH32F,
	};

	//!	アクセスタイプ
	enum class ACCESS_TYPE
	{
		READ			= GL_READ_ONLY,
		WRITE			= GL_WRITE_ONLY,
		READ_WRITE		= GL_READ_WRITE
	};

	//!	テクスチャフィルタ
	enum class FILTER
	{
		NEAREST					= GL_NEAREST,
		LINEAR					= GL_LINEAR,
		NEAREST_MIPMAP_NEAREST	= GL_NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST	= GL_LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR	= GL_NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR	= GL_LINEAR_MIPMAP_LINEAR,
	};

	//!	テクスチャパラメータ
	struct Param
	{
		//!	コンストラクタ
		Param()
		: _format(FORMAT::UNKNOWN)
		, _minFilter(FILTER::LINEAR)
		, _magFilter(FILTER::LINEAR)
		, _mipLevel(0)
		{}

		//!	コンストラクタ
		//!	@param	[in]	format		フォーマット
		//!	@param	[in]	minFilter	テクスチャフィルタ	default:LINEAR
		//!	@param	[in]	magFilter	テクスチャフィルタ	default:LINEAR
		//!	@param	[in]	mipLevel	ミップレベル		default:0
		Param(FORMAT format, FILTER minFilter = FILTER::LINEAR, FILTER magFilter = FILTER::LINEAR, u32 mipLevel = 0)
		: _format(format)
		, _minFilter(minFilter)
		, _magFilter(magFilter)
		, _mipLevel(mipLevel)
		{}

		//!	コンストラクタ
		//!	@param	[in]	format		フォーマット
		//!	@param	[in]	mipLevel	ミップレベル
		Param(FORMAT format, u32 mipLevel)
		: _format(format)
		, _mipLevel(mipLevel)
		, _minFilter(mipLevel > 0 ? FILTER::LINEAR_MIPMAP_LINEAR : FILTER::LINEAR)
		, _magFilter(FILTER::LINEAR)
		{}

		FORMAT _format;		//!< フォーマット
		FILTER _minFilter;	//!< テクスチャフィルタ
		FILTER _magFilter;	//!< テクスチャフィルタ
		u32 _mipLevel;		//!< ミップレベル
	};


public:
	//! コンストラクタ
	Texture()
	: _textureId(0)
	, _width(0)
	, _height(0)
	, _param(FORMAT::UNKNOWN, FILTER::LINEAR, FILTER::LINEAR, 0)
	{
	}

	//! デストラクタ
	virtual ~Texture()
	{
		Clear();
	}

	//!	解放
	virtual void Clear()
	{
		if (_textureId != 0)
		{
			glDeleteTextures(1, &_textureId);
			_textureId = 0;
		}
		_width  = 0;
		_height = 0;
		
	}

	//! OpenGLのテクスチャIDを取得
	//!	@return	テクスチャID
	GLuint GetTextureID()const { return _textureId; }
	//! 幅を取得
	//!	@return	幅
	s32 GetWidth()const { return _width; }
	//! 高さを取得
	//!	@return	高さ
	s32 GetHeight()const { return _height; }

	//!	テクスチャの有無
	//!	@retval	false	テクスチャ無し
	bool IsEmpty()const { return(_textureId == 0); }

	//!	テクスチャの有効化
	//!	@param	[in]	activeTexture	ActibeTextureの番号	default:0
	void BindTexture(u32 activeTexture = 0);
	//!	テクスチャの無効化
	//!	@param	[in]	activeTexture	ActibeTextureの番号	default:0
	static void UnBindTexture(u32 activeTexture = 0);
	//!	テクスチャの設定
	//!	@param	[in]	pTexture	テクスチャのポインタ(nullptr指定でOFF)
	//!	@param	[in]	activeTexture	ActibeTextureの番号	default:0
	static void BindTexture(const Texture* pTexture, u32 activeTexture = 0);

	//	何かわからないから一部引数は保留
	//!	テクスチャの設定
	//!	@param	[in]	pTexture	テクスチャのポインタ
	//!	@param	[in]	location	ロケーション
	//!	@param	[in]	access		アクセスタイプ
	static void BindImageTexture(const Texture* pTexture, u32 location, ACCESS_TYPE access/*, s32 level = 0, bool layerd = false, s32 layer = 0*/);

	//! フォーマットの取得
	Param GetParam() const
	{
		return _param;
	}
	//! フォーマットの取得
	FORMAT GetFormat() const
	{
		return _param._format;
	}
	//! フォーマットの取得
	u32 GetMipLevel() const
	{
		return _param._mipLevel;
	}
protected:
	//!	テクスチャのフォーマット
	struct TextureFormatData
	{
		u32 _internalFormat;
		u32 _colorFormat;
		u32 _typeFormat;
	};
	//!	テクスチャフォーマット
	//!	@param	[in]	format	フォーマット
	//!	@return	フォーマットデータ 失敗時はGL_FALSE
	static TextureFormatData CreateTextureFormat(FORMAT format);
protected:
	GLuint	_textureId;	//!< テクスチャID
	s32		_width;		//!< 幅
	s32		_height;	//!< 高さ

	Param _param;

};

// 描画ターゲット
class RenderTarget : public Texture
{
public:

	struct Desc
	{
		//!	コンストラクタ
		Desc()
		: _width(0)
		, _height(0)
		, _param(FORMAT::UNKNOWN, FILTER::LINEAR, FILTER::LINEAR, 0)
		{}
		//!	コンストラクタ
		//!	@param	[in]	width		幅
		//!	@param	[in]	height		高さ
		//!	@param	[in]	param		テクスチャパラメータ
		Desc(s32 width, s32 height, const Param& param)
		: _width(width)
		, _height(height)
		, _param(param)
		{}


		s32 _width;		//!< 幅
		s32 _height;	//!< 高さ
		Param _param;	//!< パラメータ
	};



	//! コンストラクタ
	RenderTarget()
	{
	}

	//! デストラクタ
	virtual ~RenderTarget()
	{
		Clear();
	}
	
	//! 解放
	virtual void Clear()
	{
		Texture::Clear();
	}

	//!	テクスチャ生成
	//!	@param	[in]	desc
	static RenderTarget* Create(const Desc& desc);

	//! テクスチャ作成
	//!	@param	[in]	width		幅
	//!	@param	[in]	height		高さ
	//!	@param	[in]	param		フォーマット
	bool Init(s32 width, s32 height, const Param& param);

private:
};

//	画像テクスチャ
class AssetTexture : public Texture
{
public:
	//! コンストラクタ
	AssetTexture();

	//! デストラクタ
	virtual ~AssetTexture();

	//! 読み込み
	//!	@param	[in]	fileName	ファイル名
	//!	@retval	false	失敗
	bool Load(const std::string& fileName, u32 mipLevel = 0);

private:
	//! TGAファイルの読み込み
	//!	@param	[in]	fileName	ファイル名
	//!	@retval	false	失敗
	bool LoadTGA(const std::string& fileName, u32 mipLevel);
	//!	ファイルの読み込み
	//!	@param	[in]	fileName	ファイル名
	//!	@retval	false	失敗
	bool LoadFromFile(const std::string& fileName, u32 mipLevel);
};

