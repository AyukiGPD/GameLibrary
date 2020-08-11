//=============================================================================
//!
//!	@file	Texture.cpp
//!	@brief	画像描画クラス
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

#pragma pack(push, 1)				// コンパイラーに変数の詰め込みを指示。パディング生成を抑制
	struct HeaderTGA
	{
		u8		_id;
		u8		_colorMap;			//!< カラーマップ有無   0:なし 1:あり

		u8		_type;				//!< 画像形式
									//!<  0:イメージなし
									//!<  1:インデックスカラー(256色)
									//!<  2:フルカラー
									//!<  3:白黒
									//!<  9:(RLE圧縮)インデックスカラー(256色)
									//!< 10:(RLE圧縮)フルカラー
									//!< 11:(RLE圧縮)白黒
		u16		_colorMapIndex;
		u16		_colorMapLength;
		u8		_colorMapSize;
		
		u16		_x;
		u16		_y;
		u16		_width;				//!< 画像の幅
		u16		_height;			//!< 画像の高さ

		u8		_bpp;				//!< 色深度 (Bit per pixel) 8=256色 16:65536色 24:フルカラー 32:フルカラー+α

		u8		_attribute;			//!< 属性
									//!< bit0-3:属性
									//!< bit4  :格納方向 0:左から右  1:右から左
									//!< bit5  :格納方向 0:下から上  1:上から下
									//!< bit6,7:インターリーブ（使用不可）
	};
#pragma pack(pop)

//=============================================================================
//! 画像クラス
//=============================================================================
class Image
{
public:
	//! コンストラクタ
	Image();
	//!	デストラクタ
	virtual ~Image();

	//! 初期化
	bool	setup(s32 w, s32 h);

	//! ピクセル参照
	Color&	pixel(s32 x, s32 y);

	//! 画像をUV座標で読み取り(バイリニアフィルタ)
	Color	fetch(f32 u, f32 v);

private:
	std::vector<Color>	_image;
	s32					_width;
	s32					_height;
};

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
Image::Image()
: _width(0)
, _height(0)
{
}
//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
Image::~Image()
{
}

//-----------------------------------------------------------------------------
//! ピクセル参照
//-----------------------------------------------------------------------------
Color& Image::pixel(s32 x, s32 y)
{
	return _image[y * _width + x];
}

//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool Image::setup(s32 w, s32 h)
{
	_image.resize(w * h);

	_width  = w;
	_height = h;
	return true;
}

//-----------------------------------------------------------------------------
//! 画像をUV座標で読み取り(バイリニアフィルタ)
//-----------------------------------------------------------------------------
Color Image::fetch(f32 u, f32 v)
{
	f32 fx = u * (f32)(_width  - 1);
	f32 fy = v * (f32)(_height - 1);
	
	s32 x = (s32)fx;
	s32 y = (s32)fy;
	
	f32	s = fx - (f32)x;
	f32	t = fy - (f32)y; 

	f32	r1, g1, b1, a1;
	f32	r2, g2, b2, a2;
	r1  = (f32)pixel(x  , y  ).r * (1.0f - s);
	g1  = (f32)pixel(x  , y  ).g * (1.0f - s);
	b1  = (f32)pixel(x  , y  ).b * (1.0f - s);
	a1  = (f32)pixel(x  , y  ).a * (1.0f - s);
	
	r1 += (f32)pixel(x+1, y  ).r * s;
	g1 += (f32)pixel(x+1, y  ).g * s;
	b1 += (f32)pixel(x+1, y  ).b * s;
	a1 += (f32)pixel(x+1, y  ).a * s;
	
	r2  = (f32)pixel(x  , y+1).r * (1.0f - s);
	g2  = (f32)pixel(x  , y+1).g * (1.0f - s);
	b2  = (f32)pixel(x  , y+1).b * (1.0f - s);
	a2  = (f32)pixel(x  , y+1).a * (1.0f - s);
	
	r2 += (f32)pixel(x+1, y+1).r * s;
	g2 += (f32)pixel(x+1, y+1).g * s;
	b2 += (f32)pixel(x+1, y+1).b * s;
	a2 += (f32)pixel(x+1, y+1).a * s;

	f32	fr = r1 * (1.0f - t) + r2 * t;
	f32	fg = g1 * (1.0f - t) + g2 * t;
	f32	fb = b1 * (1.0f - t) + b2 * t;
	f32	fa = a1 * (1.0f - t) + a2 * t;
	Color	color;
	color.r = max(0, min((s32)fr, 255));
	color.g = max(0, min((s32)fg, 255));
	color.b = max(0, min((s32)fb, 255));
	color.a = max(0, min((s32)fa, 255));

	return color;
}

