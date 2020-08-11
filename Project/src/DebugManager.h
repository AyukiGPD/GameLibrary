//=============================================================================
//!
//!	@file	DebugRender.h
//!	@brief	�f�o�b�O�`��
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

class DebugManager : public ManagerBase
{
public:
	//!	�R���X�g���N�^
	//!	@param	[in]	render	�`��t���O
	DebugManager();
	//!	�f�X�g���N�^
	virtual ~DebugManager();

	//!	������
	void Init();
	//!	�X�V
	void Update();
	//!	�`��
	void Render();

	//!	�t���[�����[�g
	FrameRate* GetFrameRateObj(){ return _pFrameRate; }
	//!	GPU�g�p��
	GpuUsage*  GetGpuUsageObj()	{ return _pGpuUsage; }
	//!	CPU�g�p��
	CpuUsage*  GetCpuUsageObj()	{ return _pCpuUsage; }

	//!	�f�o�b�O�`��
	//!	@param	[in]	index	�ԍ�
	//!	@param	[in]	str		������
	//!	@param	[in]	color	�F
	void SetDebugString(u32 index, const std::wstring& str,const Color& Color = Color::WHITE);

	//!	�f�o�b�O��~
	bool IsUpdateActive()const
	{
		return _updateActive || _nextFrame;
	}

	//!	�f�o�b�O�`��t���O�ݒ�
	//!	@param	[in]	active	�t���O
	void SetDebugRender(bool active)
	{
		_renderActive = active;
	}

	//!	�`�敶��
	//!	@param	[in]	hundler	�����f�[�^
	void SetDebugFontHundler(const FontHundler& hundler);

	//!	�f�o�b�O�����`��
	//!	@param	[in]	str	������
	//!	@param	[in]	x	X���W
	//!	@param	[in]	y	Y���W
	void RenderDebugString(const std::wstring& str, f32 x, f32 y);
	//!	�f�o�b�O�����`��
	//!	@param	[in]	str	������
	//!	@param	[in]	pos	���W
	void RenderDebugString(const std::wstring& str, const Vector2& pos);

private:
	FrameRate* _pFrameRate;	//!< �t���[�����[�g�v���N���X
	GpuUsage* _pGpuUsage;	//!< GPU�g�p���v���N���X
	CpuUsage* _pCpuUsage;	//!< CPU�g�p���v���N���X
	bool _renderActive;		//!< �\���t���O
	bool _updateActive;		//!< �\���t���O
	bool _nextFrame;		//!< �\���t���O
	std::array<std::wstring, 55> _debugWString;
	std::array<Color, 55> _debugStringColor;

	FontHundler _debugFont;	//!< �f�o�b�O�����f�[�^

};

MANAGER_INTERFACE(DebugManager, Debug);

