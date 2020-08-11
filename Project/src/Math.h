//=============================================================================
//!
//!	@file	Math.h
//!	@brief	計算関連
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

namespace Math
{
//----------------------------------------------------------------------------
//	計算
//----------------------------------------------------------------------------
static const f32 PI  = 3.14159265f;	//	円周率
static const f32 PI2 = PI * 2.f;	//	円周率

//!	ランダム関数
u32 Random();
//!	ランダム関数
f32 Randomf();
//!	ランダム関数
f32 Randomuf();


//!	xorshift乱数
u32 XoSRandom();
//!	xorshift乱数
f32 XoSRandomf();
//!	xorshift乱数
f32 XoSRandomuf();

//!	メルセンヌ・ツイスター乱数
u32 MTRandom();
//!	メルセンヌ・ツイスター乱数
f32 MTRandomf();
//!	メルセンヌ・ツイスター乱数
f32 MTRandomuf();

//!	ランダムデバイス乱数
u32 RDRandom();
//!	ランダムデバイス乱数
f32 RDRandomf();
//!	ランダムデバイス乱数
f32 RDRandomuf();



//!	ランダムシード関数
//!	@param	[in]	シード値
void RandSeed(u32 seed);


//	角度変換
//!	@param	[in]	degree	角度数値
//!	@return	ラジアン値
MY_FORCEINLINE
f32 ToRadian(f32 degree)
{
	static f32 radian = PI / 180.0f;
	return degree * radian;
}
//!	@param	[in]	radian	ラジアン値
//!	@retunr	角度数値
MY_FORCEINLINE
f32 ToDegree(f32 radian)
{
	static f32 degree = 180.0f / PI;
	return radian * degree;
}

//-----------------------------------------------------------------------------
//! @brief  線形補間
//! @param  [in]    a       数値A
//! @param  [in]    b       数値B
//! @param  [in]    scale   スケール
//! @retunr 補間
//-----------------------------------------------------------------------------
MY_FORCEINLINE
f32 Lerp(f32 a, f32 b, f32 scale)
{
	return (a + (b - a) * scale);
}


}	//	namespace Math
