//=============================================================================
//!
//!	@file	Sound.cpp
//!	@brief	�T�E���h�֘A
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

namespace /* �O������J */
{
	//------------------------------------------------------------------
	//! AL�G���[�`�F�b�N
	//! @retval	true	�G���[
	//! @retval	false	����
	//------------------------------------------------------------------
	bool IsAlError()
	{
		ALenum err = alGetError();
		switch (err)
		{
		case AL_INVALID_NAME:		//	�s���ȃl�[���̈����ł��B
			DEBUG_LOG(L"AL_INVALID_NAME");
			break;
		case AL_INVALID_ENUM:		//	�s���ȗ񋓒l�̈����ł��B
			DEBUG_LOG(L"AL_INVALID_ENUM");
			break;
		case AL_INVALID_VALUE:		//	�s���Ȉ����ł��B
			DEBUG_LOG(L"AL_INVALID_VALUE");
			break;
		case AL_INVALID_OPERATION:	//	�֎~����Ă���Ăяo���ł��B
			DEBUG_LOG(L"AL_INVALID_OPERATION");
			break;
		case AL_OUT_OF_MEMORY:		//	�����������蓖�Ă鎖���o���܂���B
			DEBUG_LOG(L"AL_OUT_OF_MEMORY");
			break;
		}

		return err != AL_NO_ERROR;
	}
} // namespace

//=============================================================================
//! �I�[�f�B�I���C�u����
//=============================================================================
class AudioLibrary
{
public:
	//------------------------------------------------------------------
	//! @name	�쐬 / ���
	//------------------------------------------------------------------
	//@{

	//!	OpenAL������
	static void Initialize()
	{
		_pDevice = alcOpenDevice( nullptr );
		assert(_pDevice != nullptr);
	
		_pContext = alcCreateContext( _pDevice, nullptr );
		assert(_pContext != nullptr);

		alcMakeContextCurrent( _pContext );
		assert(IsAlError() == false);
	}

	//!	OpenAL���
	static void Finalize()
	{
		if( _pContext )
		{
			alcMakeContextCurrent( nullptr );
			alcDestroyContext    ( _pContext );
			_pContext = nullptr;
		}

		if( _pDevice )
		{
			alcCloseDevice( _pDevice );
			_pDevice = nullptr;
		}
	}

	//@}
private:
	static ALCdevice*	_pDevice;	//!< AL�f�o�C�X
	static ALCcontext*	_pContext;	//!< AL�R���e�L�X�g
};
ALCdevice*	AudioLibrary::_pDevice  = nullptr;	//	AL�f�o�C�X
ALCcontext*	AudioLibrary::_pContext = nullptr;	//	AL�R���e�L�X�g

//=============================================================================
//! @struct    WaveHeader
//! @brief     WAVE�w�b�_�[
//=============================================================================
struct WaveHeader
{
	//! �t�H�[�}�b�g
	struct Format
	{
		u32		_fmt;			//!< "fmt"
		u32		_bytes;			//!< fmt�`�����N�̃o�C�g
		u16		_formatId;		//!< �t�H�[�}�b�gID
		u16		_channels;		//!< �`�����l����
		u32		_sampleRate;	//!< �T���v�����O���[�g
		u32		_velocity;		//!< �f�[�^���x
		u16		_blockSize;		//!< �u���b�N�T�C�Y
		u16		_bits;			//!< �ʎq���r�b�g��
	//	u16		_extension;		//!< �g�������̃T�C�Y (���j�APCM�Ȃ�Α��݂��Ȃ�)
		// �g������ (extension bytes)
	};

	//! �g�`�f�[�^
	struct Data
	{
		u32		_id;			//!< "data"
		u32		_dataSize;		//!< �g�`�f�[�^�̃o�C�g��
	};

	u32			_riff;			//!< "riff"
	u32			_size8;			//!< �t�@�C���T�C�Y - 8
	u32			_wave;			//!< "wave"
	Format		_format;		//!< �t�H�[�}�b�g
	Data		_data;			//!< �g�`�f�[�^
};

