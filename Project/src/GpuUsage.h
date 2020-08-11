//=============================================================================
//!
//!	@file	GpuUsage.cpp
//!	@brief	GPU使用率計測クラス
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//-------------------------------------------------------------------
//!	GPU使用率計測クラス
//!	現在対応はNVIDIAのみ
//-------------------------------------------------------------------
class GpuUsage
{
public:
	//!	コンストラクタ
	GpuUsage();
	//!	デストラクタ
	virtual ~GpuUsage();
	
	//!	使用率取得関数
	//!	@return	使用率
	//!	@retval	-1	取得失敗
	s32  GetUsage()const{ return _usage; }

	//!	更新
	void Update();
	//!	描画
	void Render();

private:
	enum class GPU_MODE
	{
		GPU_NONE,
		GPU_NVIDIA,
		GPU_AMD
	};

	HMODULE _hmod;
	GPU_MODE _gpuMode;

	//	関数ポインタ
	void* FP_NvAPI_GPU_GetUsages;

	s32		_gpuCount;
	s32**	_pGpuHandles;
	u32*	_gpuUsages;

	s32 _usage;
};

