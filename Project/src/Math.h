//=============================================================================
//!
//!	@file	Math.h
//!	@brief	�v�Z�֘A
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

namespace Math
{
//----------------------------------------------------------------------------
//	�v�Z
//----------------------------------------------------------------------------
static const f32 PI  = 3.14159265f;	//	�~����
static const f32 PI2 = PI * 2.f;	//	�~����

//!	�����_���֐�
u32 Random();
//!	�����_���֐�
f32 Randomf();
//!	�����_���֐�
f32 Randomuf();


//!	xorshift����
u32 XoSRandom();
//!	xorshift����
f32 XoSRandomf();
//!	xorshift����
f32 XoSRandomuf();

//!	�����Z���k�E�c�C�X�^�[����
u32 MTRandom();
//!	�����Z���k�E�c�C�X�^�[����
f32 MTRandomf();
//!	�����Z���k�E�c�C�X�^�[����
f32 MTRandomuf();

//!	�����_���f�o�C�X����
u32 RDRandom();
//!	�����_���f�o�C�X����
f32 RDRandomf();
//!	�����_���f�o�C�X����
f32 RDRandomuf();



//!	�����_���V�[�h�֐�
//!	@param	[in]	�V�[�h�l
void RandSeed(u32 seed);


//	�p�x�ϊ�
//!	@param	[in]	degree	�p�x���l
//!	@return	���W�A���l
MY_FORCEINLINE
f32 ToRadian(f32 degree)
{
	static f32 radian = PI / 180.0f;
	return degree * radian;
}
//!	@param	[in]	radian	���W�A���l
//!	@retunr	�p�x���l
MY_FORCEINLINE
f32 ToDegree(f32 radian)
{
	static f32 degree = 180.0f / PI;
	return radian * degree;
}

//-----------------------------------------------------------------------------
//! @brief  ���`���
//! @param  [in]    a       ���lA
//! @param  [in]    b       ���lB
//! @param  [in]    scale   �X�P�[��
//! @retunr ���
//-----------------------------------------------------------------------------
MY_FORCEINLINE
f32 Lerp(f32 a, f32 b, f32 scale)
{
	return (a + (b - a) * scale);
}


}	//	namespace Math
