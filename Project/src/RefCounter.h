//=============================================================================
//!
//!	@file	RefCounter.h
//!	@brief	参照カウンタ
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//-------------------------------------------------------------------
//!	参照カウンタクラス
//-------------------------------------------------------------------
class RefCounter
{
public:
	//!	コンストラクタ
	RefCounter()
	: _refCount(1)
	{
	}

	//!	デストラクタ
	virtual ~RefCounter()
	{
//		assert(_refCount <= 0);
	}

	//!	参照加算
	//!	@return	加算後の参照数
	s32 AddRef()
	{
		return ++_refCount;
	}

	//!	解放	参照がなければ自殺
	//!	@return	参照数
	s32 Release()
	{
		s32 refCount = --_refCount;
		if(refCount <= 0)
		{
			delete this;
		}
		return refCount;
	}

	//!	参照数取得
	//!	@return	参照数
	s32 GetCount()	const
	{
		return _refCount;
	}

private:
	s32 _refCount;	//!< 参照数
};

//-------------------------------------------------------------------
//!	解放コールバック
//-------------------------------------------------------------------
struct RefCountRelease
{
	void operator()(RefCounter* p) const
	{
		p->Release();
	}
};

//-------------------------------------------------------------------
//!	std::unique_ptr<T,RefCountRelease>→RefPointer<T>
//!	@tparam	T	オブジェクト型
//-------------------------------------------------------------------
template<class T>
using RefPointer = std::unique_ptr < T, RefCountRelease >;


//-------------------------------------------------------------------
//!	オブジェクト作成
//!	@tparam	T		作成するゲームオブジェクト
//!	@tparam	Args	引数型
//!	@param	[in]	args	引数
//-------------------------------------------------------------------
template <typename T, typename... Args>
RefPointer<T> CreateRefPointer(Args&&... args)
{
	return RefPointer<T>(new T(std::forward<Args>(args)...));
}

//-------------------------------------------------------------------
//!	オブジェクト作成	(Init)
//!	@tparam	T		作成するゲームオブジェクト
//!	@tparam	Args	引数型
//!	@param	[in]	args	引数
//-------------------------------------------------------------------
template <typename T, typename... Args>
RefPointer<T> CreateRefPointerInit(Args&&... args)
{
	RefPointer<T> ptr(new T);
	ptr->Init(std::forward<Args>(args)...);
	return ptr;
}



#if 0

namespace
{
	template<typename T>
	class _pointer : public RefCounter
	{
	public:
		_pointer()
			:_pInstance(nullptr)
		{
		}
		_pointer(T* pInstance)
			:_pInstance(pInstance)
		{
		}
		virtual ~_pointer()
		{
			Delete();
		}

		void Delete()
		{
			SafeDelete(_pInstance);
		}

		bool Empty() const
		{
			return _pInstance == nullptr;
		}

		operator bool()const
		{
			return !Empty();
		}

	private:
		T* _pInstance;
	};
};



template<typename T>
class ObjectPointer final
{
public:
	ObjectPointer()
	{
	}
	virtual ~ObjectPointer()
	{
	}

	void Delete()
	{
		if (_pInstance)
		{
			_pInstance.Delete();
		}
	}

	ObjectPointer& operator = (const ObjectPointer& object)
	{
		_pInstance.Release();
		_pInstance = object._pInstance;
		_pInstance.AddRef();
	}

	bool Empty() const
	{
		return _pInstance.Empty();
	}
private:

	ObjectPointer(T* pInstance)
		:_pInstance(pInstance)
	{

	}

	template<typename T> friend ObjectPointer<T> CreateObject();

	_pointer<T> _pInstance;
};

template<typename T>
ObjectPointer<T> CreateObject()
{
	return ObjectPointer<T>(new T());
}


int main()
{
	::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ObjectPointer<s32> data = CreateObject<s32>();

	ObjectPointer<s32> data2 = data;

	data2.Delete();
	return 0;
}


#endif