//struct Mp3Header
//{
//
//};
//=============================================================================
//!	�T�E���h
//=============================================================================
class SoundInstance : public RefCounter
{
public:
	//------------------------------------------------------------------
	//! @name	�쐬 / ���
	//------------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	SoundInstance()
	: _name           ("")		// ���O
	, _sourceId       (0)		// �\�[�XID
	, _bufferSize     (0)		// �o�b�t�@�T�C�Y
	, _channels       (0)		// �`�����l��
	, _format         (0)		// �t�H�[�}�b�g
	, _sampleRate     (0)		// �T���v�����O���[�g
	, _currentPosition(0)		// ���݂̍Đ��ʒu
	, _buffer		  (nullptr)	// �T���v�����O�o�b�t�@
	{
	}

	//!	�f�X�g���N�^
	virtual ~SoundInstance()
	{
		alDeleteSources(1, &_sourceId);
		_sourceId = 0;
		SafeDeleteArray(_buffer);
	}

	//@}
	//------------------------------------------------------------------
	//! @name	�ǂݍ���
	//------------------------------------------------------------------
	//@{

	//! WAVE�f�[�^�̓ǂݍ���
	//! @param	[in]	fileName		�t�@�C���p�X
	//! @param	[in]	isStreaming		�X�g���[�~���O�Đ���
	//! @retval	true	�ǂݍ��ݐ���
	//! @retval	false	�ǂݍ��ݎ��s
	bool LoadWAVE(const std::string& fileName, bool isStreaming);

	//@}

	//! �X�V
	void Update();

private:
	//--------------------------------------------------------------
	//! @name    �����֐�
	//--------------------------------------------------------------
	//@{

	//! �X�g���[�~���O�Đ��p�Ƀo�b�t�@���쐬�E�L���[�֒ǉ�
	void SetStreamingBuffer();

	//@}

private:
	//! �T�E���h�}�l�[�W������͎��R�ɎQ�Ɖ\
	friend SoundManager;

	//! �ǂݍ��ލő�T�C�Y
	static const s32	READ_SIZE_MAX = 1 * 10224 * 1024;

	std::string			_name;				//!< ���O
	u32					_sourceId;			//!< �\�[�XID

	char*				_buffer;			//!< �o�b�t�@
	u32					_bufferSize;		//!< �o�b�t�@�T�C�Y
	s32					_channels;			//!< �`�����l��
	s32					_format;			//!< �t�H�[�}�b�g
	s32					_sampleRate;		//!< �T���v�����O���[�g
	u32					_currentPosition;	//!< ���݂̍Đ��ʒu
};

