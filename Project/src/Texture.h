//=============================================================================
//!
//!	@file	Texture.h
//!	@brief	�摜�`��N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//-------------------------------------------------------------------
//! �e�N�X�`��
//-------------------------------------------------------------------
class Texture
{
public:
	//	https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml
	//!	�e�N�X�`���t�H�[�}�b�g
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

	//!	�A�N�Z�X�^�C�v
	enum class ACCESS_TYPE
	{
		READ			= GL_READ_ONLY,
		WRITE			= GL_WRITE_ONLY,
		READ_WRITE		= GL_READ_WRITE
	};

	//!	�e�N�X�`���t�B���^
	enum class FILTER
	{
		NEAREST					= GL_NEAREST,
		LINEAR					= GL_LINEAR,
		NEAREST_MIPMAP_NEAREST	= GL_NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST	= GL_LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR	= GL_NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR	= GL_LINEAR_MIPMAP_LINEAR,
	};

	//!	�e�N�X�`���p�����[�^
	struct Param
	{
		//!	�R���X�g���N�^
		Param()
		: _format(FORMAT::UNKNOWN)
		, _minFilter(FILTER::LINEAR)
		, _magFilter(FILTER::LINEAR)
		, _mipLevel(0)
		{}

		//!	�R���X�g���N�^
		//!	@param	[in]	format		�t�H�[�}�b�g
		//!	@param	[in]	minFilter	�e�N�X�`���t�B���^	default:LINEAR
		//!	@param	[in]	magFilter	�e�N�X�`���t�B���^	default:LINEAR
		//!	@param	[in]	mipLevel	�~�b�v���x��		default:0
		Param(FORMAT format, FILTER minFilter = FILTER::LINEAR, FILTER magFilter = FILTER::LINEAR, u32 mipLevel = 0)
		: _format(format)
		, _minFilter(minFilter)
		, _magFilter(magFilter)
		, _mipLevel(mipLevel)
		{}

		//!	�R���X�g���N�^
		//!	@param	[in]	format		�t�H�[�}�b�g
		//!	@param	[in]	mipLevel	�~�b�v���x��
		Param(FORMAT format, u32 mipLevel)
		: _format(format)
		, _mipLevel(mipLevel)
		, _minFilter(mipLevel > 0 ? FILTER::LINEAR_MIPMAP_LINEAR : FILTER::LINEAR)
		, _magFilter(FILTER::LINEAR)
		{}

		FORMAT _format;		//!< �t�H�[�}�b�g
		FILTER _minFilter;	//!< �e�N�X�`���t�B���^
		FILTER _magFilter;	//!< �e�N�X�`���t�B���^
		u32 _mipLevel;		//!< �~�b�v���x��
	};


public:
	//! �R���X�g���N�^
	Texture()
	: _textureId(0)
	, _width(0)
	, _height(0)
	, _param(FORMAT::UNKNOWN, FILTER::LINEAR, FILTER::LINEAR, 0)
	{
	}

	//! �f�X�g���N�^
	virtual ~Texture()
	{
		Clear();
	}

	//!	���
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

	//! OpenGL�̃e�N�X�`��ID���擾
	//!	@return	�e�N�X�`��ID
	GLuint GetTextureID()const { return _textureId; }
	//! �����擾
	//!	@return	��
	s32 GetWidth()const { return _width; }
	//! �������擾
	//!	@return	����
	s32 GetHeight()const { return _height; }

	//!	�e�N�X�`���̗L��
	//!	@retval	false	�e�N�X�`������
	bool IsEmpty()const { return(_textureId == 0); }

	//!	�e�N�X�`���̗L����
	//!	@param	[in]	activeTexture	ActibeTexture�̔ԍ�	default:0
	void BindTexture(u32 activeTexture = 0);
	//!	�e�N�X�`���̖�����
	//!	@param	[in]	activeTexture	ActibeTexture�̔ԍ�	default:0
	static void UnBindTexture(u32 activeTexture = 0);
	//!	�e�N�X�`���̐ݒ�
	//!	@param	[in]	pTexture	�e�N�X�`���̃|�C���^(nullptr�w���OFF)
	//!	@param	[in]	activeTexture	ActibeTexture�̔ԍ�	default:0
	static void BindTexture(const Texture* pTexture, u32 activeTexture = 0);

