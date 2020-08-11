//=============================================================================
//!
//!	@file	Texture.cpp
//!	@brief	�摜�`��N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

#pragma pack(push, 1)				// �R���p�C���[�ɕϐ��̋l�ߍ��݂��w���B�p�f�B���O������}��
	struct HeaderTGA
	{
		u8		_id;
		u8		_colorMap;			//!< �J���[�}�b�v�L��   0:�Ȃ� 1:����

		u8		_type;				//!< �摜�`��
									//!<  0:�C���[�W�Ȃ�
									//!<  1:�C���f�b�N�X�J���[(256�F)
									//!<  2:�t���J���[
									//!<  3:����
									//!<  9:(RLE���k)�C���f�b�N�X�J���[(256�F)
									//!< 10:(RLE���k)�t���J���[
									//!< 11:(RLE���k)����
		u16		_colorMapIndex;
		u16		_colorMapLength;
		u8		_colorMapSize;
		
		u16		_x;
		u16		_y;
		u16		_width;				//!< �摜�̕�
		u16		_height;			//!< �摜�̍���

		u8		_bpp;				//!< �F�[�x (Bit per pixel) 8=256�F 16:65536�F 24:�t���J���[ 32:�t���J���[+��

		u8		_attribute;			//!< ����
									//!< bit0-3:����
									//!< bit4  :�i�[���� 0:������E  1:�E���獶
									//!< bit5  :�i�[���� 0:�������  1:�ォ�牺
									//!< bit6,7:�C���^�[���[�u�i�g�p�s�j
	};
#pragma pack(pop)

//=============================================================================
//! �摜�N���X
//=============================================================================
class Image
{
public:
	//! �R���X�g���N�^
	Image();
	//!	�f�X�g���N�^
	virtual ~Image();

	//! ������
	bool	setup(s32 w, s32 h);

	//! �s�N�Z���Q��
	Color&	pixel(s32 x, s32 y);

	//! �摜��UV���W�œǂݎ��(�o�C���j�A�t�B���^)
	Color	fetch(f32 u, f32 v);

private:
	std::vector<Color>	_image;
	s32					_width;
	s32					_height;
};

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
Image::Image()
: _width(0)
, _height(0)
{
}
//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
Image::~Image()
{
}

//-----------------------------------------------------------------------------
//! �s�N�Z���Q��
//-----------------------------------------------------------------------------
Color& Image::pixel(s32 x, s32 y)
{
	return _image[y * _width + x];
}

//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool Image::setup(s32 w, s32 h)
{
	_image.resize(w * h);

	_width  = w;
	_height = h;
	return true;
}

//-----------------------------------------------------------------------------
//! �摜��UV���W�œǂݎ��(�o�C���j�A�t�B���^)
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

// x ���傫���ŏ��̂Q�ׂ̂��搔���v�Z
s32	nextPow2(const u32 x)
{
	u32	count = 1;
	for(s32 valule=x; valule>0; valule >>= 1, count <<= 1 );
	return  2 * x == count ? x: count;
}

class TGAStream
{
public:
	//! �R���X�g���N�^
	TGAStream() {}

	//! �f�X�g���N�^
	virtual ~TGAStream() {}

	//! 1�s�N�Z���ǂݍ���
	virtual	Color	read() = 0;
};


//=============================================================================
//! �񈳏k�f�[�^�X�g���[��
//=============================================================================
class TGAStreamRAW : public TGAStream
{
public:
	TGAStreamRAW(FILE* fp) : _fp(fp) {}

	//! 1�s�N�Z���ǂݍ���
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
//! RLE���k�f�[�^�X�g���[��
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