// x より大きい最小の２のべき乗数を計算
s32	nextPow2(const u32 x)
{
	u32	count = 1;
	for(s32 valule=x; valule>0; valule >>= 1, count <<= 1 );
	return  2 * x == count ? x: count;
}

class TGAStream
{
public:
	//! コンストラクタ
	TGAStream() {}

	//! デストラクタ
	virtual ~TGAStream() {}

	//! 1ピクセル読み込み
	virtual	Color	read() = 0;
};


//=============================================================================
//! 非圧縮データストリーム
//=============================================================================
class TGAStreamRAW : public TGAStream
{
public:
	TGAStreamRAW(FILE* fp) : _fp(fp) {}

	//! 1ピクセル読み込み
	virtual	Color	read()
	{
		u8	b = fgetc(_fp);
		u8	g = fgetc(_fp);
		u8	r = fgetc(_fp);
		u8	a = fgetc(_fp);

		return Color(r, g, b, a);
	}
public:

	FILE*	_fp;
};

//=============================================================================
//! RLE圧縮データストリーム
//=============================================================================
class TGAStreamRLE : public TGAStream
{
public:
	enum class STATE
	{
		UNKNOWN,
		COMPRESSED,
		UNCOMPRESSED
	};

	TGAStreamRLE(FILE* fp)
	: _count(0)
	, _state(STATE::UNKNOWN)
	, _color(0,0,0,0)
	, _fp(fp)
	{}

	//! 1ピクセル読み込み
	virtual	Color	read()
	{
		// 繰り返しフラグ
		if( _count == 0 ) {
			u8	flagCount = fgetc(_fp);

			// 最上位ビットがセット(1のとき)の場合→「連続するデータの数」
			// 最上位ビットがセット(0のとき)の場合→「連続しないデータの数」
			if( flagCount & 0x80 )
			{
				_state = STATE::COMPRESSED;

				u8	b = fgetc(_fp);
				u8	g = fgetc(_fp);
				u8	r = fgetc(_fp);
				u8	a = fgetc(_fp);

				_color = Color(r, g, b, a);
			}
			else
			{
				_state = STATE::UNCOMPRESSED;
			}
			_count = (flagCount & 127) + 1;
		}

		if( _state == STATE::UNCOMPRESSED )
		{
			u8	b = fgetc(_fp);
			u8	g = fgetc(_fp);
			u8	r = fgetc(_fp);
			u8	a = fgetc(_fp);

			_color = Color(r, g, b, a);
		}

		_count--;

		return _color;
	}
public:
	s32		_count;
	STATE	_state;
	Color	_color;
	
	FILE*	_fp;
};

