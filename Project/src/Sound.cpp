//=============================================================================
//!
//!	@file	Sound.cpp
//!	@brief	サウンド関連
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

namespace /* 外部非公開 */
{
	//------------------------------------------------------------------
	//! ALエラーチェック
	//! @retval	true	エラー
	//! @retval	false	正常
	//------------------------------------------------------------------
	bool IsAlError()
	{
		ALenum err = alGetError();
		switch (err)
		{
		case AL_INVALID_NAME:		//	不正なネームの引数です。
			DEBUG_LOG(L"AL_INVALID_NAME");
			break;
		case AL_INVALID_ENUM:		//	不正な列挙値の引数です。
			DEBUG_LOG(L"AL_INVALID_ENUM");
			break;
		case AL_INVALID_VALUE:		//	不正な引数です。
			DEBUG_LOG(L"AL_INVALID_VALUE");
			break;
		case AL_INVALID_OPERATION:	//	禁止されている呼び出しです。
			DEBUG_LOG(L"AL_INVALID_OPERATION");
			break;
		case AL_OUT_OF_MEMORY:		//	メモリを割り当てる事が出来ません。
			DEBUG_LOG(L"AL_OUT_OF_MEMORY");
			break;
		}

		return err != AL_NO_ERROR;
	}
} // namespace

//=============================================================================
//! オーディオライブラリ
//=============================================================================
class AudioLibrary
{
public:
	//------------------------------------------------------------------
	//! @name	作成 / 解放
	//------------------------------------------------------------------
	//@{

	//!	OpenAL初期化
	static void Initialize()
	{
		_pDevice = alcOpenDevice( nullptr );
		assert(_pDevice != nullptr);
	
		_pContext = alcCreateContext( _pDevice, nullptr );
		assert(_pContext != nullptr);

		alcMakeContextCurrent( _pContext );
		assert(IsAlError() == false);
	}

	//!	OpenAL解放
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
	static ALCdevice*	_pDevice;	//!< ALデバイス
	static ALCcontext*	_pContext;	//!< ALコンテキスト
};
ALCdevice*	AudioLibrary::_pDevice  = nullptr;	//	ALデバイス
ALCcontext*	AudioLibrary::_pContext = nullptr;	//	ALコンテキスト

//=============================================================================
//! @struct    WaveHeader
//! @brief     WAVEヘッダー
//=============================================================================
struct WaveHeader
{
	//! フォーマット
	struct Format
	{
		u32		_fmt;			//!< "fmt"
		u32		_bytes;			//!< fmtチャンクのバイト
		u16		_formatId;		//!< フォーマットID
		u16		_channels;		//!< チャンネル数
		u32		_sampleRate;	//!< サンプリングレート
		u32		_velocity;		//!< データ速度
		u16		_blockSize;		//!< ブロックサイズ
		u16		_bits;			//!< 量子化ビット数
	//	u16		_extension;		//!< 拡張部分のサイズ (リニアPCMならば存在しない)
		// 拡張部分 (extension bytes)
	};

	//! 波形データ
	struct Data
	{
		u32		_id;			//!< "data"
		u32		_dataSize;		//!< 波形データのバイト数
	};

	u32			_riff;			//!< "riff"
	u32			_size8;			//!< ファイルサイズ - 8
	u32			_wave;			//!< "wave"
	Format		_format;		//!< フォーマット
	Data		_data;			//!< 波形データ
};

//struct Mp3Header
//{
//
//};
//=============================================================================
//!	サウンド
//=============================================================================
class SoundInstance : public RefCounter
{
public:
	//------------------------------------------------------------------
	//! @name	作成 / 解放
	//------------------------------------------------------------------
	//@{

	//! コンストラクタ
	SoundInstance()
	: _name           ("")		// 名前
	, _sourceId       (0)		// ソースID
	, _bufferSize     (0)		// バッファサイズ
	, _channels       (0)		// チャンネル
	, _format         (0)		// フォーマット
	, _sampleRate     (0)		// サンプリングレート
	, _currentPosition(0)		// 現在の再生位置
	, _buffer		  (nullptr)	// サンプリングバッファ
	{
	}

	//!	デストラクタ
	virtual ~SoundInstance()
	{
		alDeleteSources(1, &_sourceId);
		_sourceId = 0;
		SafeDeleteArray(_buffer);
	}

	//@}
	//------------------------------------------------------------------
	//! @name	読み込み
	//------------------------------------------------------------------
	//@{