	//	�����킩��Ȃ�����ꕔ�����͕ۗ�
	//!	�e�N�X�`���̐ݒ�
	//!	@param	[in]	pTexture	�e�N�X�`���̃|�C���^
	//!	@param	[in]	location	���P�[�V����
	//!	@param	[in]	access		�A�N�Z�X�^�C�v
	static void BindImageTexture(const Texture* pTexture, u32 location, ACCESS_TYPE access/*, s32 level = 0, bool layerd = false, s32 layer = 0*/);

	//! �t�H�[�}�b�g�̎擾
	Param GetParam() const
	{
		return _param;
	}
	//! �t�H�[�}�b�g�̎擾
	FORMAT GetFormat() const
	{
		return _param._format;
	}
	//! �t�H�[�}�b�g�̎擾
	u32 GetMipLevel() const
	{
		return _param._mipLevel;
	}
protected:
	//!	�e�N�X�`���̃t�H�[�}�b�g
	struct TextureFormatData
	{
		u32 _internalFormat;
		u32 _colorFormat;
		u32 _typeFormat;
	};
	//!	�e�N�X�`���t�H�[�}�b�g
	//!	@param	[in]	format	�t�H�[�}�b�g
	//!	@return	�t�H�[�}�b�g�f�[�^ ���s����GL_FALSE
	static TextureFormatData CreateTextureFormat(FORMAT format);
protected:
	GLuint	_textureId;	//!< �e�N�X�`��ID
	s32		_width;		//!< ��
	s32		_height;	//!< ����

	Param _param;

};

// �`��^�[�Q�b�g
class RenderTarget : public Texture
{
public:

	struct Desc
	{
		//!	�R���X�g���N�^
		Desc()
		: _width(0)
		, _height(0)
		, _param(FORMAT::UNKNOWN, FILTER::LINEAR, FILTER::LINEAR, 0)
		{}
		//!	�R���X�g���N�^
		//!	@param	[in]	width		��
		//!	@param	[in]	height		����
		//!	@param	[in]	param		�e�N�X�`���p�����[�^
		Desc(s32 width, s32 height, const Param& param)
		: _width(width)
		, _height(height)
		, _param(param)
		{}


		s32 _width;		//!< ��
		s32 _height;	//!< ����
		Param _param;	//!< �p�����[�^
	};



	//! �R���X�g���N�^
	RenderTarget()
	{
	}

	//! �f�X�g���N�^
	virtual ~RenderTarget()
	{
		Clear();
	}
	
	//! ���
	virtual void Clear()
	{
		Texture::Clear();
	}

	//!	�e�N�X�`������
	//!	@param	[in]	desc
	static RenderTarget* Create(const Desc& desc);

	//! �e�N�X�`���쐬
	//!	@param	[in]	width		��
	//!	@param	[in]	height		����
	//!	@param	[in]	param		�t�H�[�}�b�g
	bool Init(s32 width, s32 height, const Param& param);

private:
};

//	�摜�e�N�X�`��
class AssetTexture : public Texture
{
public:
	//! �R���X�g���N�^
	AssetTexture();

	//! �f�X�g���N�^
	virtual ~AssetTexture();

	//! �ǂݍ���
	//!	@param	[in]	fileName	�t�@�C����
	//!	@retval	false	���s
	bool Load(const std::string& fileName, u32 mipLevel = 0);

private:
	//! TGA�t�@�C���̓ǂݍ���
	//!	@param	[in]	fileName	�t�@�C����
	//!	@retval	false	���s
	bool LoadTGA(const std::string& fileName, u32 mipLevel);
	//!	�t�@�C���̓ǂݍ���
	//!	@param	[in]	fileName	�t�@�C����
	//!	@retval	false	���s
	bool LoadFromFile(const std::string& fileName, u32 mipLevel);
};

