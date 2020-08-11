//=============================================================================
//!
//!	@file	Sound.h
//!	@brief	サウンド関連
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//=============================================================================
//!	サウンド
//!	宣言のみ.
//!	実装は "Sound.cpp" で行っています.
//=============================================================================
class SoundInstance;

//=============================================================================
//! サウンド管理クラス
//=============================================================================
class SoundManager : public ManagerBase
{
public:
	//--------------------------------------------------------------
	//! @name    作成 / 解放
	//--------------------------------------------------------------
	//@{

	//! コンストラクタ
	SoundManager();

	//! デストラクタ
	virtual ~SoundManager();

	//! セットアップ
	void Init();

	//! 解放
	void Exit();

	//@}
	//--------------------------------------------------------------
	//! @name    サウンド関連
	//--------------------------------------------------------------
	//@{

	//! サウンドの更新
	void Update();

	//! サウンドの解放
	//! @param  [in,out] pBuffer	サウンド
	void Release(SoundInstance*& pSound);

	//! BGM読み込み
	//! @param	[in]	fileName	ファイルパス
	//!	@return	SoundInstanseのポインタ
	//!	@retval	nullptr 失敗
	SoundInstance* LoadBGM(const std::string& fileName);

	//! SE読み込み
	//! @param	[in]	fileName	ファイルパス
	//!	@return	SoundInstanseのポインタ
	//!	@retval	nullptr 失敗
	SoundInstance* LoadSE(const std::string& fileName);

	//!	聞き手の姿勢設定
	//!	@param	[in]	matrix	姿勢行列
	void SetListener(const Matrix& matrix);

	//!	聞き手の姿勢設定
	//!	@param	[in]	position	座標
	//!	@param	[in]	forward		向きベクトル
	//!	@param	[in]	worldUp		上ベクトル
	void SetListener(const Vector3& position, const Vector3& forward, const Vector3& worldUp);

	//! 再生
	//! @param  [in]    pSound	サウンドID
	//! @param  [in]    isLoop	true：ループ, false：ワンショット
	void Play(SoundInstance* pSound, bool isLoop = false);

	//! 停止
	//! @param  [in]    pSound	サウンド
	void Stop(SoundInstance* pSound);

	//! ポーズ
	//! @param  [in]    pSound	サウンド
	void Pause(SoundInstance* pSound);

	//! 音量
	//! @param  [in]    pSound	サウンド
	//! @param  [in]    volume	音量(0.0 ~ 1.0)
	void Volume(SoundInstance* pSound, f32 volume);

	//! 再生速度
	//! @param  [in]    pSound	サウンド
	//! @param  [in]    pitch	速度(0.0 ~ 1.0)
	void Pitch(SoundInstance* pSound, f32 pitch);

	//! 座標
	//! @param  [in]    pSound		サウンド
	//! @param  [in]    position	座標
	void Position(SoundInstance* pSound, const Vector3& position);

	//! 移動速度
	//! @param  [in]    pSound		サウンド
	//! @param  [in]    velocity	移動量
	void Velocity(SoundInstance* pSound, const Vector3& velocity);

	//! 再生中か
	//! @param  [in]    pSound	サウンド
	//! @retval true    再生中
	//! @retval false   停止中
	bool IsPlay(SoundInstance* pSound)const;

	//@}
	//--------------------------------------------------------------
	//! @name    取得 / 参照
	//--------------------------------------------------------------
	//@{

	//@}

private:
	//--------------------------------------------------------------
	//! @name    内部関数
	//--------------------------------------------------------------
	//@{

	//! サウンド検索
	//! @param	[in]	name	ファイルパス
	//!	@return	SoundInstanseのポインタ
	//!	@retval	nullptr なし
	SoundInstance* Serach(const std::string& name);

	//!	サウンド読み込み
	//!	@param	[in]	name		ファイルパス
	//!	@param	[in]	isStreaming	true：ストリーミング, false：通常
	//!	@return	SoundInstanseのポインタ
	//!	@retval	nullptr 失敗
	SoundInstance* Load(const std::string& name, bool isStreaming);

	//! メインループ
	void MainLoop();

	//! ワーク実行
	//! @param	[in]	pThis	自分自身
	static void WorkerThread(void* pThis);

	//@}

private:
	std::list<SoundInstance*>	_pSounds;		//!< サウンドリスト
	bool						_isMainLoop;	//!< メインループを実行するか
};


MANAGER_INTERFACE(SoundManager, Sound);