	//! WAVEデータの読み込み
	//! @param	[in]	fileName		ファイルパス
	//! @param	[in]	isStreaming		ストリーミング再生か
	//! @retval	true	読み込み成功
	//! @retval	false	読み込み失敗
	bool LoadWAVE(const std::string& fileName, bool isStreaming);

	//@}

	//! 更新
	void Update();

private:
	//--------------------------------------------------------------
	//! @name    内部関数
	//--------------------------------------------------------------
	//@{

	//! ストリーミング再生用にバッファを作成・キューへ追加
	void SetStreamingBuffer();

	//@}

private:
	//! サウンドマネージャからは自由に参照可能
	friend SoundManager;

	//! 読み込む最大サイズ
	static const s32	READ_SIZE_MAX = 1 * 10224 * 1024;

	std::string			_name;				//!< 名前
	u32					_sourceId;			//!< ソースID

	char*				_buffer;			//!< バッファ
	u32					_bufferSize;		//!< バッファサイズ
	s32					_channels;			//!< チャンネル
	s32					_format;			//!< フォーマット
	s32					_sampleRate;		//!< サンプリングレート
	u32					_currentPosition;	//!< 現在の再生位置
};

//-----------------------------------------------------------------------------
//!	WAVEデータの読み込み
//!	@param	[in]	fileName		ファイルパス
//!	@param	[in]	isStreaming		ストリーミング再生か
//!	@retval	true	読み込み成功
//!	@retval	false	読み込み失敗
//-----------------------------------------------------------------------------
bool SoundInstance::LoadWAVE(const std::string& fileName, bool isStreaming)
{
	//---- ファイルを開く
	std::ifstream fs( fileName, std::ios::binary );
	if( fs.is_open() == false )
	{
		return false;
	}

	//------------------------------------------------------------------
	// ヘッダ部分の読み込み
	//------------------------------------------------------------------
	WaveHeader header;
	fs.read( reinterpret_cast<char*>(&header), sizeof(header) );

	const u32 bits = header._format._bits;
	ALenum format = 0;
	if( _channels == 1 )
	{		// モノクロ
		format = ( bits == 8 ) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
	}
	else {						// ステレオ
		format = ( bits == 8 ) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
	}

	//---- 必要なパラメータを取得
	_name       = fileName;						// ファイルパス
	_bufferSize = header._data._dataSize;		// バッファサイズ
	_channels   = header._format._channels;		// チャンネル
	_sampleRate = header._format._sampleRate;	// サンプリングレート
	_format     = format;						// フォーマット

	//------------------------------------------------------------------
	// 波形データの読み込み
	//------------------------------------------------------------------
	_buffer = new char [ _bufferSize ];
	if( _buffer == nullptr )
	{
		return false;
	}
	fs.read( _buffer, _bufferSize );

	//------------------------------------------------------------------
	// ソースIDを作成
	//------------------------------------------------------------------
	alGenSources( 1, &_sourceId );
	assert(IsAlError() == false);

	if( isStreaming )
	{
		//---- 事前にキューへストリーミング用のバッファを追加しておく
		// ※BGM推奨※
		for(s32 i=0; i<2; ++i)
		{
			SetStreamingBuffer();
		}
	}
	else
	{
		//---- ストリーミング再生を行わない場合は、バッファデータを全て登録する
		// ※SE推奨※
		ALuint buffer;
		alGenBuffers( 1, &buffer );
		assert(IsAlError() == false);

		alBufferData( buffer, _format, _buffer, _bufferSize, _sampleRate );
		assert(IsAlError() == false);

		alSourcei( _sourceId, AL_BUFFER, buffer );
		assert(IsAlError() == false);
	}

	//---- 初期パラメータ
	// 設定しなくでも良いが、明示的に設定しておく
	ALfloat position[3] = { 0.0f };
	ALfloat velocity[3] = { 0.0f };
	alSourcef ( _sourceId, AL_PITCH   , 1.0f         );	// ピッチ
	alSourcef ( _sourceId, AL_GAIN    , 0.5f         );	// 音量
	alSourcefv( _sourceId, AL_POSITION, &position[0] );	// 位置
	alSourcefv( _sourceId, AL_VELOCITY, &velocity[0] );	// 速度
	alSourcei ( _sourceId, AL_LOOPING , AL_FALSE     );	// ループ再生
	return true;
}