//-----------------------------------------------------------------------------
//!	WAVE�f�[�^�̓ǂݍ���
//!	@param	[in]	fileName		�t�@�C���p�X
//!	@param	[in]	isStreaming		�X�g���[�~���O�Đ���
//!	@retval	true	�ǂݍ��ݐ���
//!	@retval	false	�ǂݍ��ݎ��s
//-----------------------------------------------------------------------------
bool SoundInstance::LoadWAVE(const std::string& fileName, bool isStreaming)
{
	//---- �t�@�C�����J��
	std::ifstream fs( fileName, std::ios::binary );
	if( fs.is_open() == false )
	{
		return false;
	}

	//------------------------------------------------------------------
	// �w�b�_�����̓ǂݍ���
	//------------------------------------------------------------------
	WaveHeader header;
	fs.read( reinterpret_cast<char*>(&header), sizeof(header) );

	const u32 bits = header._format._bits;
	ALenum format = 0;
	if( _channels == 1 )
	{		// ���m�N��
		format = ( bits == 8 ) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
	}
	else {						// �X�e���I
		format = ( bits == 8 ) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
	}

	//---- �K�v�ȃp�����[�^���擾
	_name       = fileName;						// �t�@�C���p�X
	_bufferSize = header._data._dataSize;		// �o�b�t�@�T�C�Y
	_channels   = header._format._channels;		// �`�����l��
	_sampleRate = header._format._sampleRate;	// �T���v�����O���[�g
	_format     = format;						// �t�H�[�}�b�g

	//------------------------------------------------------------------
	// �g�`�f�[�^�̓ǂݍ���
	//------------------------------------------------------------------
	_buffer = new char [ _bufferSize ];
	if( _buffer == nullptr )
	{
		return false;
	}
	fs.read( _buffer, _bufferSize );

	//------------------------------------------------------------------
	// �\�[�XID���쐬
	//------------------------------------------------------------------
	alGenSources( 1, &_sourceId );
	assert(IsAlError() == false);

	if( isStreaming )
	{
		//---- ���O�ɃL���[�փX�g���[�~���O�p�̃o�b�t�@��ǉ����Ă���
		// ��BGM������
		for(s32 i=0; i<2; ++i)
		{
			SetStreamingBuffer();
		}
	}
	else
	{
		//---- �X�g���[�~���O�Đ����s��Ȃ��ꍇ�́A�o�b�t�@�f�[�^��S�ēo�^����
		// ��SE������
		ALuint buffer;
		alGenBuffers( 1, &buffer );
		assert(IsAlError() == false);

		alBufferData( buffer, _format, _buffer, _bufferSize, _sampleRate );
		assert(IsAlError() == false);

		alSourcei( _sourceId, AL_BUFFER, buffer );
		assert(IsAlError() == false);
	}

	//---- �����p�����[�^
	// �ݒ肵�Ȃ��ł��ǂ����A�����I�ɐݒ肵�Ă���
	ALfloat position[3] = { 0.0f };
	ALfloat velocity[3] = { 0.0f };
	alSourcef ( _sourceId, AL_PITCH   , 1.0f         );	// �s�b�`
	alSourcef ( _sourceId, AL_GAIN    , 0.5f         );	// ����
	alSourcefv( _sourceId, AL_POSITION, &position[0] );	// �ʒu
	alSourcefv( _sourceId, AL_VELOCITY, &velocity[0] );	// ���x
	alSourcei ( _sourceId, AL_LOOPING , AL_FALSE     );	// ���[�v�Đ�
	return true;
}

//-----------------------------------------------------------------------------
//!	�X�V
//-----------------------------------------------------------------------------
void SoundInstance::Update()
{
	ALint  type      = 0;
	ALint  processed = 0;
	alGetSourcei( _sourceId, AL_SOURCE_TYPE, &type );
	if( type != AL_STREAMING )
	{
		return;		// �X�g���[�~���O�Đ��ł͂Ȃ��ꍇ�̓X�L�b�v
	}

	//------------------------------------------------------------------
	// �󂢂��o�b�t�@�𖄂ߐs�����܂Ń��[�v
	//------------------------------------------------------------------
	alGetSourcei( _sourceId, AL_BUFFERS_PROCESSED, &processed );
	while( processed != 0 )
	{
		//--------------------------------------------------------------
		// �L���[����o�b�t�@�����o��
		//--------------------------------------------------------------
		ALuint buffer = 0;
		alSourceUnqueueBuffers( _sourceId, 1, &buffer );
		assert(IsAlError() == false);
		if( buffer == 0 )
		{
			alGenBuffers( 1, &buffer );				// �V�K�Ƀo�b�t�@���쐬
			assert(IsAlError() == false);
		}
	
		//--------------------------------------------------------------
		// �o�b�t�@������̃f�[�^��ǂݍ���
		//--------------------------------------------------------------
		s32 size = READ_SIZE_MAX;
		if( (_currentPosition + size) > _bufferSize )
		{
			size = _bufferSize - _currentPosition;	// �f�[�^�T�C�Y�̒���
		}

		//---- �f�[�^����������
		std::vector<char> pcm( size );
		memcpy( &pcm[0], &_buffer[_currentPosition], size ) ;

		//---- �ʒu�̍X�V
		// �ʒu���o�b�t�@�T�C�Y�𒴂����ꍇ�́A�ŏ��ɖ߂�
		_currentPosition += size;
		if( _currentPosition >= _bufferSize )
		{
			_currentPosition = 0;
		}

		//--------------------------------------------------------------
		// OpenAL �̃o�b�t�@�Ƀf�[�^�̏�������
		//--------------------------------------------------------------
		alBufferData( buffer, _format, &pcm[0], size, _sampleRate );
		assert(IsAlError() == false);

		//---- �L���[�ɒǉ�
		alSourceQueueBuffers( _sourceId, 1, &buffer );
		assert(IsAlError() == false);

		--processed;
	}
}

