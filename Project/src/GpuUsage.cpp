//=============================================================================
//!
//!	@file	GpuUsage.cpp
//!	@brief	GPU使用率計測クラス
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

#define NVAPI_MAX_PHYSICAL_GPUS   64	//	GpuHandles
#define NVAPI_MAX_USAGES_PER_GPU  34	//	gpuUsages

//	関数ポインタ型
typedef int *(*NvAPI_QueryInterface_t)(unsigned int offset);
typedef int(*NvAPI_Initialize_t)();
typedef int(*NvAPI_EnumPhysicalGPUs_t)(int **handles, int *count);
typedef int(*NvAPI_GPU_GetUsages_t)(int *handle, unsigned int *usages);

//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
GpuUsage::GpuUsage()
: _gpuMode(GPU_MODE::GPU_NONE)
, _usage(-1)
, _gpuCount(0)
, _pGpuHandles(nullptr)
, _gpuUsages(nullptr)
, FP_NvAPI_GPU_GetUsages(nullptr)
{
	std::string nvapiName;
#ifdef X64
	nvapiName = "nvapi64.dll";
#else
	nvapiName = "nvapi.dll";
#endif // WIN32


	//	NVIDIA
	_hmod = LoadLibraryA(nvapiName.c_str());
	if (_hmod != nullptr)
	{
		NvAPI_QueryInterface_t      FpNvAPI_QueryInterface = nullptr;	//	内部機能取得用関数
		NvAPI_Initialize_t          FpNvAPI_Initialize = nullptr;		//	ライブラリ初期化関数
		NvAPI_EnumPhysicalGPUs_t    FpNvAPI_EnumPhysicalGPUs = nullptr;

		FpNvAPI_QueryInterface = (NvAPI_QueryInterface_t)GetProcAddress(_hmod, "nvapi_QueryInterface");
		if (FpNvAPI_QueryInterface == nullptr)
		{
			ERROR_LOG("Couldn't get functions in " + nvapiName);
			return;
		}

		//	内部関数の取得
		FpNvAPI_Initialize = (NvAPI_Initialize_t)(*FpNvAPI_QueryInterface)(0x0150E828);
		FpNvAPI_EnumPhysicalGPUs = (NvAPI_EnumPhysicalGPUs_t)(*FpNvAPI_QueryInterface)(0xE5AC921F);
		FP_NvAPI_GPU_GetUsages = (NvAPI_GPU_GetUsages_t)(*FpNvAPI_QueryInterface)(0x189A1FDF);

		if (FpNvAPI_Initialize == nullptr || FpNvAPI_EnumPhysicalGPUs == nullptr ||
			FP_NvAPI_GPU_GetUsages == nullptr)
		{
			ERROR_LOG("Couldn't get functions in " + nvapiName);
			return;
		}

		//	ライブラリ初期化
		(*FpNvAPI_Initialize)();


		_pGpuHandles = new s32*[NVAPI_MAX_PHYSICAL_GPUS];
		_gpuUsages = new u32[NVAPI_MAX_USAGES_PER_GPU];

		//	gpuUsages[0]以外の場合FP_NvAPI_GPU_GetUsagesが動作しない
		_gpuUsages[0] = (NVAPI_MAX_USAGES_PER_GPU * 4) | 0x10000;

		(*FpNvAPI_EnumPhysicalGPUs)(_pGpuHandles, &_gpuCount);

		_gpuMode = GPU_MODE::GPU_NVIDIA;
	}
	else
	{
		ERROR_LOG("Couldn't find " + nvapiName);
	}
}


//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
GpuUsage::~GpuUsage()
{
	SafeDeleteArray(_pGpuHandles);
	SafeDeleteArray(_gpuUsages);
}

//-----------------------------------------------------------------------------
//!	更新
//-----------------------------------------------------------------------------
void GpuUsage::Update()
{
	if (_gpuMode != GPU_MODE::GPU_NVIDIA){ return; }

	(*(NvAPI_GPU_GetUsages_t)FP_NvAPI_GPU_GetUsages)(_pGpuHandles[0], _gpuUsages);
	_usage = (s32)_gpuUsages[3];
}

//-----------------------------------------------------------------------------
//!	描画
//-----------------------------------------------------------------------------
void GpuUsage::Render()
{
	// TODO 表示の簡略バージョンも
	SetColor(0,0,255);
	std::wstringstream wsstr;
	if (_gpuMode == GPU_MODE::GPU_NVIDIA)
	{
		wsstr << "GPU:" << _usage << "%";
		f32 percent = (f32)_usage * 0.01f;
		Render2D::Rect(0, (f32)SCRN_HEIGHT - 8, SCRN_WIDTH * percent, 4, Color(0, 0, 255));
	}
	else
	{
		wsstr << "GPU:" << "NotSupport";
	}
	Manager::Debug()->RenderDebugString(wsstr.str(), Vector2(0, SCRN_HEIGHT - 40));
}


