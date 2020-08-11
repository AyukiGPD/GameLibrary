//=============================================================================
//!
//!	@file	Sound.h
//!	@brief	�T�E���h�֘A
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//=============================================================================
//!	�T�E���h
//!	�錾�̂�.
//!	������ "Sound.cpp" �ōs���Ă��܂�.
//=============================================================================
class SoundInstance;

//=============================================================================
//! �T�E���h�Ǘ��N���X
//=============================================================================
class SoundManager : public ManagerBase
{
public:
	//--------------------------------------------------------------
	//! @name    �쐬 / ���
	//--------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	SoundManager();

	//! �f�X�g���N�^
	virtual ~SoundManager();

	//! �Z�b�g�A�b�v
	void Init();

	//! ���
	void Exit();

	//@}
	//--------------------------------------------------------------
	//! @name    �T�E���h�֘A
	//--------------------------------------------------------------
	//@{

	//! �T�E���h�̍X�V
	void Update();

	//! �T�E���h�̉��
	//! @param  [in,out] pBuffer	�T�E���h
	void Release(SoundInstance*& pSound);

	//! BGM�ǂݍ���
	//! @param	[in]	fileName	�t�@�C���p�X
	//!	@return	SoundInstanse�̃|�C���^
	//!	@retval	nullptr ���s
	SoundInstance* LoadBGM(const std::string& fileName);

	//! SE�ǂݍ���
	//! @param	[in]	fileName	�t�@�C���p�X
	//!	@return	SoundInstanse�̃|�C���^
	//!	@retval	nullptr ���s
	SoundInstance* LoadSE(const std::string& fileName);

	//!	������̎p���ݒ�
	//!	@param	[in]	matrix	�p���s��
	void SetListener(const Matrix& matrix);

	//!	������̎p���ݒ�
	//!	@param	[in]	position	���W
	//!	@param	[in]	forward		�����x�N�g��
	//!	@param	[in]	worldUp		��x�N�g��
	void SetListener(const Vector3& position, const Vector3& forward, const Vector3& worldUp);

	//! �Đ�
	//! @param  [in]    pSound	�T�E���hID
	//! @param  [in]    isLoop	true�F���[�v, false�F�����V���b�g
	void Play(SoundInstance* pSound, bool isLoop = false);

	//! ��~
	//! @param  [in]    pSound	�T�E���h
	void Stop(SoundInstance* pSound);

	//! �|�[�Y
	//! @param  [in]    pSound	�T�E���h
	void Pause(SoundInstance* pSound);

	//! ����
	//! @param  [in]    pSound	�T�E���h
	//! @param  [in]    volume	����(0.0 ~ 1.0)
	void Volume(SoundInstance* pSound, f32 volume);

	//! �Đ����x
	//! @param  [in]    pSound	�T�E���h
	//! @param  [in]    pitch	���x(0.0 ~ 1.0)
	void Pitch(SoundInstance* pSound, f32 pitch);

	//! ���W
	//! @param  [in]    pSound		�T�E���h
	//! @param  [in]    position	���W
	void Position(SoundInstance* pSound, const Vector3& position);

	//! �ړ����x
	//! @param  [in]    pSound		�T�E���h
	//! @param  [in]    velocity	�ړ���
	void Velocity(SoundInstance* pSound, const Vector3& velocity);

	//! �Đ�����
	//! @param  [in]    pSound	�T�E���h
	//! @retval true    �Đ���
	//! @retval false   ��~��
	bool IsPlay(SoundInstance* pSound)const;

	//@}
	//--------------------------------------------------------------
	//! @name    �擾 / �Q��
	//--------------------------------------------------------------
	//@{

	//@}

private:
	//--------------------------------------------------------------
	//! @name    �����֐�
	//--------------------------------------------------------------
	//@{

	//! �T�E���h����
	//! @param	[in]	name	�t�@�C���p�X
	//!	@return	SoundInstanse�̃|�C���^
	//!	@retval	nullptr �Ȃ�
	SoundInstance* Serach(const std::string& name);

	//!	�T�E���h�ǂݍ���
	//!	@param	[in]	name		�t�@�C���p�X
	//!	@param	[in]	isStreaming	true�F�X�g���[�~���O, false�F�ʏ�
	//!	@return	SoundInstanse�̃|�C���^
	//!	@retval	nullptr ���s
	SoundInstance* Load(const std::string& name, bool isStreaming);

	//! ���C�����[�v
	void MainLoop();

	//! ���[�N���s
	//! @param	[in]	pThis	�������g
	static void WorkerThread(void* pThis);

	//@}

private:
	std::list<SoundInstance*>	_pSounds;		//!< �T�E���h���X�g
	bool						_isMainLoop;	//!< ���C�����[�v�����s���邩
};


MANAGER_INTERFACE(SoundManager, Sound);