//=============================================================================
//! サウンドプロセス
//=============================================================================
class SoundProcess : public ProcessBase
{
public:
	//! コンストラクタ
	SoundProcess();

	//! デストラクタ
	virtual ~SoundProcess();

	//!	初期化
	//! @param	[in]	fileName	ファイルパス
	//! @param	[in]	isBGM		true：BGM読み込み, false：SE読み込み
	//! @param	[in]	is3dSound	true：3Dサウンド, false：staticサウンド default:false
	void Init(const std::string& fileName, bool isBGM,bool is3dSound = false);

	//!	更新
	void Update();

	//! 解放 (参照カウンタ減算)
	void Clear();

	//!	読み込み
	//! @param	[in]	fileName	ファイルパス
	//! @param	[in]	isBGM		true：BGM読み込み, false：SE読み込み
	//! @param	[in]	is3dSound	true：3Dサウンド, false：staticサウンド default:false
	bool Load(const std::string& fileName, bool isBGM, bool is3dSound = false);

	//! 再生
	//! @param	[in]	isLoop		true：ループ, false：ワンショット
	void Play(bool isLoop = false);

	//! 停止
	void Stop();

	//! ポーズ
	void Pause();

	//! 音量
	//! @param	[in]	volume	音量(0.0 ~ 1.0)
	void Volume(f32 volume);

	//! 再生中か
	//! @retval	true	再生中
	//! @retval	false	停止中
	bool IsPlay()const;

	//!	3Dサウンド設定
	//!	@param	[in]	is3DSound	3Dサウンドフラグ
	void Set3DSound(bool is3dSound)
	{
		_is3dSound = is3dSound;
	}

	//!	保持チェック
	//!	@retval	false	保持していない
	bool IsEmpty()const
	{
		return _pSound == nullptr;
	}

private:
	SoundInstance*	_pSound;	//!< サウンド
	bool _is3dSound;
};



//=============================================================================
//! サウンドオブジェクト
//=============================================================================
class SoundObject : public GameObjectBase
{
public:
	//! コンストラクタ
	SoundObject();

	//! デストラクタ
	virtual ~SoundObject();

	//!	初期化
	//! @param	[in]	fileName	ファイルパス
	//! @param	[in]	isBGM		true：BGM読み込み, false：SE読み込み
	//! @param	[in]	is3dSound	true：3Dサウンド, false：staticサウンド default:false
	void Init(const std::string& fileName, bool isBGM, bool is3dSound = false);

	//!	更新
	void Update();

	//! 解放
	void Clear();

	//!	読み込み
	//! @param	[in]	fileName	ファイルパス
	//! @param	[in]	isBGM		true：BGM読み込み, false：SE読み込み
	//! @param	[in]	is3dSound	true：3Dサウンド, false：staticサウンド default:false
	bool Load(const std::string& fileName, bool isBGM, bool is3dSound = false)
	{
		if(_pSound)
		{
			_pSound.reset();
		}
		_pSound = CreateProcessInit<SoundProcess>(fileName, isBGM, is3dSound);
		return (_pSound) && !_pSound->IsEmpty();
	}

	//! 再生
	//! @param	[in]	isLoop		true：ループ, false：ワンショット
	void Play(bool isLoop = false)
	{
		if(!_pSound) return;
		_pSound->Play(isLoop);
	}

	//! 停止
	void Stop()
	{
		if(!_pSound) return;
		_pSound->Stop();
	}

	//! ポーズ
	void Pause()
	{
		if(!_pSound) return;
		_pSound->Pause();
	}

	//! 音量
	//! @param	[in]	volume	音量(0.0 ~ 1.0)
	void Volume(f32 volume)
	{
		if(!_pSound) return;
		_pSound->Volume(volume);
	}

	//! 再生中か
	//! @retval	true	再生中
	//! @retval	false	停止中
	bool IsPlay()const
	{
		if(!_pSound) return false;
		return _pSound->IsPlay();
	}

	//!	3Dサウンド設定
	//!	@param	[in]	is3DSound	3Dサウンドフラグ
	void Set3DSound(bool is3dSound)
	{
		if(!_pSound) return;
		_pSound->Set3DSound(is3dSound);
	}

	//!	保持チェック
	//!	@retval	false	保持していない
	bool IsEmpty()const
	{
		return !((_pSound) && !_pSound->IsEmpty());
	}
private:
	RefPointer<SoundProcess>	_pSound;	//!< サウンドプロセス
};