	//! 1�s�N�Z���ǂݍ���
	virtual	Color	read()
	{
		// �J��Ԃ��t���O
		if( _count == 0 ) {
			u8	flagCount = fgetc(_fp);

			// �ŏ�ʃr�b�g���Z�b�g(1�̂Ƃ�)�̏ꍇ���u�A������f�[�^�̐��v
			// �ŏ�ʃr�b�g���Z�b�g(0�̂Ƃ�)�̏ꍇ���u�A�����Ȃ��f�[�^�̐��v
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
//!	�e�N�X�`���̐ݒ�
//!	@param	[in]	pTexture	�e�N�X�`���̃|�C���^(nullptr�w���OFF)
//!	@param	[in]	activeTexture	ActibeTexture�̔ԍ�	default:0
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
//!	�e�N�X�`���̐ݒ�
//!	@param	[in]	pTexture	�e�N�X�`���̃|�C���^
//!	@param	[in]	location	���P�[�V����
//!	@param	[in]	access		�A�N�Z�X�^�C�v
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
//!	�e�N�X�`���̗L����
//!	@param	[in]	activeTexture	ActibeTexture�̔ԍ�	default:0
//-----------------------------------------------------------------------------
void Texture::BindTexture(u32 activeTexture)
{
	if (activeTexture >= 32){ return; }
	glActiveTexture(GL_TEXTURE0 + activeTexture);
	glBindTexture(GL_TEXTURE_2D, _textureId);
}

//-----------------------------------------------------------------------------
//!	�e�N�X�`���̖�����
//!	@param	[in]	activeTexture	ActibeTexture�̔ԍ�	default:0
//-----------------------------------------------------------------------------
void Texture::UnBindTexture(u32 activeTexture)
{
	if (activeTexture >= 32){ return; }
	glActiveTexture(GL_TEXTURE0 + activeTexture);
	glBindTexture(GL_TEXTURE_2D, 0);
}


//-----------------------------------------------------------------------------
//!	�e�N�X�`���t�H�[�}�b�g
//!	@param	[in]	format	�t�H�[�}�b�g
//!	@return	�t�H�[�}�b�g�f�[�^ ���s����GL_FALSE
//-----------------------------------------------------------------------------
Texture::TextureFormatData Texture::CreateTextureFormat(FORMAT format)
{
	//	�N���X�������邩������Ȃ�
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
//!	�e�N�X�`������
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
//! �e�N�X�`���쐬
//!	@param	[in]	format		�t�H�[�}�b�g
//!	@param	[in]	width		��
//!	@param	[in]	param		����
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
		//	�~�b�v�}�b�v����
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
//! TGA�t�@�C���̓ǂݍ���
//!	@param	[in]	fileName	�t�@�C����
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
	// �w�b�_�[�̓ǂݍ���
	//-------------------------------------------------------------
	fread(&header, sizeof(header), 1, fp);

	if( header._bpp != 32 )
	{
		DEBUG_LOG(L"32bit�J���[�`���ł͂���܂���.");
		return false;
	}

	s32	width  = header._width;
	s32	height = header._height;

	//---- �摜�N���X�ɃC���[�W��ǂݍ���
	// �����ł�2�̏搔�ɂȂ��Ă���K�v�͂Ȃ�
	Image	image;
	image.setup(width, height);

	//-------------------------------------------------------------
	// ���k���[�h����
	//-------------------------------------------------------------
	std::unique_ptr<TGAStream>	pStream;
	
	if( header._type & (1 << 3) )
	{
		// RLE���k
		pStream = std::unique_ptr<TGAStream>(new TGAStreamRLE(fp));
	}
	else
	{
		// �񈳏k
		pStream = std::unique_ptr<TGAStream>(new TGAStreamRAW(fp));
	}

	//-------------------------------------------------------------
	// �C���[�W�̎��o��
	//-------------------------------------------------------------
	if( header._attribute & (1 << 5) )
	{
		// �ʏ�
		for( s32 y=0; y<height; y++ )
		{
			for( s32 x=0; x<width; x++ )
			{
				image.pixel(x, y) = pStream->read();
			}
		}
	}
	else {
		// �㉺���]
		for( s32 y=height-1; y>=0; y-- )
		{
			for( s32 x=0; x<width; x++ )
			{
				image.pixel(x, y) = pStream->read();
			}
		}
	}
	// �t�@�C���A�N�Z�X�I��
	fclose(fp);

	//---- OpenGL�ŉ摜�̓]��
	// OpenGL�ł�2�̏搔�̃T�C�Y�ł͂Ȃ��e�N�X�`�����T�|�[�g���Ă��Ȃ�����
	// ���T�C�Y���s��
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

	// �摜�]��
	glTexImage2D(GL_TEXTURE_2D,		// �e�N�X�`���^�C�v
				 0,					// �~�b�v�}�b�v�i�� (0�Ŗ���)
				 GL_RGBA,			// �����x����
				 alignedW,			// ��
				 alignedH,			// ����
				 0,					// �e�N�X�`���{�[�_�[ON/OFF
				 GL_RGBA,			// �e�N�X�`���̃s�N�Z���`��
				 GL_UNSIGNED_BYTE,	// �s�N�Z��1�v�f�̃T�C�Y
				 &alignedImage[0]);			// �摜�̏ꏊ

	if(mipLevel > 0)
	{
		//	�~�b�v�}�b�v����
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	return true;
}

//-----------------------------------------------------------------------------
//!	�t�@�C���̓ǂݍ���
//!	@param	[in]	fileName	�t�@�C����
//-----------------------------------------------------------------------------
bool AssetTexture::LoadFromFile(const std::string& fileName, u32 mipLevel)
{
	// �����R�[�h�����C�h������ɕϊ�
	// �y���Ӂz�{���͂��̉ӏ��͕�����o�b�t�@���̍l���̑��ɕ�����I�[�R�[�h������������Z�L���A�ȑΉ����D�܂����ł��B
	wchar_t		path[ MAX_PATH ];
	size_t		pathLength = 0;

	setlocale(LC_ALL, "jpn");
	if( mbstowcs_s( &pathLength,			//	[out]	�ϊ����ꂽ������
					&path[0],				//	[out]	�ϊ����ꂽ���C�h��������i�[����o�b�t�@�̃A�h���X(�ϊ���)
					MAX_PATH,				//	[in]	�o�͑��̃o�b�t�@�̃T�C�Y(�P��:������)
					fileName.c_str(),		//	[in]	�}���`�o�C�g������̃A�h���X(�ϊ���)
					_TRUNCATE ) != 0 )		//	[in]	�o�͐�Ɋi�[���郏�C�h�����̍ő吔
	{
		return false;
	}

	//  GDI+�I�u�W�F�N�g�i�摜�W�J�ɕK�v�j
	Gdiplus::GdiplusStartupInput	gdiplusStartupInput;
	ULONG_PTR						gdiplusToken;

	//----		GDI+�̏����ݒ�
	if( Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr) !=  Gdiplus::Ok )
	{
		return false;
	}

	//-------------------------------------------------------------
	// �摜�̓ǂݍ���
	//-------------------------------------------------------------
	bool	result = false;
	u32		width  = 0;			// �摜�̕�
	u32		height = 0;			//  �V  ����
 
	u8*	pImage = nullptr;
	
	//--- �摜�t�@�C�����J��
	//  �y�Ή��摜�`���z  BMP, JPEG, PNG, GIF, TIFF, WMF, EMF
	Gdiplus::Bitmap*    pBitmap = Gdiplus::Bitmap::FromFile(path);
	if( pBitmap && pBitmap->GetLastStatus() == Gdiplus::Ok )
	{
		//---- �摜�T�C�Y���̗̈�m��
		width  = pBitmap->GetWidth();
		height = pBitmap->GetHeight();
 
		// �����[�h����摜�̃������̉𑜓x��ύX/�ݒ�i���̈ʒu�ɔC�ӂɋL�q���ĉ������j

		// �T�C�Y��ۑ����Ă���
		_width  = width;
		_height = height;
		
		pImage = new u8[width * height * 4];
		if( pImage )
		{
			result = true;
		}
	}
 
	//---- �摜�C���[�W�̓ǂݍ���
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

				// ���s�N�Z���ւ̕`��i���̈ʒu�ɔC�ӂɋL�q���ĉ������j

				pImage[(y * width + x) * 4 + 0] = r;
				pImage[(y * width + x) * 4 + 1] = g;
				pImage[(y * width + x) * 4 + 2] = b;
				pImage[(y * width + x) * 4 + 3] = a;

			}
		}
	}

	
	//---- OpenGL�ŉ摜�̓]��
	glTexImage2D(GL_TEXTURE_2D,				// �e�N�X�`���^�C�v
					0,						// �~�b�v�}�b�v�i�� (0�Ŗ���)
					GL_RGBA,
					width,					// ��
					height,					// ����
					0,						// �e�N�X�`���{�[�_�[ON/OFF
					GL_RGBA,				// �e�N�X�`���̃s�N�Z���`��
					GL_UNSIGNED_BYTE,		// �s�N�Z��1�v�f�̃T�C�Y
					pImage);				// �摜�̏ꏊ