//---------------------------------------------------------------------------
//!	�X�g���[�~���O�Đ��p�Ƀo�b�t�@���쐬�E�L���[�֒ǉ�
//---------------------------------------------------------------------------
void SoundInstance::SetStreamingBuffer()
{
	//------------------------------------------------------------------
	// �o�b�t�@������̃f�[�^��ǂݍ���
	//------------------------------------------------------------------
	s32 size = READ_SIZE_MAX;
	if( (_currentPosition + size) > _bufferSize )
	{
		size = _bufferSize - _currentPosition;	// �f�[�^�T�C�Y�̒���
	}
	std::vector<char> pcm( size );

	//---- �f�[�^����������
	memcpy( &pcm[0], &_buffer[_currentPosition], size) ;
	_currentPosition += size;
	if( _currentPosition >= _bufferSize )
	{
		_currentPosition = 0;
	}

	//------------------------------------------------------------------
	// OpenAL �̃o�b�t�@�Ƀf�[�^�̏�������
	//------------------------------------------------------------------
	ALuint buffer;
	alGenBuffers( 1, &buffer );
	assert(IsAlError() == false);

	alBufferData( buffer, _format, &pcm[0], size, _sampleRate );
	assert(IsAlError() == false);

	// �L���[�ɒǉ�
	alSourceQueueBuffers( _sourceId, 1, &buffer );
	assert(IsAlError() == false);
}



//=============================================================================
//!	�T�E���h�}�l�[�W���[
//=============================================================================
//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
SoundManager::SoundManager()
: _pSounds   (0)		// �T�E���h���X�g
, _isMainLoop(false)	// ���C�����[�v�����s���邩
{
}

//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
SoundManager::~SoundManager()
{
}

//-----------------------------------------------------------------------------
//!	�Z�b�g�A�b�v
//-----------------------------------------------------------------------------
void SoundManager::Init()
{
	AudioLibrary::Initialize();
}

//-----------------------------------------------------------------------------
//!	�I��
//-----------------------------------------------------------------------------
void SoundManager::Exit()
{
	//---- �X���b�h���I������܂őҋ@
	_isMainLoop = false;

	AudioLibrary::Finalize();
}

//-----------------------------------------------------------------------------
//!	�T�E���h�̍X�V
//-----------------------------------------------------------------------------
void SoundManager::Update()
{
	auto itr = _pSounds.begin();
	while( itr != _pSounds.end() )
	{
		(*itr)->Update();	// �T�E���h�̍X�V
		++itr;
	}
}

//-----------------------------------------------------------------------------
//!	�T�E���h�̉��
//!	@param  [in,out] pSound    �T�E���h
//-----------------------------------------------------------------------------
void SoundManager::Release(SoundInstance*& pSound)
{
	if( pSound )
	{
		auto itr = _pSounds.begin();
		while( itr != _pSounds.end() )
		{
			if( pSound == (*itr) )
			{
				break;
			}
			++itr;
		}

		s32 refCount = pSound->Release();
		pSound = nullptr;

		if( refCount == 0 )
		{
			// �ǂ�������Q�Ƃ���Ȃ��Ȃ����烊�X�g������폜����
			_pSounds.erase( itr );
		}
	}
}

//-----------------------------------------------------------------------------
//!	BGM�ǂݍ���
//! @param	[in]	fileName	�t�@�C���p�X
//!	@return	SoundInstanse�̃|�C���^
//!	@retval	nullptr ���s
//-----------------------------------------------------------------------------
SoundInstance* SoundManager::LoadBGM(const std::string& fileName)
{
	return Load( fileName, true );
}

//-----------------------------------------------------------------------------
//!	SE�ǂݍ���
//! @param	[in]	fileName	�t�@�C���p�X
//!	@return	SoundInstanse�̃|�C���^
//!	@retval	nullptr ���s
//-----------------------------------------------------------------------------
SoundInstance* SoundManager::LoadSE(const std::string& fileName)
{
	return Load( fileName, false );
}