//-----------------------------------------------------------------------------
//!	テクスチャの設定
//!	@param	[in]	pTexture	テクスチャのポインタ(nullptr指定でOFF)
//!	@param	[in]	activeTexture	ActibeTextureの番号	default:0
//-----------------------------------------------------------------------------
void Texture::BindTexture(const Texture* pTexture, u32 activeTexture)
{
	if (activeTexture >= 32){ return; }

	if (pTexture)
	{
		glActiveTexture(GL_TEXTURE0 + activeTexture);
		glBindTexture(GL_TEXTURE_2D, pTexture->GetTextureID());
	}
	else
	{
		glActiveTexture(GL_TEXTURE0 + activeTexture);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

//-----------------------------------------------------------------------------
//!	テクスチャの設定
//!	@param	[in]	pTexture	テクスチャのポインタ
//!	@param	[in]	location	ロケーション
//!	@param	[in]	access		アクセスタイプ
//-----------------------------------------------------------------------------
void Texture::BindImageTexture(const Texture* pTexture, u32 location, ACCESS_TYPE access/*, s32 level, bool layerd, s32 layer*/)
{
	if (!pTexture)return;
	s32 id = pTexture->GetTextureID();
	TextureFormatData formatData = CreateTextureFormat( pTexture->GetFormat() );
//	glBindImageTexture(location, id, level, (layerd ? GL_TRUE : GL_FALSE), layer, (GLenum)access, formatData._internalFormat);
	glBindImageTexture(location, id, 0, GL_FALSE, 0, (GLenum)access, formatData._internalFormat);
}

//-----------------------------------------------------------------------------
//!	テクスチャの有効化
//!	@param	[in]	activeTexture	ActibeTextureの番号	default:0
//-----------------------------------------------------------------------------
void Texture::BindTexture(u32 activeTexture)
{
	if (activeTexture >= 32){ return; }
	glActiveTexture(GL_TEXTURE0 + activeTexture);
	glBindTexture(GL_TEXTURE_2D, _textureId);
}

//-----------------------------------------------------------------------------
//!	テクスチャの無効化
//!	@param	[in]	activeTexture	ActibeTextureの番号	default:0
//-----------------------------------------------------------------------------
void Texture::UnBindTexture(u32 activeTexture)
{
	if (activeTexture >= 32){ return; }
	glActiveTexture(GL_TEXTURE0 + activeTexture);
	glBindTexture(GL_TEXTURE_2D, 0);
}


//-----------------------------------------------------------------------------
//!	テクスチャフォーマット
//!	@param	[in]	format	フォーマット
//!	@return	フォーマットデータ 失敗時はGL_FALSE
//-----------------------------------------------------------------------------
Texture::TextureFormatData Texture::CreateTextureFormat(FORMAT format)
{
	//	クラス化させるかもしれない
	TextureFormatData result;
	switch (format)
	{
	case FORMAT::RED8:
		result._internalFormat	= GL_R8;
		result._colorFormat		= GL_RED;
		result._typeFormat		= GL_UNSIGNED_BYTE;
	case FORMAT::RED16F:
		result._internalFormat	= GL_R16F;
		result._colorFormat		= GL_RED;
		result._typeFormat		= GL_FLOAT;
	case FORMAT::RED32F:
		result._internalFormat	= GL_R32F;
		result._colorFormat		= GL_RED;
		result._typeFormat		= GL_FLOAT;
		//	RGB
	case FORMAT::RGB8:
		result._internalFormat	= GL_RGB8;
		result._colorFormat		= GL_RGB;
		result._typeFormat		= GL_UNSIGNED_BYTE;
		break;
	case FORMAT::RGB16F:
		result._internalFormat	= GL_RGB16F;
		result._colorFormat		= GL_RGB;
		result._typeFormat		= GL_HALF_FLOAT;
		break;
	case FORMAT::RGB32F:
		result._internalFormat	= GL_RGB32F;
		result._colorFormat		= GL_RGB;
		result._typeFormat		= GL_FLOAT;
		break;
	case FORMAT::RGB10A2:
		result._internalFormat	= GL_RGB10_A2;
		result._colorFormat		= GL_RGBA;
		result._typeFormat		= GL_UNSIGNED_INT_2_10_10_10_REV;
		break;
		//	RGBA
	case FORMAT::RGBA8:
		result._internalFormat	= GL_RGBA8;
		result._colorFormat		= GL_RGBA;
		result._typeFormat		= GL_UNSIGNED_BYTE;
		break;
	case FORMAT::RGBA16F:
		result._internalFormat	= GL_RGBA16F;
		result._colorFormat		= GL_RGBA;
		result._typeFormat		= GL_HALF_FLOAT;
		break;
	case FORMAT::RGBA32F:
		result._internalFormat	= GL_RGBA32F;
		result._colorFormat		= GL_RGBA;
		result._typeFormat		= GL_FLOAT;
		break;
		//	sRGB
	case FORMAT::SRGB8:
		result._internalFormat	= GL_SRGB8;
		result._colorFormat		= GL_RGB;
		result._typeFormat		= GL_UNSIGNED_BYTE;
		break;
	case FORMAT::SRGBA8:
		result._internalFormat	= GL_SRGB8_ALPHA8;
		result._colorFormat		= GL_RGBA;
		result._typeFormat		= GL_UNSIGNED_BYTE;
		break;
		//	Depth
	case FORMAT::DEPTH16:
		result._internalFormat	= GL_DEPTH_COMPONENT16;
		result._colorFormat		= GL_DEPTH_COMPONENT;
		result._typeFormat		= GL_UNSIGNED_SHORT;
		break;
	case FORMAT::DEPTH32:
		result._internalFormat	= GL_DEPTH_COMPONENT32;
		result._colorFormat		= GL_DEPTH_COMPONENT;
		result._typeFormat		= GL_UNSIGNED_INT;
		break;
	case FORMAT::DEPTH32F:
		result._internalFormat	= GL_DEPTH_COMPONENT32F;
		result._colorFormat		= GL_DEPTH_COMPONENT;
		result._typeFormat		= GL_FLOAT;
		break;
	default:
		DEBUG_LOG(L"Unsupport Format Texture");
		result._internalFormat	= GL_FALSE;
		result._colorFormat		= GL_FALSE;
		result._typeFormat		= GL_FALSE;
		break;
	}
	return result;

}

//-----------------------------------------------------------------------------
//!	テクスチャ生成
//!	@param	[in]	desc
//-----------------------------------------------------------------------------
RenderTarget* RenderTarget::Create(const Desc& desc)
{
	RenderTarget* p = new RenderTarget();
	if (p)
	{
		if(p->Init(desc._width, desc._height, desc._param) == false)
		{
			SafeDelete(p);
		}
	}
	return p;
}



//-----------------------------------------------------------------------------
//! テクスチャ作成
//!	@param	[in]	format		フォーマット
//!	@param	[in]	width		幅
//!	@param	[in]	param		高さ
//-----------------------------------------------------------------------------
bool RenderTarget::Init(s32 width, s32 height, const Param& param)
{
	static const s32 TEX_MAX_SIZE = 4096;
	if (width <= 0 || height <= 0)
	{
		return false;
	}
	else if (width >= TEX_MAX_SIZE || height >= TEX_MAX_SIZE)
	{
		ERROR_LOG("TEX_MAX_SIZE over");
	}

	TextureFormatData fromatData = CreateTextureFormat( param._format );
	if (fromatData._internalFormat == GL_FALSE){ return false; }
	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if(param._mipLevel > 0)
	{
		//	ミップマップあり
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, param._mipLevel);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)param._magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)param._minFilter);

	{
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			fromatData._internalFormat,
			width,
			height,
			0,
			fromatData._colorFormat,
			fromatData._typeFormat,
			nullptr);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	_width = width;
	_height = height;
	_param = param;
	return true;
}