	//	�~�b�v�}�b�v��������
	if(mipLevel > 0)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	SafeDeleteArray(pImage);
	SafeDelete(pBitmap);
	//---- GDI+�̉��
	Gdiplus::GdiplusShutdown(gdiplusToken);

	_param = Param(FORMAT::RGBA8,mipLevel);


	return result;
}


//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
AssetTexture::AssetTexture()
{
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
AssetTexture::~AssetTexture()
{
}

//-----------------------------------------------------------------------------
//! �ǂݍ���
//!	@param	[in]	fileName	�摜�t�@�C����
//-----------------------------------------------------------------------------
bool AssetTexture::Load(const std::string& fileName, u32 mipLevel)
{
	//-------------------------------------------------------------
	// (1) �e�N�X�`��ID���쐬
	//-------------------------------------------------------------
	glGenTextures(1, &_textureId);

	//-------------------------------------------------------------
	// (2) ID��GPU�ɐݒ�
	//-------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, _textureId);

	//-------------------------------------------------------------
	// (3) �e�N�X�`���[�̃t�B���^�[�ݒ�
	//-------------------------------------------------------------

	if(mipLevel > 0)
	{
		//	�~�b�v�}�b�v����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipLevel);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		//	�~�b�v�}�b�v�Ȃ�
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	isErrorGL();

	//-------------------------------------------------------------
	// (4) �摜�C���[�W�̓]��
	//-------------------------------------------------------------
	bool result = false;
	s32 index = fileName.rfind(".", fileName.size() - 1);
	if(index != std::string::npos)
	{
		std::string ext = fileName.substr(index);	//	�g���q

		if( (ext == ".tga") || (ext == ".TGA") )
		{
			result = LoadTGA(fileName, mipLevel);
		}
		result = LoadFromFile(fileName, mipLevel);
	}
	return result;
}