//-----------------------------------------------------------------------------
//!	������̎p���ݒ�
//!	@param	[in]	matrix	�p���s��
//-----------------------------------------------------------------------------
void SoundManager::SetListener(const Matrix& matrix)
{
	// ���X�i�[�̈ʒu(x, y, z)
	Vector3 position = matrix.GetPosition();
	alListenerfv(AL_POSITION, position);

	// ���X�i�[�̌���(x, y, z)��UP�x�N�g��(x, y, z)
	Vector3 vec[2] = { matrix.GetZVector(), matrix.GetYVector() };
	alListenerfv(AL_ORIENTATION, (f32*)&vec);
}

//-----------------------------------------------------------------------------
//!	������̎p���ݒ�
//!	@param	[in]	position	���W
//!	@param	[in]	forward		�����x�N�g��
//!	@param	[in]	worldUp		��x�N�g��
//-----------------------------------------------------------------------------
void SoundManager::SetListener(const Vector3& position, const Vector3& forward, const Vector3& worldUp)
{
	// ���X�i�[�̈ʒu(x, y, z)
	alListenerfv(AL_POSITION, position);

	// ���X�i�[�̌���(x, y, z)��UP�x�N�g��(x, y, z)
	Vector3 vec[2] = { forward, worldUp };
	alListenerfv(AL_ORIENTATION, (f32*)&vec);
}

//-----------------------------------------------------------------------------
//!	�Đ�
//! @param  [in]    pSound	�T�E���hID
//! @param  [in]    isLoop	true�F���[�v, false�F�����V���b�g
//-----------------------------------------------------------------------------
void SoundManager::Play(SoundInstance* pSound, bool isLoop /* = false */)
{
	if( pSound )
	{
		const s32 sourceId = pSound->_sourceId;
		s32       type     = 0;

		alGetSourcei( sourceId, AL_SOURCE_TYPE, &type );
		if( type != AL_STREAMING )
		{
			alSourcei(sourceId, AL_LOOPING, isLoop ? AL_TRUE : AL_FALSE);
		}
		alSourcePlay( sourceId );
	}
}

//-----------------------------------------------------------------------------
//!	��~
//! @param  [in]    pSound	�T�E���h
//-----------------------------------------------------------------------------
void SoundManager::Stop(SoundInstance* pSound)
{
	if( pSound )
	{
		alSourceStop( pSound->_sourceId );
		pSound->_currentPosition = 0;
	}
}

//-----------------------------------------------------------------------------
//!	�|�[�Y
//! @param  [in]    pSound	�T�E���h
//-----------------------------------------------------------------------------
void SoundManager::Pause(SoundInstance* pSound)
{
	if( pSound )
	{
		alSourcePause( pSound->_sourceId );
	}
}

//-----------------------------------------------------------------------------
//! ����
//! @param  [in]    pSound	�T�E���h
//! @param  [in]    volume	����(0.0 ~ 1.0)
//-----------------------------------------------------------------------------
void SoundManager::Volume(SoundInstance* pSound, f32 volume)
{
	if( pSound )
	{
		f32 vol = max(0.f, min(volume, 1.f));
		alSourcef(pSound->_sourceId, AL_GAIN, vol);
	}
}

//-----------------------------------------------------------------------------
//! �Đ����x
//! @param  [in]    pSound	�T�E���h
//! @param  [in]    pitch	���x(0.0 ~ 1.0)
//-----------------------------------------------------------------------------
void SoundManager::Pitch(SoundInstance* pSound, f32 pitch)
{
	if(pSound)
	{
		alSourcef(pSound->_sourceId, AL_PITCH, pitch);
	}
}

//-----------------------------------------------------------------------------
//! ���W
//! @param  [in]    pSound		�T�E���h
//! @param  [in]    position	���W
//-----------------------------------------------------------------------------
void SoundManager::Position(SoundInstance* pSound, const Vector3& position)
{
	if(pSound)
	{
		alSourcefv(pSound->_sourceId, AL_POSITION, position);
	}
}

//-----------------------------------------------------------------------------
//! �ړ����x
//! @param  [in]    pSound		�T�E���h
//! @param  [in]    velocity	�ړ���
//-----------------------------------------------------------------------------
void SoundManager::Velocity(SoundInstance* pSound, const Vector3& velocity)
{
	if(pSound)
	{
		alSourcefv(pSound->_sourceId, AL_VELOCITY, velocity);
	}
}

