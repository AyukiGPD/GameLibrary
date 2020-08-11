//=============================================================================
//!
//!	@file	GpuUsage.cpp
//!	@brief	GPU�g�p���v���N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//-------------------------------------------------------------------
//!	GPU�g�p���v���N���X
//!	���ݑΉ���NVIDIA�̂�
//-------------------------------------------------------------------
class GpuUsage
{
public:
	//!	�R���X�g���N�^
	GpuUsage();
	//!	�f�X�g���N�^
	virtual ~GpuUsage();
	
	//!	�g�p���擾�֐�
	//!	@return	�g�p��
	//!	@retval	-1	�擾���s
	s32  GetUsage()const{ return _usage; }

	//!	�X�V
	void Update();
	//!	�`��
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

	//	�֐��|�C���^
	void* FP_NvAPI_GPU_GetUsages;

	s32		_gpuCount;
	s32**	_pGpuHandles;
	u32*	_gpuUsages;

	s32 _usage;
};

