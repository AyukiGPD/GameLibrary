//=============================================================================
//!
//!	@file	Color.cpp
//!	@brief	カラー構造体
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

const f32	Color::TO_FLOAT = 1.0f / 255;


const Color Color::WHITE	= Color( 255 );				//!< 白
const Color Color::GRAY		= Color( 128 );				//!< 灰
const Color Color::BLACK	= Color(   0 );				//!< 黒
const Color Color::RED		= Color( 255,   0,   0 );	//!< 赤
const Color Color::GREEN	= Color(   0, 255,   0 );	//!< 緑
const Color Color::BLUE		= Color(   0,   0, 255 );	//!< 青
const Color Color::YELLOW	= Color( 255, 255,   0 );	//!< 黄
const Color Color::MAGENTA	= Color( 255,   0, 255 );	//!< マゼンダ
const Color Color::CYAN		= Color(   0, 255, 255 );	//!< シアン
const Color Color::ORANGE	= Color( 255, 165,   0 );	//!< 橙
const Color Color::INDIGO	= Color(  35,  71, 148 );	//!< 藍
const Color Color::PURPLE	= Color( 160,  32, 240 );	//!< 紫

namespace Math
{

Color RandomColor()
{
	return Color(Random() % 256, Random() % 256, Random() % 256, Random() % 256);
}

}


