//=============================================================================
//!
//!	@file	Singleton.h
//!	@brief	�V���O���g���N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//=============================================================================
//!	�p���V���O���g���N���X
//!	@tparam	T	�I�u�W�F�N�g�^
//=============================================================================
template <typename T>
class SingletonInherit
{
public:
	//! �R���X�g���N�^
	SingletonInherit()
	{
		assert(_pInstance == nullptr);
		size_t offset = (size_t)(T*)1 - (size_t)(SingletonInherit<T>*)(T*)1;
		_pInstance = reinterpret_cast<T*>((size_t)this + offset);
	}

	//! �f�X�g���N�^
	virtual ~SingletonInherit()
	{
		assert(_pInstance != nullptr);
		_pInstance = nullptr;
	}

	//! �C���X�^���X�擾
	static T*	GetInstance() { return _pInstance;  }

private:
	static T*	_pInstance;		//!< �C���X�^���X
};

//!	�C���X�^���X
template <typename T>
T*  SingletonInherit<T>::_pInstance = nullptr;



//=============================================================================
//!	�V���O���g������p�N���X
//=============================================================================
class SingletonFinalizer
{
public:
	//!	����֐��^
	typedef void(*FinalizerFunc)();

	//!	����֐��ǉ�
	//!	@param	[in]	�ǉ��֐�
	static void AddFinalizer(FinalizerFunc func);
	//!	����֐��Ăяo��
	static void Finalize();
};

//=============================================================================
//!	mozc���V���O���g��
//!	@tparam	T	�I�u�W�F�N�g�^
//=============================================================================
template <typename T>
class Singleton final
{
public:
	//!	�C���X�^���X�擾
	static T* GetInstance()
	{
		std::call_once(_initFlag, Create);
		assert(_pInstance);
		return _pInstance;
	}

private:
	//!	�C���X�^���X�쐬
	static void Create()
	{
		_pInstance = new T;
		SingletonFinalizer::AddFinalizer(&Singleton<T>::Destroy);
	}

	//!	�C���X�^���X�폜
	static void Destroy()
	{
		SafeDelete(_pInstance);
	}

	static std::once_flag _initFlag;	//!< �������֐��t���O
	static T* _pInstance;				//!< �C���X�^���X
};

template <typename T>
std::once_flag Singleton<T>::_initFlag;

template <typename T>
T* Singleton<T>::_pInstance = nullptr;




//	�V���O���g���擾�֐�����
#define MY_INTERFACE(CLASS_, FUNCTION_NAME_)	\
MY_FORCEINLINE CLASS_*		FUNCTION_NAME_()	\
{												\
	return Singleton<CLASS_>::GetInstance();	\
}												\