//=============================================================================
//! �T�E���h�v���Z�X
//=============================================================================
class SoundProcess : public ProcessBase
{
public:
	//! �R���X�g���N�^
	SoundProcess();

	//! �f�X�g���N�^
	virtual ~SoundProcess();

	//!	������
	//! @param	[in]	fileName	�t�@�C���p�X
	//! @param	[in]	isBGM		true�FBGM�ǂݍ���, false�FSE�ǂݍ���
	//! @param	[in]	is3dSound	true�F3D�T�E���h, false�Fstatic�T�E���h default:false
	void Init(const std::string& fileName, bool isBGM,bool is3dSound = false);

	//!	�X�V
	void Update();

	//! ��� (�Q�ƃJ�E���^���Z)
	void Clear();

	//!	�ǂݍ���
	//! @param	[in]	fileName	�t�@�C���p�X
	//! @param	[in]	isBGM		true�FBGM�ǂݍ���, false�FSE�ǂݍ���
	//! @param	[in]	is3dSound	true�F3D�T�E���h, false�Fstatic�T�E���h default:false
	bool Load(const std::string& fileName, bool isBGM, bool is3dSound = false);

	//! �Đ�
	//! @param	[in]	isLoop		true�F���[�v, false�F�����V���b�g
	void Play(bool isLoop = false);

	//! ��~
	void Stop();

	//! �|�[�Y
	void Pause();

	//! ����
	//! @param	[in]	volume	����(0.0 ~ 1.0)
	void Volume(f32 volume);

	//! �Đ�����
	//! @retval	true	�Đ���
	//! @retval	false	��~��
	bool IsPlay()const;

	//!	3D�T�E���h�ݒ�
	//!	@param	[in]	is3DSound	3D�T�E���h�t���O
	void Set3DSound(bool is3dSound)
	{
		_is3dSound = is3dSound;
	}

	//!	�ێ��`�F�b�N
	//!	@retval	false	�ێ����Ă��Ȃ�
	bool IsEmpty()const
	{
		return _pSound == nullptr;
	}

private:
	SoundInstance*	_pSound;	//!< �T�E���h
	bool _is3dSound;
};



//=============================================================================
//! �T�E���h�I�u�W�F�N�g
//=============================================================================
class SoundObject : public GameObjectBase
{
public:
	//! �R���X�g���N�^
	SoundObject();

	//! �f�X�g���N�^
	virtual ~SoundObject();

	//!	������
	//! @param	[in]	fileName	�t�@�C���p�X
	//! @param	[in]	isBGM		true�FBGM�ǂݍ���, false�FSE�ǂݍ���
	//! @param	[in]	is3dSound	true�F3D�T�E���h, false�Fstatic�T�E���h default:false
	void Init(const std::string& fileName, bool isBGM, bool is3dSound = false);

	//!	�X�V
	void Update();

	//! ���
	void Clear();

	//!	�ǂݍ���
	//! @param	[in]	fileName	�t�@�C���p�X
	//! @param	[in]	isBGM		true�FBGM�ǂݍ���, false�FSE�ǂݍ���
	//! @param	[in]	is3dSound	true�F3D�T�E���h, false�Fstatic�T�E���h default:false
	bool Load(const std::string& fileName, bool isBGM, bool is3dSound = false)
	{
		if(_pSound)
		{
			_pSound.reset();
		}
		_pSound = CreateProcessInit<SoundProcess>(fileName, isBGM, is3dSound);
		return (_pSound) && !_pSound->IsEmpty();
	}

	//! �Đ�
	//! @param	[in]	isLoop		true�F���[�v, false�F�����V���b�g
	void Play(bool isLoop = false)
	{
		if(!_pSound) return;
		_pSound->Play(isLoop);
	}

	//! ��~
	void Stop()
	{
		if(!_pSound) return;
		_pSound->Stop();
	}

	//! �|�[�Y
	void Pause()
	{
		if(!_pSound) return;
		_pSound->Pause();
	}

	//! ����
	//! @param	[in]	volume	����(0.0 ~ 1.0)
	void Volume(f32 volume)
	{
		if(!_pSound) return;
		_pSound->Volume(volume);
	}

	//! �Đ�����
	//! @retval	true	�Đ���
	//! @retval	false	��~��
	bool IsPlay()const
	{
		if(!_pSound) return false;
		return _pSound->IsPlay();
	}

	//!	3D�T�E���h�ݒ�
	//!	@param	[in]	is3DSound	3D�T�E���h�t���O
	void Set3DSound(bool is3dSound)
	{
		if(!_pSound) return;
		_pSound->Set3DSound(is3dSound);
	}

	//!	�ێ��`�F�b�N
	//!	@retval	false	�ێ����Ă��Ȃ�
	bool IsEmpty()const
	{
		return !((_pSound) && !_pSound->IsEmpty());
	}
private:
	RefPointer<SoundProcess>	_pSound;	//!< �T�E���h�v���Z�X
};

