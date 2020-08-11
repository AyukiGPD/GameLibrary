//=============================================================================
//!
//!	@file	Math.cpp
//!	@brief	�v�Z�֘A
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

namespace Math
{

//!	�V�[�h�l
static u32 randomSeed[4] = { 123456789, 362436069, 521288629, 88675123 };

//!	�V�[�h�ݒ�
void RandSeed(u32 seed)
{
	for (u32 i = 0; i < 4; ++i)
	{
		randomSeed[i] = seed = 1812433253U * (seed ^ (seed >> 30)) + i;
	}
}

static const f32 longint = 1.0f / 2147483647.0f;
static const f32 ulongint = 1.0f / 4294967295.0f;


//!	�����_���֐�
u32 Random()
{
	return XoSRandom();
//	return MTRandom();
//	return RDRandom();
}

//!	�����_���֐�
f32 Randomf()
{
	s32L rand = Random() - 2147483647;
	return (f32)rand * longint;
}

//!	�����_���֐�
f32 Randomuf()
{
	u32L rand = Random();
	return (f32)rand * ulongint;
}

//!	xorshift����
u32 XoSRandom()
{
	u32 *a = randomSeed;
	u32  t = (a[0] ^ (a[0] << 11));
	a[0] = a[1]; a[1] = a[2]; a[2] = a[3];
	return (a[3] = (a[3] ^ (a[3] >> 19)) ^ (t ^ (t >> 8)));
}

//!	xorshift����
f32  XoSRandomf()
{
	s32 rand = XoSRandom() - 2147483647;
	return (f32)rand * longint;
}

//!	xorshift����
f32  XoSRandomuf()
{
	u32 rand = XoSRandom();
	return (f32)rand * ulongint;
}

//!	�����Z���k�E�c�C�X�^�[����
u32 MTRandom()
{
	static std::random_device rd;
	static std::mt19937 mt(rd());
	return mt();
}

//!	�����Z���k�E�c�C�X�^�[����
f32 MTRandomf()
{
	s32L rand = MTRandom() - 2147483647;
	return (f32)rand * longint;
}

//!	�����Z���k�E�c�C�X�^�[����
f32 MTRandomuf()
{
	u32L rand = MTRandom();
	return (f32)rand * ulongint;
}

//!	�����_���f�o�C�X����
u32 RDRandom()
{
	std::random_device rd;
	return rd();
}

//!	�����_���f�o�C�X����
f32 RDRandomf()
{
	s32L rand = RDRandom() - 2147483647;
	return (f32)rand * longint;
}

//!	�����_���f�o�C�X����
f32 RDRandomuf()
{
	u32L rand = RDRandom();
	return (f32)rand * ulongint;
}


}	//	namesapce Math
