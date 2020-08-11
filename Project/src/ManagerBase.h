//=============================================================================
//!
//!	@file	ManagerBase.h
//!	@brief	�Ǘ��N���X���
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

class ManagerBase
{
public:
	//!	�R���X�g���N�^
	ManagerBase(){};
	//!	�f�X�g���N�^
	virtual ~ManagerBase(){};

	//!	������
	virtual void Init(){}
	//!	�X�V
	virtual void Update(){}
	//!	�`��
	virtual void Render(){}
	//!	�I��
	virtual void Exit(){}

private:

};


//!	�V���O���g���擾�֐�����
#define MANAGER_INTERFACE(CLASS_, FUNCTION_NAME_)	\
namespace Manager									\
{													\
MY_FORCEINLINE CLASS_*		FUNCTION_NAME_()		\
{													\
	return Singleton<CLASS_>::GetInstance();		\
}													\
}													\

//!	�}�l�[�W���[�Ăяo���}�N��
#define CALL_MANAGER(MANAGER_FUNC_,POINER_)		\
auto* POINER_ = MANAGER_FUNC_;					\
MY_ASSERT(POINER_ != nullptr,"Manager Empty")	\


