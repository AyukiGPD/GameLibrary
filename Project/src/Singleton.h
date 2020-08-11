//=============================================================================
//!
//!	@file	Singleton.h
//!	@brief	シングルトンクラス
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//=============================================================================
//!	継承シングルトンクラス
//!	@tparam	T	オブジェクト型
//=============================================================================
template <typename T>
class SingletonInherit
{
public:
	//! コンストラクタ
	SingletonInherit()
	{
		assert(_pInstance == nullptr);
		size_t offset = (size_t)(T*)1 - (size_t)(SingletonInherit<T>*)(T*)1;
		_pInstance = reinterpret_cast<T*>((size_t)this + offset);
	}

	//! デストラクタ
	virtual ~SingletonInherit()
	{
		assert(_pInstance != nullptr);
		_pInstance = nullptr;
	}

	//! インスタンス取得
	static T*	GetInstance() { return _pInstance;  }

private:
	static T*	_pInstance;		//!< インスタンス
};

//!	インスタンス
template <typename T>
T*  SingletonInherit<T>::_pInstance = nullptr;



//=============================================================================
//!	シングルトン解放用クラス
//=============================================================================
class SingletonFinalizer
{
public:
	//!	解放関数型
	typedef void(*FinalizerFunc)();

	//!	解放関数追加
	//!	@param	[in]	追加関数
	static void AddFinalizer(FinalizerFunc func);
	//!	解放関数呼び出し
	static void Finalize();
};

//=============================================================================
//!	mozc式シングルトン
//!	@tparam	T	オブジェクト型
//=============================================================================
template <typename T>
class Singleton final
{
public:
	//!	インスタンス取得
	static T* GetInstance()
	{
		std::call_once(_initFlag, Create);
		assert(_pInstance);
		return _pInstance;
	}

private:
	//!	インスタンス作成
	static void Create()
	{
		_pInstance = new T;
		SingletonFinalizer::AddFinalizer(&Singleton<T>::Destroy);
	}

	//!	インスタンス削除
	static void Destroy()
	{
		SafeDelete(_pInstance);
	}

	static std::once_flag _initFlag;	//!< 初期化関数フラグ
	static T* _pInstance;				//!< インスタンス
};

template <typename T>
std::once_flag Singleton<T>::_initFlag;

template <typename T>
T* Singleton<T>::_pInstance = nullptr;




//	シングルトン取得関数生成
#define MY_INTERFACE(CLASS_, FUNCTION_NAME_)	\
MY_FORCEINLINE CLASS_*		FUNCTION_NAME_()	\
{												\
	return Singleton<CLASS_>::GetInstance();	\
}												\



