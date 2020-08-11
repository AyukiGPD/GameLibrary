//=============================================================================
//!
//!	@file	FrameRate.cpp
//!	@brief	�t���[���v���N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

const s32 FRAME_COUNT = 30;

//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
FrameRate::FrameRate()
: _nowTime(timeGetTime())	//	���Ԃ̎擾
, _count(0)
, _frameRate(0.0f)
, _frame(nullptr)
, _deltaTime(0)
{
	_frame = new s32[FRAME_COUNT];	//	�t���[�����̔z��̊m��
	assert(_frame != nullptr);

	for(s32 i=0; i<FRAME_COUNT; ++i)
	{
		_frame[i] = 0;
	}
	_oldTime = _nowTime;

}
//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
FrameRate::~FrameRate()
{
	SafeDeleteArray(_frame);
}

//-----------------------------------------------------------------------------
//!	�X�V
//-----------------------------------------------------------------------------
void FrameRate::Update()
{

	s32 nowCount = _count % FRAME_COUNT;

	_nowTime         = timeGetTime();		//	���Ԏ擾

	s32 offset		 = _nowTime - _oldTime;	//	�������i�[
	_frame[nowCount] = offset;				//	������ۑ����Ă���

	_deltaTime		 = (f32)offset * 0.001f;//	�b�P�ʂɕϊ�
	_oldTime         = _nowTime;			//	�O�̎��Ԃ�ۑ�

	f32 average = 0;
	//	�t���[�������������
	if( nowCount == (FRAME_COUNT-1) )
	{
		//	�P�t���[���ɂ����������Ԃ����ׂđ���
		for(s32 i=0; i<FRAME_COUNT; ++i) 
		{
			average += _frame[i];
		}

		if( average != 0 ) 
		{
			//	���ς��o��
			average /= FRAME_COUNT;
		}
	}

	_count = (_count + 1) % FRAME_COUNT;	//	�J�E���g



	//u32 time = (1000.0f / (f32)FRAME_COUNT);
	//_sleepTime = time - (_deltaTime * 1000.0f);
	//Sleep((_sleepTime <= 0 ? 0 : _sleepTime));

	if( average == 0 ) return;

	_frameRate = (1000.0f/average);		//	�t���[�����[�g
	

}

//-----------------------------------------------------------------------------
//!	�`��
//-----------------------------------------------------------------------------
void FrameRate::Render()
{
	SetColor(255);

	f32 total = 0;
	for(s32 i=0; i<FRAME_COUNT; ++i)
	{
		total += (f32)_frame[i];
	}
	SetColor(255);

	const f32 div = 1000.0f;
	const f32 percent = (total / div) * 0.5f;

	Render2D::Rect(0, (f32)SCRN_HEIGHT, SCRN_WIDTH, SCRN_HEIGHT - 2, Color(0));

	f32 frame = ((total / FRAME_COUNT) / div ) * div;	//	�P�t���[���̕��ώ���
	
	std::wstringstream wsstr;
	wsstr	<< "1Frame:" << (s32)frame 
			<< " FPS:" << std::fixed << _frameRate 
			<< " DeltaTime:" << std::fixed << _deltaTime;
//			<< " SleepTime" << _sleepTime;
	if (percent > 0.55)
	{
		// ��
		SetColor(255,0,0,255);


		Render2D::Rect(0, (f32)SCRN_HEIGHT - 4, SCRN_WIDTH * percent, 4, Color(255, 0, 0));
		Manager::Debug()->RenderDebugString(wsstr.str(), Vector2(0, SCRN_HEIGHT - 20));
	}
	else
	{
		// ��
		SetColor(0,255,0,255);
		Render2D::Rect(0, (f32)SCRN_HEIGHT - 4, SCRN_WIDTH * percent, 4, Color(0, 255, 0));
		Manager::Debug()->RenderDebugString(wsstr.str(), Vector2(0, SCRN_HEIGHT - 20));
	}


}


//-----------------------------------------------------------------------------
//!	�f���^�^�C���̎擾
//!	@return	�f���^�^�C��
//-----------------------------------------------------------------------------
f32 GetDeltaTime()
{
	FrameRate* p = Singleton<FrameRate>::GetInstance();
	if (p == nullptr)
	{
		//	���̂��Ȃ�����擾�ł��Ȃ�
		ERROR_LOG_W(L"FrameRate�̎��̂��Ȃ�");
		return 0.f;
	}

	return p->GetDeltaTime();
}


f32 GetFrameRate()
{
	FrameRate* p = Singleton<FrameRate>::GetInstance();
	if (p == nullptr)
	{
		//	���̂��Ȃ�����擾�ł��Ȃ�
		ERROR_LOG_W(L"FrameRate�̎��̂��Ȃ�");
		return 0.f;
	}

	return p->GetFrameRate();
}



//!	�f���^�^�C�����|�����l�̍쐬
//!	@return	�f���^�^�C�����|�����l
f32 GetDeltaValue(f32 val)
{
	return val * GetDeltaTime();
}

//!	�f���^�^�C�����|�����l�̍쐬
//!	@return	�f���^�^�C�����|�����l
Vector2 GetDeltaValue(const Vector2& val)
{
	return val * GetDeltaTime();
}

//!	�f���^�^�C�����|�����l�̍쐬
//!	@return	�f���^�^�C�����|�����l
Vector3 GetDeltaValue(const Vector3& val)
{
	return val * GetDeltaTime();
}

//!	�f���^�^�C�����|�����l�̍쐬
//!	@return	�f���^�^�C�����|�����l
Vector4 GetDeltaValue(const Vector4& val)
{
	return val * GetDeltaTime();
}