//-----------------------------------------------------------------------------
//! �Đ�����
//! @param  [in]    pSound	�T�E���h
//! @retval true    �Đ���
//! @retval false   ��~��
//-----------------------------------------------------------------------------
bool SoundManager::IsPlay(SoundInstance* pSound) const
{
	ALint state = 0;
	if( pSound )
	{
		alGetSourcei( pSound->_sourceId, AL_SOURCE_STATE, &state );
	}
	return state == AL_PLAYING;
}

//-----------------------------------------------------------------------------
//! �T�E���h����
//! @param	[in]	name	�t�@�C���p�X
//!	@return	SoundInstanse�̃|�C���^
//!	@retval	nullptr �Ȃ�
//-----------------------------------------------------------------------------
SoundInstance* SoundManager::Serach(const std::string& name)
{
	SoundInstance* pSound = nullptr;
	auto itr = _pSounds.begin();
	while( itr != _pSounds.end() )
	{
		if( name == (*itr)->_name )
		{
			pSound = *itr;
			break;
		}
		++itr;
	}
	return pSound;
}

//-----------------------------------------------------------------------------
//!	�T�E���h�ǂݍ���
//!	@param	[in]	name		�t�@�C���p�X
//!	@param	[in]	isStreaming	true�F�X�g���[�~���O, false�F�ʏ�
//!	@return	SoundInstanse�̃|�C���^
//!	@retval	nullptr ���s
//-----------------------------------------------------------------------------
SoundInstance* SoundManager::Load(const std::string& name, bool isStreaming)
{
	SoundInstance* pSound = Serach(name);
	if( pSound )
	{
		pSound->AddRef();
		return pSound;
	}

	pSound = new SoundInstance();
	if( pSound == nullptr )
	{
		return nullptr;
	}
	size_t index = name.rfind(".", name.size() - 1);
	if (index != std::string::npos)
	{
		std::string ext = name.substr(index);
		//	wav�ǂݍ���
		if (ext == ".wav" || ext == ".WAV")
		{
			if (pSound->LoadWAVE(name, isStreaming) == false)
			{
				SafeDelete(pSound);
				return nullptr;
			}
		}
		else
		{
			SafeDelete(pSound);
			return nullptr;
		}
		//else if (ext == ".fbx" || ext == ".FBX")
		//{
		//	result = LoadFBX(fileName, scale);
		//}
		//else if (ext == ".x" || ext == ".X")
		//{
		//	result = LoadXFile(fileName, scale);
		//}
		_pSounds.push_back(pSound);
	}

	return pSound;
}

//-----------------------------------------------------------------------------
//!	���C�����[�v
//-----------------------------------------------------------------------------
void SoundManager::MainLoop()
{
	for (;;)
	{
		Sleep( 1 );
		if( _isMainLoop == false )
		{
			std::cout << __FUNCTION__ << std::endl;
			break;
		}
		auto itr = _pSounds.begin();
		while( itr != _pSounds.end() )
		{
			(*itr)->Update();	// �T�E���h�̍X�V
			++itr;
		}
	}
}

//-----------------------------------------------------------------------------
//! ���[�N���s
//! @param	[in]	pThis	�������g
//-----------------------------------------------------------------------------
void SoundManager::WorkerThread(void* pThis)
{
	assert(pThis);
	SoundManager* p = static_cast<SoundManager*>( pThis );
	p->MainLoop();
}

//=============================================================================
//! �T�E���h�v���Z�X
//=============================================================================
//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
SoundProcess::SoundProcess()
: _pSound(nullptr)
, _is3dSound(false)
{
}

//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
SoundProcess::~SoundProcess()
{
	Clear();
}

//-----------------------------------------------------------------------------
//!	��� (�Q�ƃJ�E���^���Z)
//-----------------------------------------------------------------------------
void SoundProcess::Clear()
{
	Manager::Sound()->Release(_pSound);
}

