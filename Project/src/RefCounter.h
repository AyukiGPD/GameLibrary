//=============================================================================
//!
//!	@file	RefCounter.h
//!	@brief	�Q�ƃJ�E���^
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//-------------------------------------------------------------------
//!	�Q�ƃJ�E���^�N���X
//-------------------------------------------------------------------
class RefCounter
{
public:
	//!	�R���X�g���N�^
	RefCounter()
	: _refCount(1)
	{
	}

	//!	�f�X�g���N�^
	virtual ~RefCounter()
	{
//		assert(_refCount <= 0);
	}

	//!	�Q�Ɖ��Z
	//!	@return	���Z��̎Q�Ɛ�
	s32 AddRef()
	{
		return ++_refCount;
	}

	//!	���	�Q�Ƃ��Ȃ���Ύ��E
	//!	@return	�Q�Ɛ�
	s32 Release()
	{
		s32 refCount = --_refCount;
		if(refCount <= 0)
		{
			delete this;
		}
		return refCount;
	}

	//!	�Q�Ɛ��擾
	//!	@return	�Q�Ɛ�
	s32 GetCount()	const
	{
		return _refCount;
	}

private:
	s32 _refCount;	//!< �Q�Ɛ�
};

//-------------------------------------------------------------------
//!	����R�[���o�b�N
//-------------------------------------------------------------------
struct RefCountRelease
{
	void operator()(RefCounter* p) const
	{
		p->Release();
	}
};

//-------------------------------------------------------------------
//!	std::unique_ptr<T,RefCountRelease>��RefPointer<T>
//!	@tparam	T	�I�u�W�F�N�g�^
//-------------------------------------------------------------------
template<class T>
using RefPointer = std::unique_ptr < T, RefCountRelease >;


//-------------------------------------------------------------------
//!	�I�u�W�F�N�g�쐬
//!	@tparam	T		�쐬����Q�[���I�u�W�F�N�g
//!	@tparam	Args	�����^
//!	@param	[in]	args	����
//-------------------------------------------------------------------
template <typename T, typename... Args>
RefPointer<T> CreateRefPointer(Args&&... args)
{
	return RefPointer<T>(new T(std::forward<Args>(args)...));
}

//-------------------------------------------------------------------
//!	�I�u�W�F�N�g�쐬	(Init)
//!	@tparam	T		�쐬����Q�[���I�u�W�F�N�g
//!	@tparam	Args	�����^
//!	@param	[in]	args	����
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