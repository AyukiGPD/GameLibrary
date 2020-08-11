//=============================================================================
//!
//!	@file	Macro.h
//!	@brief	�}�N��
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

#define OFFSET_OF(STRUCT_, VALUE_) (size_t)( &((STRUCT_*)0)->VALUE_ )


//! ���g�p�ϐ��̌x���𖾎��I�ɗ}��
#define UNUSED(VALUE_)	(void)VALUE_

//! ���b�Z�[�W�t���A�T�[�V����
#define MY_ASSERT(CONDITION_, MESSAGE_)		assert(CONDITION_ && MESSAGE_)


//-----------------------------------------------------------------------------
// ���������[�N���m
//-----------------------------------------------------------------------------
#if _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
	#define   new					::new			(_NORMAL_BLOCK, __FILE__, __LINE__)
	#define   malloc(s)             _malloc_dbg		(s, _NORMAL_BLOCK, __FILE__, __LINE__)
	#define   calloc(c, s)          _calloc_dbg		(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
	#define   realloc(p, s)         _realloc_dbg	(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
	#define   _recalloc(p, c, s)    _recalloc_dbg	(p, c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
	#define   _expand(p, s)         _expand_dbg		(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif // #if _DEBUG


//	�f�o�b�O���O
#ifdef _DEBUG
#define OUTPUT_DEBUG_STRING(MESSAGE_)	{ OutputDebugStringA((LPCSTR)  MESSAGE_); }
#define OUTPUT_DEBUG_STRING_W(MESSAGE_)	{ OutputDebugStringW((LPCWSTR) MESSAGE_); }
#else
#define OUTPUT_DEBUG_STRING(MESSAGE_)	{  }
#define OUTPUT_DEBUG_STRING_W(MESSAGE_)	{  }
#endif

//	�G���[���O
#ifdef _DEBUG
#define ERROR_LOG(MESSAGE_)									\
{															\
	std::stringstream sstr;									\
	sstr << MESSAGE_ << ":"<< __FUNCTION__ <<__LINE__;		\
	std::cout << sstr.str() << std::endl;					\
	MessageBoxA(nullptr,sstr.str().c_str(),nullptr,MB_OK);	\
	OUTPUT_DEBUG_STRING(sstr.str().c_str());				\
	assert(false);											\
}
#define ERROR_LOG_W(MESSAGE_)								\
{															\
	std::wstringstream wsstr;								\
	wsstr << MESSAGE_ << L":"<< __FUNCTION__ <<__LINE__;	\
	std::wcout << wsstr.str() << std::endl;					\
	MessageBox(nullptr,wsstr.str().c_str(),nullptr,MB_OK);	\
	OUTPUT_DEBUG_STRING_W(wsstr.str().c_str());				\
	assert(false);											\
}

#else
	#ifndef RELEASE_FINAL
		#define ERROR_LOG(MESSAGE_)									\
		{															\
			std::stringstream sstr;									\
			sstr << MESSAGE_ << ":"<< __FUNCTION__ <<__LINE__;		\
			MessageBoxA(nullptr,sstr.str().c_str(),nullptr,MB_OK);	\
		}
		#define ERROR_LOG_W(MESSAGE_)								\
		{															\
			std::wstringstream wsstr;								\
			wsstr << MESSAGE_ << L":"<< __FUNCTION__ <<__LINE__;	\
			MessageBox(nullptr,wsstr.str().c_str(),nullptr,MB_OK);	\
		}
	#else
		#define ERROR_LOG(MESSAGE_){}
		#define ERROR_LOG_W(MESSAGE_){}
	#endif	//	RELEASE_FINAL
#endif	//	_DEBUG

#ifdef _DEBUG
#define CONSOLE_LOG(MESSAGE_)		{ std::cout<< MESSAGE_ << std::endl; }
#define CONSOLE_LOG_W(MESSAGE_)		{ std::wcout<< MESSAGE_ << std::endl; }
#else
#define CONSOLE_LOG(MESSAGE_)		{  }
#define CONSOLE_LOG_W(MESSAGE_)		{  }
#endif

//	�f�o�b�O���O(�R���\�[��)
#ifdef _DEBUG
#define DEBUG_LOG(MESSAGE_)		{ std::cout<< MESSAGE_ << ":"<< __FUNCTION__ <<__LINE__ << std::endl; }
#define DEBUG_LOG_W(MESSAGE_)	{ std::wcout<< MESSAGE_ << L":"<< __FUNCTION__ <<__LINE__ << std::endl; }
#else
#define DEBUG_LOG(MESSAGE_)	{  }
#define DEBUG_LOG_W(MESSAGE_)	{  }
#endif

//	�֐��G���[�I���}�N��
#ifdef _DEBUG
#define ERROR_RETURN(MESSAGE_,VALUE_)	{ DEBUG_LOG(MESSAGE_);	return VALUE_; }
//#define ERROR_RETURN(MESSAGE_,VALUE_)	{ ERROR_LOG(MESSAGE_);	return VALUE_; }
//#define ERROR_RETURN(MESSAGE_,VALUE_)	{ return VALUE_; }
#else
//#define ERROR_RETURN(MESSAGE_,VALUE_)	{ DEBUG_LOG(MESSAGE_);	return VALUE_; }
//#define ERROR_RETURN(MESSAGE_,VALUE_)	{ ERROR_LOG(MESSAGE_);	return VALUE_; }
#define ERROR_RETURN(MESSAGE_,VALUE_)	{ return VALUE_; }
#endif

//!	������쐬�}�N��
#define CREATE_STRING(STRING_DATA_, STRING_)	\
{												\
	std::stringstream sstr;						\
	sstr << STRING_DATA_;						\
	STRING_ = sstr.str();						\
}

//!	�}���`�o�C�g������쐬�}�N��
#define CREATE_WSTRING(STRING_DATA_, WSTRING_)	\
{												\
	std::wstringstream wsstr;					\
	wsstr << STRING_DATA_;						\
	WSTRING_ = wsstr.str();						\
}

//-----------------------------------------------------------------------------
//!	GL�G���[���m�}�N��
//-----------------------------------------------------------------------------
#ifndef RELEASE_FINAL
#define isErrorGL()										\
{														\
	s32 code = glGetError();							\
	switch(code)										\
	{													\
	case GL_NO_ERROR:									\
		break;											\
	case GL_INVALID_VALUE:								\
		ERROR_LOG(L"GL Error : GL_INVALID_VALUE");		\
		break;											\
	case GL_INVALID_ENUM:								\
		ERROR_LOG(L"GL Error : GL_INVALID_ENUM");		\
		break;											\
	case GL_INVALID_OPERATION:							\
		ERROR_LOG(L"GL Error : GL_INVALID_OPERATION");	\
		break;											\
	case GL_STACK_OVERFLOW:								\
		ERROR_LOG(L"GL Error : GL_STACK_OVERFLOW");		\
		break;											\
	case GL_STACK_UNDERFLOW:							\
		ERROR_LOG(L"GL Error : GL_STACK_UNDERFLOW");	\
		break;											\
	case GL_OUT_OF_MEMORY:								\
		ERROR_LOG(L"GL Error : GL_OUT_OF_MEMORY");		\
		break;											\
	}													\
}
#else
#define isErrorGL(){}
#endif

//!	�C�����C��
#define MY_INLINE inline
//#define MY_INLINE __forceinline

//!	�����C�����C��
//#define MY_FORCEINLINE inline
#define MY_FORCEINLINE __forceinline


