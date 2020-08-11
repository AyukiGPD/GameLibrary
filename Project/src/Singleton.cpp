//=============================================================================
//!
//!	@file	Singleton.cpp
//!	@brief	�V���O���g���N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"


static const u32 gMaxFinalizersSize = 256;	//!	�V���O���g�����
static u32 gNumFinalizersSize = 0;			//!	�V���O���g����
static SingletonFinalizer::FinalizerFunc gFinalizers[gMaxFinalizersSize];	//!	�V���O���g������֐��z��

//-----------------------------------------------------------------------------
//!	����֐��ǉ�
//!	@param	[in]	�ǉ��֐�
//-----------------------------------------------------------------------------
void SingletonFinalizer::AddFinalizer(FinalizerFunc func)
{
	assert(gNumFinalizersSize < gMaxFinalizersSize);
	gFinalizers[gNumFinalizersSize++] = func;
}

//-----------------------------------------------------------------------------
//!	����֐��Ăяo��
//-----------------------------------------------------------------------------
void SingletonFinalizer::Finalize()
{
	for(int i = gNumFinalizersSize - 1; i >= 0; --i)
	{
		(*gFinalizers[i])();
	}
	gNumFinalizersSize = 0;
}

