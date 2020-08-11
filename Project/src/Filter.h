//=============================================================================
//!
//!	@file	Filter.h
//!	@brief	�t�B���^�[
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//=============================================================================
//!	�e�N�X�`���G�t�F�N�g���N���X
//=============================================================================
class FilterBase
{
public:
	FilterBase()
	{}
	virtual ~FilterBase(){}
	//!	���
	virtual void Clear()
	{
	}
	//!	������
	//!	@param	[in]	width	��
	//!	@param	[in]	height	����
	//!	@param	[in]	param	�e�N�X�`���p�����[�^
	//!	@retval	true	����
	//!	@retval	false	���s
	virtual bool Init(u32 width, u32 height, const Texture::Param& param) = 0;

	//!	�e�N�X�`���`��
	//!	@param	[in]	texture	�e�N�X�`��
	virtual void Render(const Texture* pTexture)
	{
		UNUSED(pTexture);
	}

};


//static const u32 BLUR_PASS_SIZE = 2;

//=============================================================================
//!	�u���[�G�t�F�N�g�N���X
//=============================================================================
class BlurFilter : public FilterBase
{
public:
	//!	�R���X�g���N�^
	BlurFilter();
	//!	�f�X�g���N�^
	virtual ~BlurFilter();
	//!	���
	void Clear();
	//!	������
	//!	@param	[in]	width	��
	//!	@param	[in]	height	����
	//!	@param	[in]	param	�e�N�X�`���p�����[�^
	//!	@retval	true	����
	//!	@retval	false	���s
	bool Init(u32 width, u32 height, const Texture::Param& param);

	//!	�e�N�X�`���`��
	//!	@param	[in]	texture	�e�N�X�`��
	void Render(const Texture* pTexture);


	//!	�e�N�X�`���擾
	//!	@param	[in]	index	�e�N�X�`���ԍ�	default:1
	//!	@return	�e�N�X�`���̃|�C���^ ���s����nullptr;
	Texture* GetTexture()const
	{
		if(!_pBlur){ return nullptr; }
		return _pBlur->GetTexture(0);
	}


private:
	//!	�V�F�[�_�[�ǂݍ��ݓ����֐�
	//!	@retval	true	����
	//!	@retval	false	���s
	bool ShaderLoad();
private:
	FrameBuffer* _pBlur;
	ShaderHundler _shader;
};

//=============================================================================
//!	�G�~�b�V�u�J���[�␳�t�B���^�[
//=============================================================================
class EmissiveColorFilter : public FilterBase
{
public:
	//!	�R���X�g���N�^
	EmissiveColorFilter();
	//!	�f�X�g���N�^
	virtual ~EmissiveColorFilter();

	//!	������
	//!	@param	[in]	width	��
	//!	@param	[in]	height	����
	//!	@param	[in]	param	�e�N�X�`���p�����[�^
	//!	@retval	true	����
	//!	@retval	false	���s
	bool Init(u32 width, u32 height, const Texture::Param& param);

	//!	�e�N�X�`���`��
	//!	@param	[in]	pTexEmissive	Emissive�e�N�X�`��
	//!	@param	[in]	pTexDiffuse		Diffuse�e�N�X�`��
	void Render(const Texture* pTexEmissive,const Texture* pTexDiffuse);

	//!	�e�N�X�`���擾
	//!	@param	[in]	index	�e�N�X�`���ԍ�	default:1
	//!	@return	�e�N�X�`���̃|�C���^ ���s����nullptr;
	Texture* GetTexture()const
	{
		if(!_pEmissive){ return nullptr; }
		return _pEmissive->GetTexture(0);
	}
private:
	//!	�V�F�[�_�[�ǂݍ��ݓ����֐�
	//!	@retval	true	����
	//!	@retval	false	���s
	bool ShaderLoad();

private:
	FrameBuffer* _pEmissive;
	ShaderHundler _shader;
};


//=============================================================================
//!	�g�[���}�b�s���O�N���X
//=============================================================================
class ToneMappingFilter : public FilterBase
{
public:
	//!	�R���X�g���N�^
	ToneMappingFilter();
	//!	�f�X�g���N�^
	virtual ~ToneMappingFilter();
	//!	���
	void Clear();
	//!	������
	//!	@param	[in]	width	��
	//!	@param	[in]	height	����
	//!	@param	[in]	param	�e�N�X�`���p�����[�^
	//!	@retval	true	����
	//!	@retval	false	���s
	bool Init(u32 width, u32 height, const Texture::Param& param);

