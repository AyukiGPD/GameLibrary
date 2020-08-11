//=============================================================================
//!
//!	@file	Color.cpp
//!	@brief	�J���[�\����
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

const f32	Color::TO_FLOAT = 1.0f / 255;


const Color Color::WHITE	= Color( 255 );				//!< ��
const Color Color::GRAY		= Color( 128 );				//!< �D
const Color Color::BLACK	= Color(   0 );				//!< ��
const Color Color::RED		= Color( 255,   0,   0 );	//!< ��
const Color Color::GREEN	= Color(   0, 255,   0 );	//!< ��
const Color Color::BLUE		= Color(   0,   0, 255 );	//!< ��
const Color Color::YELLOW	= Color( 255, 255,   0 );	//!< ��
const Color Color::MAGENTA	= Color( 255,   0, 255 );	//!< �}�[���_
const Color Color::CYAN		= Color(   0, 255, 255 );	//!< �V�A��
const Color Color::ORANGE	= Color( 255, 165,   0 );	//!< ��
const Color Color::INDIGO	= Color(  35,  71, 148 );	//!< ��
const Color Color::PURPLE	= Color( 160,  32, 240 );	//!< ��

namespace Math
{

Color RandomColor()
{
	return Color(Random() % 256, Random() % 256, Random() % 256, Random() % 256);
}

}


