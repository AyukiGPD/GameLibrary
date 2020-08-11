//=============================================================================
//!
//!	@file	Singleton.cpp
//!	@brief	シングルトンクラス
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"


static const u32 gMaxFinalizersSize = 256;	//!	シングルトン上限
static u32 gNumFinalizersSize = 0;			//!	シングルトン数
static SingletonFinalizer::FinalizerFunc gFinalizers[gMaxFinalizersSize];	//!	シングルトン解放関数配列

//-----------------------------------------------------------------------------
//!	解放関数追加
//!	@param	[in]	追加関数
//-----------------------------------------------------------------------------
void SingletonFinalizer::AddFinalizer(FinalizerFunc func)
{
	assert(gNumFinalizersSize < gMaxFinalizersSize);
	gFinalizers[gNumFinalizersSize++] = func;
}

//-----------------------------------------------------------------------------
//!	解放関数呼び出し
//-----------------------------------------------------------------------------
void SingletonFinalizer::Finalize()
{
	for(int i = gNumFinalizersSize - 1; i >= 0; --i)
	{
		(*gFinalizers[i])();
	}
	gNumFinalizersSize = 0;
}