//-----------------------------------------------------------------------------
//!	更新
//-----------------------------------------------------------------------------
void SoundInstance::Update()
{
	ALint  type      = 0;
	ALint  processed = 0;
	alGetSourcei( _sourceId, AL_SOURCE_TYPE, &type );
	if( type != AL_STREAMING )
	{
		return;		// ストリーミング再生ではない場合はスキップ
	}

	//------------------------------------------------------------------
	// 空いたバッファを埋め尽くすまでループ
	//------------------------------------------------------------------
	alGetSourcei( _sourceId, AL_BUFFERS_PROCESSED, &processed );
	while( processed != 0 )
	{
		//--------------------------------------------------------------
		// キューからバッファを取り出す
		//--------------------------------------------------------------
		ALuint buffer = 0;
		alSourceUnqueueBuffers( _sourceId, 1, &buffer );
		assert(IsAlError() == false);
		if( buffer == 0 )
		{
			alGenBuffers( 1, &buffer );				// 新規にバッファを作成
			assert(IsAlError() == false);
		}
	
		//--------------------------------------------------------------
		// バッファから一定のデータを読み込む
		//--------------------------------------------------------------
		s32 size = READ_SIZE_MAX;
		if( (_currentPosition + size) > _bufferSize )
		{
			size = _bufferSize - _currentPosition;	// データサイズの調整
		}

		//---- データを書き込む
		std::vector<char> pcm( size );
		memcpy( &pcm[0], &_buffer[_currentPosition], size ) ;

		//---- 位置の更新
		// 位置がバッファサイズを超えた場合は、最初に戻る
		_currentPosition += size;
		if( _currentPosition >= _bufferSize )
		{
			_currentPosition = 0;
		}

		//--------------------------------------------------------------
		// OpenAL のバッファにデータの書き込み
		//--------------------------------------------------------------
		alBufferData( buffer, _format, &pcm[0], size, _sampleRate );
		assert(IsAlError() == false);

		//---- キューに追加
		alSourceQueueBuffers( _sourceId, 1, &buffer );
		assert(IsAlError() == false);

		--processed;
	}
}

//---------------------------------------------------------------------------
//!	ストリーミング再生用にバッファを作成・キューへ追加
//---------------------------------------------------------------------------
void SoundInstance::SetStreamingBuffer()
{
	//------------------------------------------------------------------
	// バッファから一定のデータを読み込む
	//------------------------------------------------------------------
	s32 size = READ_SIZE_MAX;
	if( (_currentPosition + size) > _bufferSize )
	{
		size = _bufferSize - _currentPosition;	// データサイズの調整
	}
	std::vector<char> pcm( size );

	//---- データを書き込む
	memcpy( &pcm[0], &_buffer[_currentPosition], size) ;
	_currentPosition += size;
	if( _currentPosition >= _bufferSize )
	{
		_currentPosition = 0;
	}

	//------------------------------------------------------------------
	// OpenAL のバッファにデータの書き込み
	//------------------------------------------------------------------
	ALuint buffer;
	alGenBuffers( 1, &buffer );
	assert(IsAlError() == false);

	alBufferData( buffer, _format, &pcm[0], size, _sampleRate );
	assert(IsAlError() == false);

	// キューに追加
	alSourceQueueBuffers( _sourceId, 1, &buffer );
	assert(IsAlError() == false);
}



//=============================================================================
//!	サウンドマネージャー
//=============================================================================
//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
SoundManager::SoundManager()
: _pSounds   (0)		// サウンドリスト
, _isMainLoop(false)	// メインループを実行するか
{
}

//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
SoundManager::~SoundManager()
{
}

//-----------------------------------------------------------------------------
//!	セットアップ
//-----------------------------------------------------------------------------
void SoundManager::Init()
{
	AudioLibrary::Initialize();
}

//-----------------------------------------------------------------------------
//!	終了
//-----------------------------------------------------------------------------
void SoundManager::Exit()
{
	//---- スレッドが終了するまで待機
	_isMainLoop = false;

	AudioLibrary::Finalize();
}

//-----------------------------------------------------------------------------
//!	サウンドの更新
//-----------------------------------------------------------------------------
void SoundManager::Update()
{
	auto itr = _pSounds.begin();
	while( itr != _pSounds.end() )
	{
		(*itr)->Update();	// サウンドの更新
		++itr;
	}
}

//-----------------------------------------------------------------------------
//!	サウンドの解放
//!	@param  [in,out] pSound    サウンド
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
			// どこからも参照されなくなったらリストからも削除する
			_pSounds.erase( itr );
		}
	}
}