//-----------------------------------------------------------------------------
//! TGAファイルの読み込み
//!	@param	[in]	fileName	ファイル名
//-----------------------------------------------------------------------------
bool AssetTexture::LoadTGA(const std::string& fileName, u32 mipLevel)
{
	HeaderTGA	header;

	FILE* fp;
	errno_t err = fopen_s(&fp, fileName.c_str(), "rb");
	if (err != 0)
	{
		ERROR_LOG("File Not Open:" << fileName);
		return false;
	}

	//-------------------------------------------------------------
	// ヘッダーの読み込み
	//-------------------------------------------------------------
	fread(&header, sizeof(header), 1, fp);

	if( header._bpp != 32 )
	{
		DEBUG_LOG(L"32bitカラー形式ではありません.");
		return false;
	}

	s32	width  = header._width;
	s32	height = header._height;

	//---- 画像クラスにイメージを読み込み
	// ここでは2の乗数になっている必要はない
	Image	image;
	image.setup(width, height);

	//-------------------------------------------------------------
	// 圧縮モード判定
	//-------------------------------------------------------------
	std::unique_ptr<TGAStream>	pStream;
	
	if( header._type & (1 << 3) )
	{
		// RLE圧縮
		pStream = std::unique_ptr<TGAStream>(new TGAStreamRLE(fp));
	}
	else
	{
		// 非圧縮
		pStream = std::unique_ptr<TGAStream>(new TGAStreamRAW(fp));
	}

	//-------------------------------------------------------------
	// イメージの取り出し
	//-------------------------------------------------------------
	if( header._attribute & (1 << 5) )
	{
		// 通常
		for( s32 y=0; y<height; y++ )
		{
			for( s32 x=0; x<width; x++ )
			{
				image.pixel(x, y) = pStream->read();
			}
		}
	}
	else {
		// 上下反転
		for( s32 y=height-1; y>=0; y-- )
		{
			for( s32 x=0; x<width; x++ )
			{
				image.pixel(x, y) = pStream->read();
			}
		}
	}
	// ファイルアクセス終了
	fclose(fp);

	//---- OpenGLで画像の転送
	// OpenGLでは2の乗数のサイズではないテクスチャをサポートしていないため
	// リサイズを行う
	s32	alignedW = nextPow2(width);
	s32	alignedH = nextPow2(height);
	
	std::vector<Color>	alignedImage;
	alignedImage.resize(alignedW * alignedH);

	for( s32 y=0; y<alignedH; y++ )
	{
		for( s32 x=0; x<alignedW; x++ )
		{
			f32	u = (f32)x / (f32)alignedW;
			f32	v = (f32)y / (f32)alignedH;

			alignedImage[y * alignedW + x] = image.fetch(u, v);
		}
	}

	// 画像転送
	glTexImage2D(GL_TEXTURE_2D,		// テクスチャタイプ
				 0,					// ミップマップ段数 (0で無効)
				 GL_RGBA,			// 透明度あり
				 alignedW,			// 幅
				 alignedH,			// 高さ
				 0,					// テクスチャボーダーON/OFF
				 GL_RGBA,			// テクスチャのピクセル形式
				 GL_UNSIGNED_BYTE,	// ピクセル1要素のサイズ
				 &alignedImage[0]);			// 画像の場所

	if(mipLevel > 0)
	{
		//	ミップマップあり
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	return true;
}

//-----------------------------------------------------------------------------
//!	ファイルの読み込み
//!	@param	[in]	fileName	ファイル名
//-----------------------------------------------------------------------------
bool AssetTexture::LoadFromFile(const std::string& fileName, u32 mipLevel)
{
	// 文字コードをワイド文字列に変換
	// 【注意】本来はこの箇所は文字列バッファ長の考慮の他に文字列終端コードを処理するよりセキュアな対応が好ましいです。
	wchar_t		path[ MAX_PATH ];
	size_t		pathLength = 0;

	setlocale(LC_ALL, "jpn");
	if( mbstowcs_s( &pathLength,			//	[out]	変換された文字数
					&path[0],				//	[out]	変換されたワイド文字列を格納するバッファのアドレス(変換先)
					MAX_PATH,				//	[in]	出力側のバッファのサイズ(単位:文字数)
					fileName.c_str(),		//	[in]	マルチバイト文字列のアドレス(変換元)
					_TRUNCATE ) != 0 )		//	[in]	出力先に格納するワイド文字の最大数
	{
		return false;
	}

	//  GDI+オブジェクト（画像展開に必要）
	Gdiplus::GdiplusStartupInput	gdiplusStartupInput;
	ULONG_PTR						gdiplusToken;

	//----		GDI+の初期設定
	if( Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr) !=  Gdiplus::Ok )
	{
		return false;
	}

	//-------------------------------------------------------------
	// 画像の読み込み
	//-------------------------------------------------------------
	bool	result = false;
	u32		width  = 0;			// 画像の幅
	u32		height = 0;			//  〃  高さ
 
	u8*	pImage = nullptr;
	
	//--- 画像ファイルを開く
	//  【対応画像形式】  BMP, JPEG, PNG, GIF, TIFF, WMF, EMF
	Gdiplus::Bitmap*    pBitmap = Gdiplus::Bitmap::FromFile(path);
	if( pBitmap && pBitmap->GetLastStatus() == Gdiplus::Ok )
	{
		//---- 画像サイズ分の領域確保
		width  = pBitmap->GetWidth();
		height = pBitmap->GetHeight();
 
		// ■ロードする画像のメモリの解像度を変更/設定（この位置に任意に記述して下さい）

		// サイズを保存しておく
		_width  = width;
		_height = height;
		
		pImage = new u8[width * height * 4];
		if( pImage )
		{
			result = true;
		}
	}
 
	//---- 画像イメージの読み込み
	if( result == true )
	{
		for(u32 y = 0; y < height; y++)
		{
			for(u32 x = 0; x < width; x++)
			{
				Gdiplus::Color  srcColor;
				pBitmap->GetPixel(x, y, &srcColor);

				u8 r = srcColor.GetR();
				u8 g = srcColor.GetG();
				u8 b = srcColor.GetB();
				u8 a = srcColor.GetA();

				// ■ピクセルへの描画（この位置に任意に記述して下さい）

				pImage[(y * width + x) * 4 + 0] = r;
				pImage[(y * width + x) * 4 + 1] = g;
				pImage[(y * width + x) * 4 + 2] = b;
				pImage[(y * width + x) * 4 + 3] = a;

			}
		}
	}

	
	//---- OpenGLで画像の転送
	glTexImage2D(GL_TEXTURE_2D,				// テクスチャタイプ
					0,						// ミップマップ段数 (0で無効)
					GL_RGBA,
					width,					// 幅
					height,					// 高さ
					0,						// テクスチャボーダーON/OFF
					GL_RGBA,				// テクスチャのピクセル形式
					GL_UNSIGNED_BYTE,		// ピクセル1要素のサイズ
					pImage);				// 画像の場所


	//	ミップマップ自動生成
	if(mipLevel > 0)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	SafeDeleteArray(pImage);
	SafeDelete(pBitmap);
	//---- GDI+の解放
	Gdiplus::GdiplusShutdown(gdiplusToken);

	_param = Param(FORMAT::RGBA8,mipLevel);


	return result;
}


