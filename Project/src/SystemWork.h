//=============================================================================
//!
//!	@file	WorkSpace.h
//!	@brief	���[�N�X�y�[�X
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

// TODO �ʃt�@�C���Ɉړ�
// TODO �{����
//!	�}�l�[�W���[�N���X�n�̐����p�N���X
class ManagerBundle
{
public:
	//!	�R���X�g���N�^
	ManagerBundle();
	//!	�f�X�g���N�^
	virtual ~ManagerBundle();

	//!	�}�l�[�W���[�ǉ�
	//!	@tparam	T	�}�l�[�W���[�^
	template<typename T>
	static void AddManager()
	{
		_pManagers.push_back(Singleton<T>::GetInstance());
	}

	//void Update();
	//void Render();

	//!	�I��
	void Exit();

private:
	static std::list<ManagerBase*> _pManagers;
};

//!	�}�l�[�W���[�ǉ�
//!	@tparam	T	�}�l�[�W���[�^
template<typename T>
void AddManager()
{
	ManagerBundle::AddManager<T>();
}

//!	���C���N���X
class SystemWork
{
public:
	//!	�R���X�g���N�^
	SystemWork(){}
	//!	�f�X�g���N�^
	virtual ~SystemWork(){}

	//!	������
	bool Init();
	//!	�X�V
	void Update();
	//!	�`��
	void Render();
	//!	�I��
	void Exit();

private:
	ManagerBundle* _pManager;
};