//-----------------------------------------------------------------------------
//!	BGM読み込み
//! @param	[in]	fileName	ファイルパス
//!	@return	SoundInstanseのポインタ
//!	@retval	nullptr 失敗
//-----------------------------------------------------------------------------
SoundInstance* SoundManager::LoadBGM(const std::string& fileName)
{
	return Load( fileName, true );
}

//-----------------------------------------------------------------------------
//!	SE読み込み
//! @param	[in]	fileName	ファイルパス
//!	@return	SoundInstanseのポインタ
//!	@retval	nullptr 失敗
//-----------------------------------------------------------------------------
SoundInstance* SoundManager::LoadSE(const std::string& fileName)
{
	return Load( fileName, false );
}


//-----------------------------------------------------------------------------
//!	聞き手の姿勢設定
//!	@param	[in]	matrix	姿勢行列
//-----------------------------------------------------------------------------
void SoundManager::SetListener(const Matrix& matrix)
{
	// リスナーの位置(x, y, z)
	Vector3 position = matrix.GetPosition();
	alListenerfv(AL_POSITION, position);

	// リスナーの向き(x, y, z)とUPベクトル(x, y, z)
	Vector3 vec[2] = { matrix.GetZVector(), matrix.GetYVector() };
	alListenerfv(AL_ORIENTATION, (f32*)&vec);
}

//-----------------------------------------------------------------------------
//!	聞き手の姿勢設定
//!	@param	[in]	position	座標
//!	@param	[in]	forward		向きベクトル
//!	@param	[in]	worldUp		上ベクトル
//-----------------------------------------------------------------------------
void SoundManager::SetListener(const Vector3& position, const Vector3& forward, const Vector3& worldUp)
{
	// リスナーの位置(x, y, z)
	alListenerfv(AL_POSITION, position);

	// リスナーの向き(x, y, z)とUPベクトル(x, y, z)
	Vector3 vec[2] = { forward, worldUp };
	alListenerfv(AL_ORIENTATION, (f32*)&vec);
}

//-----------------------------------------------------------------------------
//!	再生
//! @param  [in]    pSound	サウンドID
//! @param  [in]    isLoop	true：ループ, false：ワンショット
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
//!	停止
//! @param  [in]    pSound	サウンド
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
//!	ポーズ
//! @param  [in]    pSound	サウンド
//-----------------------------------------------------------------------------
void SoundManager::Pause(SoundInstance* pSound)
{
	if( pSound )
	{
		alSourcePause( pSound->_sourceId );
	}
}

//-----------------------------------------------------------------------------
//! 音量
//! @param  [in]    pSound	サウンド
//! @param  [in]    volume	音量(0.0 ~ 1.0)
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
//! 再生速度
//! @param  [in]    pSound	サウンド
//! @param  [in]    pitch	速度(0.0 ~ 1.0)
//-----------------------------------------------------------------------------
void SoundManager::Pitch(SoundInstance* pSound, f32 pitch)
{
	if(pSound)
	{
		alSourcef(pSound->_sourceId, AL_PITCH, pitch);
	}
}

//-----------------------------------------------------------------------------
//! 座標
//! @param  [in]    pSound		サウンド
//! @param  [in]    position	座標
//-----------------------------------------------------------------------------
void SoundManager::Position(SoundInstance* pSound, const Vector3& position)
{
	if(pSound)
	{
		alSourcefv(pSound->_sourceId, AL_POSITION, position);
	}
}

//-----------------------------------------------------------------------------
//! 移動速度
//! @param  [in]    pSound		サウンド
//! @param  [in]    velocity	移動量
//-----------------------------------------------------------------------------
void SoundManager::Velocity(SoundInstance* pSound, const Vector3& velocity)
{
	if(pSound)
	{
		alSourcefv(pSound->_sourceId, AL_VELOCITY, velocity);
	}
}

//-----------------------------------------------------------------------------
//! 再生中か
//! @param  [in]    pSound	サウンド
//! @retval true    再生中
//! @retval false   停止中
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
//! サウンド検索
//! @param	[in]	name	ファイルパス
//!	@return	SoundInstanseのポインタ
//!	@retval	nullptr なし
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
//!	サウンド読み込み
//!	@param	[in]	name		ファイルパス
//!	@param	[in]	isStreaming	true：ストリーミング, false：通常
//!	@return	SoundInstanseのポインタ
//!	@retval	nullptr 失敗
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
		//	wav読み込み
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
//!	メインループ
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
			(*itr)->Update();	// サウンドの更新
			++itr;
		}
	}
}

