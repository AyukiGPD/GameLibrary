//=============================================================================
//!
//!	@file	typeDef.h
//!	@brief	タイプデファイン
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

typedef   signed char		s8;			//!< 符号あり  8bit整数
typedef unsigned char		u8;			//!< 符号なし  8bit整数
typedef   signed short		s16;		//!< 符号あり 16bit整数
typedef unsigned short		u16;		//!< 符号なし 16bit整数
typedef   signed int		s32;		//!< 符号あり 32bit整数
typedef unsigned int		u32;		//!< 符号なし 32bit整数
typedef   signed long		s32L;		//!< 符号あり 32bit整数
typedef unsigned long		u32L;		//!< 符号なし 32bit整数
typedef   signed long long	s64;		//!< 符号あり 64bit整数
typedef unsigned long long	u64;		//!< 符号なし 64bit整数

typedef	float				f32;		//!< 単精度浮動小数点数
typedef	double				f64;		//!< 倍精度浮動小数点数



typedef			char		U_CHAR;		//!< 文字
typedef	const	char		U_CCHAR;	//!< 文字(const)
typedef			char*		U_STR;		//!< 文字列
typedef	const	char*		U_CSTR;		//!< 文字列(const)

typedef			wchar_t		WU_CHAR;	//!< 文字
typedef	const	wchar_t		WU_CCHAR;	//!< 文字(const)
typedef			wchar_t*	WU_STR;		//!< 文字列
typedef	const	wchar_t*	WU_CSTR;	//!< 文字列(const)


//!	サイズ(幅,高さ)
struct Size
{
public:
	//!	コンストラクタ
	Size()
	: _w(0)
	, _h(0)
	{};
	//!	コンストラクタ
	Size(u32 width, u32 height)
	: _w(width)
	, _h(height)
	{};

public:
	u32 _w;		//!< 幅
	u32 _h;		//!< 高さ
};

//!	トリガー,リリース機能付きbool	試作
struct SmartBool
{
	SmartBool() :_param(0){};
	SmartBool(bool val) :_param(0){ _param = val; };
	~SmartBool(){};

	//!	呼び出し用
	enum MODE
	{
		COMMON = 1 << 0,
		TRIGGER = 1 << 1,
		RELEASE = 1 << 2,
	};

	//!	呼び出し
	bool operator()(SmartBool::MODE mode) const
	{
		u32 val = (_param&mode);
		return (val != 0);
	}

	//!	キャスト
	//	呼び出し時は通常のbool値
	operator bool() const
	{
		return ((_param&COMMON) != 0);
	}

	//!	キャスト
	//	呼び出し時は通常のbool値
	bool operator ! () const
	{
		return ((_param&COMMON) == 0);
	}

	//!	代入
	//	代入のときにトリガーとリリースを作る
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
	u8 _param;		//!	通常bool | トリガー | リリース 計3bit
	//	u8 _param : 3;	//!	通常bool | トリガー | リリース 計3bit
};




