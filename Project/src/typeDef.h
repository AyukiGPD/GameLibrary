//=============================================================================
//!
//!	@file	typeDef.h
//!	@brief	�^�C�v�f�t�@�C��
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

typedef   signed char		s8;			//!< ��������  8bit����
typedef unsigned char		u8;			//!< �����Ȃ�  8bit����
typedef   signed short		s16;		//!< �������� 16bit����
typedef unsigned short		u16;		//!< �����Ȃ� 16bit����
typedef   signed int		s32;		//!< �������� 32bit����
typedef unsigned int		u32;		//!< �����Ȃ� 32bit����
typedef   signed long		s32L;		//!< �������� 32bit����
typedef unsigned long		u32L;		//!< �����Ȃ� 32bit����
typedef   signed long long	s64;		//!< �������� 64bit����
typedef unsigned long long	u64;		//!< �����Ȃ� 64bit����

typedef	float				f32;		//!< �P���x���������_��
typedef	double				f64;		//!< �{���x���������_��



typedef			char		U_CHAR;		//!< ����
typedef	const	char		U_CCHAR;	//!< ����(const)
typedef			char*		U_STR;		//!< ������
typedef	const	char*		U_CSTR;		//!< ������(const)

typedef			wchar_t		WU_CHAR;	//!< ����
typedef	const	wchar_t		WU_CCHAR;	//!< ����(const)
typedef			wchar_t*	WU_STR;		//!< ������
typedef	const	wchar_t*	WU_CSTR;	//!< ������(const)


//!	�T�C�Y(��,����)
struct Size
{
public:
	//!	�R���X�g���N�^
	Size()
	: _w(0)
	, _h(0)
	{};
	//!	�R���X�g���N�^
	Size(u32 width, u32 height)
	: _w(width)
	, _h(height)
	{};

public:
	u32 _w;		//!< ��
	u32 _h;		//!< ����
};

//!	�g���K�[,�����[�X�@�\�t��bool	����
struct SmartBool
{
	SmartBool() :_param(0){};
	SmartBool(bool val) :_param(0){ _param = val; };
	~SmartBool(){};

	//!	�Ăяo���p
	enum MODE
	{
		COMMON = 1 << 0,
		TRIGGER = 1 << 1,
		RELEASE = 1 << 2,
	};

	//!	�Ăяo��
	bool operator()(SmartBool::MODE mode) const
	{
		u32 val = (_param&mode);
		return (val != 0);
	}

	//!	�L���X�g
	//	�Ăяo�����͒ʏ��bool�l
	operator bool() const
	{
		return ((_param&COMMON) != 0);
	}

	//!	�L���X�g
	//	�Ăяo�����͒ʏ��bool�l
	bool operator ! () const
	{
		return ((_param&COMMON) == 0);
	}

	//!	���
	//	����̂Ƃ��Ƀg���K�[�ƃ����[�X�����
	SmartBool operator = (bool v)
	{
		u8 param = 0;

		if (v){ param |= COMMON; }

		u8 now = param&COMMON;
		u8 old = _param&COMMON;
		if ((now ^ old) & now){ param |= TRIGGER; }
		if ((now ^ old) & old){ param |= RELEASE; }

		_param = param;

		return *this;
	}
private:
	u8 _param;		//!	�ʏ�bool | �g���K�[ | �����[�X �v3bit
	//	u8 _param : 3;	//!	�ʏ�bool | �g���K�[ | �����[�X �v3bit
};