//-----------------------------------------------------------------------------
//! ワーク実行
//! @param	[in]	pThis	自分自身
//-----------------------------------------------------------------------------
void SoundManager::WorkerThread(void* pThis)
{
	assert(pThis);
	SoundManager* p = static_cast<SoundManager*>( pThis );
	p->MainLoop();
}

//=============================================================================
//! サウンドプロセス
//=============================================================================
//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
SoundProcess::SoundProcess()
: _pSound(nullptr)
, _is3dSound(false)
{
}

//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
SoundProcess::~SoundProcess()
{
	Clear();
}

//-----------------------------------------------------------------------------
//!	解放 (参照カウンタ減算)
//-----------------------------------------------------------------------------
void SoundProcess::Clear()
{
	Manager::Sound()->Release(_pSound);
}

//-----------------------------------------------------------------------------
//!	初期化
//! @param	[in]	fileName	ファイルパス
//! @param	[in]	isBGM		true：BGM読み込み, false：SE読み込み
//! @param	[in]	is3dSound	true：3Dサウンド, false：staticサウンド default:false
//-----------------------------------------------------------------------------
void SoundProcess::Init(const std::string& fileName, bool isBGM,bool is3dSound)
{
	Load(fileName, isBGM, is3dSound);
}

//-----------------------------------------------------------------------------
//!	更新
//-----------------------------------------------------------------------------
void SoundProcess::Update()
{
	if(!_is3dSound)return;
	if(!_pParent)return;
	CALL_MANAGER(Manager::Sound(), pSoundManager);
	pSoundManager->Position(_pSound, _pParent->GetWorldPosition());
}


//-----------------------------------------------------------------------------
//!	読み込み
//! @param	[in]	fileName	ファイルパス
//! @param	[in]	isBGM		true：BGM読み込み, false：SE読み込み
//! @param	[in]	is3dSound	true：3Dサウンド, false：staticサウンド default:false
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
//! 再生
//! @param	[in]	isLoop		true：ループ, false：ワンショット
//-----------------------------------------------------------------------------
void SoundProcess::Play(bool isLoop)
{
	CALL_MANAGER(Manager::Sound(), pSoundManager);
	pSoundManager->Play(_pSound,isLoop);
}

//-----------------------------------------------------------------------------
//! 停止
//-----------------------------------------------------------------------------
void SoundProcess::Stop()
{
	CALL_MANAGER(Manager::Sound(), pSoundManager);
	pSoundManager->Stop(_pSound);
}

//-----------------------------------------------------------------------------
//! ポーズ
//-----------------------------------------------------------------------------
void SoundProcess::Pause()
{
	CALL_MANAGER(Manager::Sound(), pSoundManager);
	pSoundManager->Pause(_pSound);
}

//-----------------------------------------------------------------------------
//! 音量
//! @param	[in]	volume	音量(0.0 ~ 1.0)
//-----------------------------------------------------------------------------
void SoundProcess::Volume(f32 volume)
{
	CALL_MANAGER(Manager::Sound(), pSoundManager);
	pSoundManager->Volume(_pSound,volume);
}

//-----------------------------------------------------------------------------
//! 再生中か
//! @retval	true	再生中
//! @retval	false	停止中
//-----------------------------------------------------------------------------
bool SoundProcess::IsPlay()const
{
	CALL_MANAGER(Manager::Sound(), pSoundManager);
	return pSoundManager->IsPlay(_pSound);
}


//=============================================================================
//! サウンドオブジェクト
//=============================================================================
//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
SoundObject::SoundObject()
{
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
SoundObject::~SoundObject()
{
}

//-----------------------------------------------------------------------------
//!	初期化
//! @param	[in]	fileName	ファイルパス
//! @param	[in]	isBGM		true：BGM読み込み, false：SE読み込み
//! @param	[in]	is3dSound	true：3Dサウンド, false：staticサウンド default:false
//-----------------------------------------------------------------------------
void SoundObject::Init(const std::string& fileName, bool isBGM, bool is3dSound)
{
	Load(fileName, isBGM, is3dSound);
}

//-----------------------------------------------------------------------------
//!	更新
//-----------------------------------------------------------------------------
void SoundObject::Update()
{
}

//-----------------------------------------------------------------------------
//! 解放
//-----------------------------------------------------------------------------
void SoundObject::Clear()
{
}