	//!	�e�N�X�`���`��
	//!	@param	[in]	texture	�e�N�X�`��
	void Render(const Texture* pTexture);


	//!	�e�N�X�`���擾
	//!	@param	[in]	index	�e�N�X�`���ԍ�	default:0
	//!	@return	�e�N�X�`���̃|�C���^ ���s����nullptr;
	Texture* GetTexture()const
	{
		if(!_pTonemapping){ return nullptr; }
		return _pTonemapping->GetTexture(0);
	}

	//!	�e�N�X�`���擾
	//!	@param	[in]	index	�e�N�X�`���ԍ�	default:0
	//!	@return	�e�N�X�`���̃|�C���^ ���s����nullptr;
	Texture* GetMinTexture(u32 index)const
	{
		if(index >= _pMinBuffer.size()){ return nullptr; }
		if (!_pMinBuffer[index]){ return nullptr; }
		return _pMinBuffer[index]->GetTexture(0);
	}

	//!	�ő�P�x�ݒ�
	void SetExposureMax(f32 exposureBiasMax)
	{
		_exposureBiasMax = exposureBiasMax;
	}
	//!	�ŏ��P�x�ݒ�
	void SetExposureMin(f32 exposureBiasMin)
	{
		_exposureBiasMin = exposureBiasMin;
	}

	//!	�ő�P�x�擾
	f32 GetExposureMax()const
	{
		return _exposureBiasMax;
	}
	//!	�ŏ��P�x�擾
	f32 GetExposureMin()const
	{
		return _exposureBiasMin;
	}


	//!	���ϋP�x
	f32 GetExposure()const
	{
		return _exposureBias;
	}

	//!	�P�x�擾�p�k���o�b�t�@
	size_t GetBufferSize()const
	{
		return _pMinBuffer.size();
	}

private:
	//!	�V�F�[�_�[�ǂݍ��ݓ����֐�
	//!	@retval	true	����
	//!	@retval	false	���s
	bool ShaderLoad();

private:

	FrameBuffer*				_pTonemapping;	//!< �g�[���}�b�s���OFB
	std::vector<FrameBuffer*>	_pMinBuffer;	//!< �P�x�擾�p�k���o�b�t�@

	ShaderHundler _shaderLuminance;			//!< �P�x�擾�V�F�[�_�[
	ShaderHundler _shaderToneMapping;		//!< �g�[���}�b�s���O�V�F�[�_�[

	f32 _exposureBias;			//!< ���ϋP�x
	f32 _exposureOffset;		//!< �P�x����

	f32 _exposureBiasMax;
	f32 _exposureBiasMin;

	u32 _minBufferCount;				//!< �o�b�t�@�J�E���g
};


//=============================================================================
//!	�u���[���t�B���^�[
//=============================================================================
class BloomFilter : public FilterBase
{
public:
	//!	�R���X�g���N�^
	BloomFilter();
	//!	�f�X�g���N�^
	virtual ~BloomFilter();

	//!	���
	void Clear();

	//!	������
	//!	@param	[in]	width	��
	//!	@param	[in]	height	����
	//!	@param	[in]	param	�e�N�X�`���p�����[�^
	//!	@retval	true	����
	//!	@retval	false	���s
	bool Init(u32 width, u32 height, const Texture::Param& param);

	//!	�e�N�X�`���`��
	//!	@param	[in]	texture	�e�N�X�`��
	void Render(const Texture* pTexture);

	//!	�P�x�擾
	//!	@return	�P�x�e�N�X�`��
	Texture* GetBrightness()const
	{
		return _pBrightness->GetTexture(0);
	}
	
	//!	�u���[���o�b�t�@�擾
	//!	@param	[in]	index	�o�b�t�@�C���f�b�N�X
	//!	@return	�e�N�X�`��
	Texture* GetTexture(u32 index)const
	{
		if(index >= BLOOM_BUFFER_SIZE){ return nullptr; }
		if(!_pBlooms[index]){ return nullptr; }
		return _pBlooms[index]->GetTexture();
	}

	static const u32 BLOOM_BUFFER_SIZE = 7;	//!< �u���[���o�b�t�@�T�C�Y

private:
	FrameBuffer*	_pBrightness;				//!< �P�x
	std::array<BlurFilter*, BLOOM_BUFFER_SIZE>		_pBlooms;	//!< �u���[�t�B���^�[
	ShaderHundler	_shader;					//!< �V�F�[�_�[�ԍ�
};



