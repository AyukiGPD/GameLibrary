//=============================================================================
//!
//!	@file	SceneManager.h
//!	@brief	�V�[���}�l�[�W���[
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//-------------------------------------------------------------------
//!	�V�[���Ǘ��N���X
//-------------------------------------------------------------------
class SceneManager : public ManagerBase
{
public:
	//!	�R���X�g���N�^
	SceneManager();
	//!	�f�X�g���N�^
	virtual ~SceneManager();

	//!	������
	void Init();
	//!	�X�V
	void Update();
	//!	�`��
	void Render();
	//!	�I��
	void Exit();

	//!	�V�[���؂�ւ��֐�
	//!	@tparam	T	���̃V�[���N���X
	template<typename T>
	void SceneJump()
	{
		SafeDelete(_pNextScene);		//	�O�̂��߂ɒ��g��nullptr������������
		_pNextScene = new T();			//	�V�����V�[�����쐬
	}

	//!	���݂̃V�[�����擾
	//!	@return	�V�[���I�u�W�F�N�g�̃|�C���^
	 SceneBase* GetNowScene()
	{
		return _pScene;
	}

private:
	SceneBase*	_pScene;		//!< ���C���œ����V�[���N���X
	SceneBase*	_pNextScene;	//!< ���؂�ւ��V�[��

};

MANAGER_INTERFACE(SceneManager, Scene);


