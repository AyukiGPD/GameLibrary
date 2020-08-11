//=============================================================================
//!
//!	@file	System.h
//!	@brief	�V�X�e���֘A
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//	��ʃT�C�Y
#ifdef _DEBUG
//static const s32 SCRN_WIDTH = 1920;	//!< ��
//static const s32 SCRN_HEIGHT = 1080;	//!< ����

static const s32 SCRN_WIDTH = 1600;		//!< ��
static const s32 SCRN_HEIGHT = 900;		//!< ����

//static const s32 SCRN_WIDTH = 960;		//!< ��
//static const s32 SCRN_HEIGHT = 540;		//!< ����

//static const s32 SCRN_WIDTH  = 1920;	//!< ��
//static const s32 SCRN_HEIGHT = 1080;	//!< ����

#else
//static const s32 SCRN_WIDTH  = 1920;	//!< ��
//static const s32 SCRN_HEIGHT = 1080;	//!< ����

static const s32 SCRN_WIDTH = 1920;	//!< ��
static const s32 SCRN_HEIGHT = 1080;	//!< ����

//static const s32 SCRN_WIDTH = 1600;		//!< ��
//static const s32 SCRN_HEIGHT = 900;		//!< ����

//static const s32 SCRN_WIDTH = 1440;		//!< ��
//static const s32 SCRN_HEIGHT = 900;		//!< ����
#endif

static const s32 SCRN_WIDTH_TWICE  = SCRN_WIDTH  * 2;	//!< ��
static const s32 SCRN_HEIGHT_TWICE = SCRN_HEIGHT * 2;	//!< ����

static const s32 SCRN_WIDTH_HALF  = SCRN_WIDTH  * 0.5f;	//!< ��
static const s32 SCRN_HEIGHT_HALF = SCRN_HEIGHT * 0.5f;	//!< ����

static const f32 SCRN_ASPECT = static_cast<f32>(SCRN_WIDTH) / static_cast<f32>(SCRN_HEIGHT);	//!< �A�X�y�N�g��

//-----------------------------------------------------------------------------
//!	�C���f�b�N�X�v�Z
//!	@param	[in]	x			�z��ԍ�X
//!	@param	[in]	y			�z��ԍ�Y
//!	@param	[in]	paramMaxX	�z��T�C�YX
//!	@return	�v�Z�����z��ԍ�
//-----------------------------------------------------------------------------
MY_FORCEINLINE
u32 IndexCall(u32 x, u32 y, u32 paramMaxX)
{
	//	array[y][x]
	return y * paramMaxX + x;
}
//-----------------------------------------------------------------------------
//!	�C���f�b�N�X�v�Z
//!	@param	[in]	x			�z��ԍ�X
//!	@param	[in]	y			�z��ԍ�Y
//!	@param	[in]	z			�z��ԍ�Z
//!	@param	[in]	paramMaxX	�z��T�C�YX
//!	@param	[in]	paramMaxY	�z��T�C�YY
//!	@return	�v�Z�����z��ԍ�
//-----------------------------------------------------------------------------
MY_FORCEINLINE
u32 IndexCall(u32 x, u32 y, u32 z, u32 paramMaxX, u32 paramMaxY)
{
	//	array[z][y][x]
	return (paramMaxX * paramMaxY * z) + (paramMaxX * y) + x;
}

//----------------------------------------------------------------------------
//! @brief  ���S�ȉ�� (Release)
//! @param  [inout] p    �|�C���^
//! @retval �Ȃ�
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
//! @brief  ���S�ȉ�� (Delete)
//! @param  [inout] p    �|�C���^
//! @retval �Ȃ�
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
//! @brief  ���S�ȉ�� (�z��)(Delete)
//! @param  [inout] p    �|�C���^
//! @retval �Ȃ�
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
//! @brief  ���S�ȉ�� (vector�|�C���^�z��)(Delete)
//! @param  [inout] p    �|�C���^
//! @retval �Ȃ�
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
//! @brief  ���S�ȉ�� (vector�|�C���^�z��)(Delete)
//! @param  [inout] p    �|�C���^
//! @retval �Ȃ�
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



//	GLUT�x���폜�p�R�[�h
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