//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
AssetTexture::AssetTexture()
{
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
AssetTexture::~AssetTexture()
{
}

//-----------------------------------------------------------------------------
//! 読み込み
//!	@param	[in]	fileName	画像ファイル名
//-----------------------------------------------------------------------------
bool AssetTexture::Load(const std::string& fileName, u32 mipLevel)
{
	//-------------------------------------------------------------
	// (1) テクスチャIDを作成
	//-------------------------------------------------------------
	glGenTextures(1, &_textureId);

	//-------------------------------------------------------------
	// (2) IDをGPUに設定
	//-------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, _textureId);

	//-------------------------------------------------------------
	// (3) テクスチャーのフィルター設定
	//-------------------------------------------------------------

	if(mipLevel > 0)
	{
		//	ミップマップあり
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipLevel);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		//	ミップマップなし
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	isErrorGL();

	//-------------------------------------------------------------
	// (4) 画像イメージの転送
	//-------------------------------------------------------------
	bool result = false;
	s32 index = fileName.rfind(".", fileName.size() - 1);
	if(index != std::string::npos)
	{
		std::string ext = fileName.substr(index);	//	拡張子

		if( (ext == ".tga") || (ext == ".TGA") )
		{
			result = LoadTGA(fileName, mipLevel);
		}
		result = LoadFromFile(fileName, mipLevel);
	}
	return result;
}
