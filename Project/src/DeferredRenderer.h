//=============================================================================
//!
//!	@file	DeferredRenderer.h
//!	@brief	�f�B�t�@�[�h�����_�����O
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once
//static const u32 BLOOM_BUFFER_SIZE = 4;

class BaseDeferredRenderer
{
public:
	//!	�R���X�g���N�^
	BaseDeferredRenderer()
	: _pGbuffer(nullptr)
	, _pLighting(nullptr)
	, _pFinalize(nullptr)
	, _pBufferTexture(nullptr)
	{};
	//!	�f�X�g���N�^
	virtual ~BaseDeferredRenderer(){ Clear(); };
public:
	//!	������
	//!	@param	[in]	width	��
	//!	@param	[in]	height	����
	//!	@retval	true	����
	//!	@retval	false	���s
	virtual bool Init(u32 width, u32 height) = 0;
	//!	���
	virtual void Clear()
	{
		SafeDelete(_pGbuffer);
		SafeDelete(_pLighting);
		SafeDelete(_pFinalize);
	};

	//!	�X�V
	virtual void Update(){};

	//! G�o�b�t�@�`��J�n
	virtual void BeginGBuffer() = 0;

	//! G�o�b�t�@�`��I��
	virtual void EndGBuffer() = 0;

	//! ���C�e�B���O
	virtual void Lighting(const Matrix& matProj, const Matrix& matView) = 0;

	//! �ŏI����
	virtual void Finalize() = 0;

	//!	�쐬�����o�b�t�@�̕\��
	void RenderTexture();

	//!	�`�悷��e�N�X�`��
	//!	@return	�`�悷��e�N�X�`���̃|�C���^
	Texture* GetBufferTexture()const
	{
		return _pBufferTexture;
	}

protected:
	std::vector<ShaderHundler> _shaderPass;		//!< �V�F�[�_�[

	FrameBuffer*	_pGbuffer;		//!< G�o�b�t�@FBO
	FrameBuffer*	_pLighting;		//!< ���C�e�B���OFBO
	FrameBuffer*	_pFinalize; 

	Texture* _pBufferTexture;

private:

};

//=============================================================================
//!	�f�B�t�@�[�h�����_�����O
//=============================================================================
class PhysicalBasedRendering : public BaseDeferredRenderer
{
public:

	//!	G�o�b�t�@�̎��
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
			| ���[���h���WXYZ | rough |
			+-----+-----+-----+-------+
			| ���[���h�@��XYZ | metal |
			+-----+-----+-----+-------+
			|  R  |  G  |  B  |   A   |
			+-----+-----+-----+-------+
		*/

		//----
		GBUFFER_COUNT_MAX,	// �ő��
	};

	//! ���C�e�B���O
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

	//!	�p�X�̎��
	enum RENDER_PASS
	{
		GBUFFER,
//		GBUFFER_ARRAY,
		LIGHTING,
		MAX,
	};


	//	�`�惂�[�h
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
	//!	�R���X�g���N�^
	PhysicalBasedRendering();
	//!	�f�X�g���N�^
	virtual ~PhysicalBasedRendering();

	//!	������
	//!	@param	[in]	width	��
	//!	@param	[in]	height	����
	//!	@retval	true	����
	//!	@retval	false	���s
	bool Init(u32 width, u32 height);
	//!	���
	void Clear();

	void Update();

	//! G�o�b�t�@�`��J�n
	void BeginGBuffer();

	//! G�o�b�t�@�`��I��
	void EndGBuffer();

	//! ���C�e�B���O
	void Lighting(const Matrix& matProj, const Matrix& matView);

	//! �ŏI����
	void Finalize();

	//	�擾�֐�

	//!	�e�N�X�`��
	//!	@param	[in]	type	�o�b�t�@�̎��
	//!	@return	G�o�b�t�@�̃|�C���^�[
	RenderTarget* GetGbufferTexture(GBUFFER_TYPE type)
	{
		if (type == GBUFFER_COUNT_MAX){ return nullptr; }
		return _pGbuffer->GetTexture(type);
	}
	//!	�e�N�X�`��
	//!	@param	[in]	type	�o�b�t�@�̎��
	//!	@return	���C�g�e�N�X�`��
	RenderTarget* GetLightTexture(LIGHT_TYPE type)
	{
		if (type == LIGHT_COUNT_MAX){ return nullptr; }
		return _pLighting->GetTexture(type);
	}

	//!	�u���[���t�B���^�[�擾
	//!	@return	�t�B���^�[�̃|�C���^
	BloomFilter* GetBloomFilter()
	{
		return _pBloom;
	}

	//!	�g�[���}�b�s���O�t�B���^�[�擾
	//!	@return	�t�B���^�[�̃|�C���^
	ToneMappingFilter* GeToneMappingFilter()
	{
		return _pToneMapping;
	}

private:
	//!	G�o�b�t�@����
	//!	@param	[in]	width	��
	//!	@param	[in]	height	�c
	//!	@retval	true	����
	//!	@retval	false	���s
	bool CreateGBuffer(u32 width, u32 height);
	//!	���C�g�o�b�t�@����
	//!	@param	[in]	width	��
	//!	@param	[in]	height	�c
	//!	@retval	true	����
	//!	@retval	false	���s
	bool CreateLighting(u32 width, u32 height);
	//!	�ŏI�����o�b�t�@����
	//!	@param	[in]	width	��
	//!	@param	[in]	height	�c
	//!	@retval	true	����
	//!	@retval	false	���s
	bool CreateFinalize(u32 width, u32 height);

private:
	BloomFilter*	_pBloom;	//!< �u���[��
	EmissiveColorFilter*	_pEmissive;	//!< �u���[��

	ToneMappingFilter* _pToneMapping;	//!< �g�[���}�b�s���O

	RENDER_MODE _renderMode;		//!< �`�惂�[�h	

	s32 _index;				//!< �f�o�b�O�`��p�o�b�t�@�C���f�b�N�X
	u32 _lightCount;		//!< ���C�g�̃J�E���^

	Timer _timeGbuffer;		//!< ���Ԍv��
	Timer _timeLightng;		//!< ���Ԍv��
	Timer _timeFinalize;	//!< ���Ԍv��

	s32 _luminaceCount;
};





