//=============================================================================
//!
//!	@file	SceneGame.h
//!	@brief	�Q�[���V�[��
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//-------------------------------------------------------------------
//!	�Q�[���V�[���N���X
//-------------------------------------------------------------------
class SceneGame :public SceneBase
{
public:
	//!	�R���X�g���N�^
	SceneGame();
	//!	�f�X�g���N�^
	virtual ~SceneGame();

	//!	������
	void Init();
	//!	�X�V
	void Update();
	//!	�`��
	void Render();
	//!	�I��
	void Exit();

private:
	RefPointer<CameraObject> _pCamera;			//!< �J�����I�u�W�F�N�g
	RefPointer<LightObject> _pDirectionalLight;	//!< ���s�����I�u�W�F�N�g

	std::array< RefPointer<SoundObject>,3 >	_pBgmSounds;	//!< BGM�I�u�W�F�N�g

	FontHundler _font;		//!< �t�H���g�f�[�^
	bool _isPause;	//!< �ꎞ��~�t���O
};


