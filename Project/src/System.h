//=============================================================================
//!
//!	@file	System.h
//!	@brief	システム関連
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//	画面サイズ
#ifdef _DEBUG
//static const s32 SCRN_WIDTH = 1920;	//!< 幅
//static const s32 SCRN_HEIGHT = 1080;	//!< 高さ

static const s32 SCRN_WIDTH = 1600;		//!< 幅
static const s32 SCRN_HEIGHT = 900;		//!< 高さ

//static const s32 SCRN_WIDTH = 960;		//!< 幅
//static const s32 SCRN_HEIGHT = 540;		//!< 高さ

//static const s32 SCRN_WIDTH  = 1920;	//!< 幅
//static const s32 SCRN_HEIGHT = 1080;	//!< 高さ

#else
//static const s32 SCRN_WIDTH  = 1920;	//!< 幅
//static const s32 SCRN_HEIGHT = 1080;	//!< 高さ

static const s32 SCRN_WIDTH = 1920;	//!< 幅
static const s32 SCRN_HEIGHT = 1080;	//!< 高さ

//static const s32 SCRN_WIDTH = 1600;		//!< 幅
//static const s32 SCRN_HEIGHT = 900;		//!< 高さ

//static const s32 SCRN_WIDTH = 1440;		//!< 幅
//static const s32 SCRN_HEIGHT = 900;		//!< 高さ
#endif

static const s32 SCRN_WIDTH_TWICE  = SCRN_WIDTH  * 2;	//!< 幅
static const s32 SCRN_HEIGHT_TWICE = SCRN_HEIGHT * 2;	//!< 高さ

static const s32 SCRN_WIDTH_HALF  = SCRN_WIDTH  * 0.5f;	//!< 幅
static const s32 SCRN_HEIGHT_HALF = SCRN_HEIGHT * 0.5f;	//!< 高さ

static const f32 SCRN_ASPECT = static_cast<f32>(SCRN_WIDTH) / static_cast<f32>(SCRN_HEIGHT);	//!< アスペクト比

//-----------------------------------------------------------------------------
//!	インデックス計算
//!	@param	[in]	x			配列番号X
//!	@param	[in]	y			配列番号Y
//!	@param	[in]	paramMaxX	配列サイズX
//!	@return	計算した配列番号
//-----------------------------------------------------------------------------
MY_FORCEINLINE
u32 IndexCall(u32 x, u32 y, u32 paramMaxX)
{
	//	array[y][x]
	return y * paramMaxX + x;
}
//-----------------------------------------------------------------------------
//!	インデックス計算
//!	@param	[in]	x			配列番号X
//!	@param	[in]	y			配列番号Y
//!	@param	[in]	z			配列番号Z
//!	@param	[in]	paramMaxX	配列サイズX
//!	@param	[in]	paramMaxY	配列サイズY
//!	@return	計算した配列番号
//-----------------------------------------------------------------------------
MY_FORCEINLINE
u32 IndexCall(u32 x, u32 y, u32 z, u32 paramMaxX, u32 paramMaxY)
{
	//	array[z][y][x]
	return (paramMaxX * paramMaxY * z) + (paramMaxX * y) + x;
}

//----------------------------------------------------------------------------
//! @brief  安全な解放 (Release)
//! @param  [inout] p    ポインタ
//! @retval なし
//----------------------------------------------------------------------------
template<typename T>
MY_FORCEINLINE
s32 SafeRelease(T*& p)
{
	if(p)
	{
		s32 count = p->Release();
		p = nullptr;

		return count;
	}
	return 0;
}

//----------------------------------------------------------------------------
//! @brief  安全な解放 (Delete)
//! @param  [inout] p    ポインタ
//! @retval なし
//----------------------------------------------------------------------------
template<typename T>
MY_FORCEINLINE
void SafeDelete(T*& p)
{
	if(p)
	{
		delete p;
		p = nullptr;
	}
}

//----------------------------------------------------------------------------
//! @brief  安全な解放 (配列)(Delete)
//! @param  [inout] p    ポインタ
//! @retval なし
//----------------------------------------------------------------------------
template<typename T>
MY_FORCEINLINE
void SafeDeleteArray(T*& pArr)
{
	if (pArr)
	{
		delete[] pArr;
		pArr = nullptr;
	}
}

//----------------------------------------------------------------------------
//! @brief  安全な解放 (vectorポインタ配列)(Delete)
//! @param  [inout] p    ポインタ
//! @retval なし
//----------------------------------------------------------------------------
template<typename T>
MY_FORCEINLINE
void SafeDeleteArray(std::vector<T*>& pArr)
{
	for (auto* p : pArr)
	{
		SafeDelete(p);
	}
	pArr.clear();
}

//----------------------------------------------------------------------------
//! @brief  安全な解放 (vectorポインタ配列)(Delete)
//! @param  [inout] p    ポインタ
//! @retval なし
//----------------------------------------------------------------------------
template<typename T>
MY_FORCEINLINE
void SafeDeleteArray(std::list<T*>& pArr)
{
	for (auto* p : pArr)
	{
		SafeDelete(p);
	}
	pArr.clear();
}


inline
f32 Lerp(f32 a, f32 b, f32 scale)
{
	return (a + (b - a) * scale);
}



//	GLUT警告削除用コード
namespace freeglut_avoid_warning
{
	class _dummy
	{
		_dummy()
		{
			glutInit(nullptr, nullptr);
			glutCreateWindow(nullptr);
			glutCreateMenu(nullptr);
		}
	};
}


