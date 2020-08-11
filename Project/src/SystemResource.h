//=============================================================================
//!
//!	@file	SystemResource.h
//!	@brief	���\�[�X�r���h�N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//	������

#if 0

class SystemResource
{
public:
	//!	�R���X�g���N�^
	SystemResource();
	//!	�f�X�g���N�^
	virtual ~SystemResource();

	//!	�f�[�^�r���h
	void DataBuild();
private:
	//	�r���h�p�v���C�x�[�g�֐�

	//!	�摜
	void ImageBuild();
	//!	���y
	void SoundBuild();
	//!	���f��
	void ModelBuild();
	//!	�t�H���g
	void FontBuild();
	//!	�V�F�[�_�[
	void ShaderBuild();
	//!	���̑�
	void OtherBuild();

	//!	�e�L�X�g
	void TextBuild();
};

#endif