//-----------------------------------------------------------------------------
//!	������
//! @param	[in]	fileName	�t�@�C���p�X
//! @param	[in]	isBGM		true�FBGM�ǂݍ���, false�FSE�ǂݍ���
//! @param	[in]	is3dSound	true�F3D�T�E���h, false�Fstatic�T�E���h default:false
//-----------------------------------------------------------------------------
void SoundProcess::Init(const std::string& fileName, bool isBGM,bool is3dSound)
{
	Load(fileName, isBGM, is3dSound);
}

//-----------------------------------------------------------------------------
//!	�X�V
//-----------------------------------------------------------------------------
void SoundProcess::Update()
{
	if(!_is3dSound)return;
	if(!_pParent)return;
	CALL_MANAGER(Manager::Sound(), pSoundManager);
	pSoundManager->Position(_pSound, _pParent->GetWorldPosition());
}


//-----------------------------------------------------------------------------
//!	�ǂݍ���
//! @param	[in]	fileName	�t�@�C���p�X
//! @param	[in]	isBGM		true�FBGM�ǂݍ���, false�FSE�ǂݍ���
//! @param	[in]	is3dSound	true�F3D�T�E���h, false�Fstatic�T�E���h default:false
//-----------------------------------------------------------------------------
bool SoundProcess::Load(const std::string& fileName, bool isBGM, bool is3dSound)
{
	Clear();
	CALL_MANAGER(Manager::Sound(), pSoundManager);
	if(isBGM)
	{
		_pSound = pSoundManager->LoadBGM(fileName);
	}
	else
	{
		_pSound = pSoundManager->LoadSE(fileName);
	}
	_is3dSound = is3dSound;
	return _pSound != nullptr;
}

//-----------------------------------------------------------------------------
//! �Đ�
//! @param	[in]	isLoop		true�F���[�v, false�F�����V���b�g
//-----------------------------------------------------------------------------
void SoundProcess::Play(bool isLoop)
{
	CALL_MANAGER(Manager::Sound(), pSoundManager);
	pSoundManager->Play(_pSound,isLoop);
}

//-----------------------------------------------------------------------------
//! ��~
//-----------------------------------------------------------------------------
void SoundProcess::Stop()
{
	CALL_MANAGER(Manager::Sound(), pSoundManager);
	pSoundManager->Stop(_pSound);
}

//-----------------------------------------------------------------------------
//! �|�[�Y
//-----------------------------------------------------------------------------
void SoundProcess::Pause()
{
	CALL_MANAGER(Manager::Sound(), pSoundManager);
	pSoundManager->Pause(_pSound);
}

//-----------------------------------------------------------------------------
//! ����
//! @param	[in]	volume	����(0.0 ~ 1.0)
//-----------------------------------------------------------------------------
void SoundProcess::Volume(f32 volume)
{
	CALL_MANAGER(Manager::Sound(), pSoundManager);
	pSoundManager->Volume(_pSound,volume);
}

//-----------------------------------------------------------------------------
//! �Đ�����
//! @retval	true	�Đ���
//! @retval	false	��~��
//-----------------------------------------------------------------------------
bool SoundProcess::IsPlay()const
{
	CALL_MANAGER(Manager::Sound(), pSoundManager);
	return pSoundManager->IsPlay(_pSound);
}


//=============================================================================
//! �T�E���h�I�u�W�F�N�g
//=============================================================================
//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
SoundObject::SoundObject()
{
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
SoundObject::~SoundObject()
{
}

//-----------------------------------------------------------------------------
//!	������
//! @param	[in]	fileName	�t�@�C���p�X
//! @param	[in]	isBGM		true�FBGM�ǂݍ���, false�FSE�ǂݍ���
//! @param	[in]	is3dSound	true�F3D�T�E���h, false�Fstatic�T�E���h default:false
//-----------------------------------------------------------------------------
void SoundObject::Init(const std::string& fileName, bool isBGM, bool is3dSound)
{
	Load(fileName, isBGM, is3dSound);
}

//-----------------------------------------------------------------------------
//!	�X�V
//-----------------------------------------------------------------------------
void SoundObject::Update()
{
}

//-----------------------------------------------------------------------------
//! ���
//-----------------------------------------------------------------------------
void SoundObject::Clear()
{
}

