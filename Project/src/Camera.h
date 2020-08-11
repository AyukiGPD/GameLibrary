//=============================================================================
//!
//!	@file	Camera.h
//!	@brief	カメラクラス
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once


class BaseDeferredRenderer;


//-------------------------------------------------------------------
//!	Hud系オブジェクト
//-------------------------------------------------------------------
namespace Hud
{
//-------------------------------------------------------------------
//!	描画オブジェクトベース
//-------------------------------------------------------------------
class RenderBase
{
public:
	//!	コンストラクタ
	RenderBase() :_position(Vector2::ZERO){}
	//!	コンストラクタ
	//!	@param	[in]	position	座標
	RenderBase(const Vector2 position) :_position(position){}
	//!	デストラクタ
	virtual ~RenderBase(){}

	//!	描画
	virtual void Render(){}
protected:
	Vector2 _position;	//!< 座標
};

//-------------------------------------------------------------------
//!	文字列描画オブジェクト
//-------------------------------------------------------------------
class RenderStringObject : public RenderBase
{
public:
	//!	コンストラクタ
	RenderStringObject()
	: _str(L"")
	, _size(32.f)
	{}
	//!	デストラクタ
	virtual ~RenderStringObject(){}

	//!	設定
	//!	@param	[in]	hundler		フォントデータ
	//!	@param	[in]	position	座標
	//!	@param	[in]	color		色
	//!	@param	[in]	str			描画文字
	//!	@param	[in]	size		文字サイズ(仮) default:64.0f
	void Set(const FontHundler& hundler, const Vector2& position, const Color& color, const std::wstring& str, f32 size = 64.f);

	//!	描画
	void Render();
private:
	FontHundler		_fontHundler;	//!< フォントデータ
	std::wstring	_str;			//!< 文字列
	Color			_color;			//!< 色
	f32				_size;			//!< 描画サイズ(仮)
};

//-------------------------------------------------------------------
//!	テクスチャ描画オブジェクト
//-------------------------------------------------------------------
class RenderTextureObject : public RenderBase
{
public:
	//!	コンストラクタ
	RenderTextureObject()
	: _pTexture(nullptr)
	, _uv(Vector4(0.0f, 0.0f, 1.0f, 1.0f))
	, _size(Vector2::ONE)
	{}
	//!	デストラクタ
	virtual ~RenderTextureObject(){}

	//!	設定
	//!	@param	[in]	position	座標
	//!	@param	[in]	pTexture	テクスチャポインタ
	//!	@param	[in]	uv			uv default:(0.0f,0.0f,1.0f,1.0f)
	void Set(const Vector2& position, Texture* pTexture, const Vector4& uv = Vector4(0.0f, 0.0f, 1.0f, 1.0f));

	//!	描画
	void Render();
private:
	Texture* _pTexture;	//!< 表示テクスチャ
	Vector4 _uv;		//!< UV
	Vector2 _size;		//!< 描画サイズ
};

//-------------------------------------------------------------------
//!	テクスチャ描画オブジェクト
//-------------------------------------------------------------------
class RenderRectObject : public RenderBase
{
public:
	//!	コンストラクタ
	RenderRectObject()
	: _size(Vector2::ONE)
	, _color(255)
	{}
	//!	デストラクタ
	virtual ~RenderRectObject(){}

	//!	設定
	//!	@param	[in]	position	座標
	//!	@param	[in]	size		文字サイズ
	//!	@param	[in]	color		色
	void Set(const Vector2& position, const Vector2& size, const Color& color);

	//!	描画
	void Render();
private:
	Vector2	_size;		//!< 描画サイズ
	Color	_color;		//!< 色
};


//-------------------------------------------------------------------
//!	描画オブジェクト管理クラス
//!	@tparam	T	オブジェクト型
//!	@param		MAX_SIZE	オブジェクト数
//-------------------------------------------------------------------
template <class T, s32 MAX_SIZE>
class RenderObjects
{
public:
	//!	コンストラクタ
	RenderObjects()
	:_arrayCount(0)
	{
	}
	//!	デストラクタ
	virtual ~RenderObjects()
	{
	}

	//!	オブジェクト設定
	//!	@param	[in]	val	オブジェクト
	void SetObject(const T& val)
	{
		if (_arrayCount >= MAX_SIZE)return;
		u32 index = _arrayCount;
		++_arrayCount;
		_renderArray[index] = val;
	}

	//!	描画
	void Render()
	{
		for (u32 i = 0; i < _arrayCount; ++i)
		{
			_renderArray[i].Render();
		}
		_arrayCount = 0;
	}
private:
	std::array<T, MAX_SIZE> _renderArray;	//!< オブジェクト
	u32 _arrayCount;						//!< オブジェクト数
};

};	//	namespace Hud




static const u32 RENDER_ARRAY_MAX = 64;

//-------------------------------------------------------------------
//!	HUDオブジェクト オーバーライド可
//-------------------------------------------------------------------
class HudObject : public BasicBase
{
	friend class CameraObject;
public:
	//!	コンストラクタ
	HudObject()
	: _pFrameBuffer(nullptr)
	{
	}
	//!	デストラクタ
	virtual ~HudObject()
	{
		SafeDelete(_pFrameBuffer);
	}

	//!	文字描画
	//!	@param	[in]	hundler		フォントデータ
	//!	@param	[in]	position	座標
	//!	@param	[in]	color		色
	//!	@param	[in]	str			描画文字
	//!	@param	[in]	size		文字サイズ(仮) default:64.0f
	void RenderString(const FontHundler& hundler, const Vector2& position, const Color& color, const std::wstring& str, f32 size = 64.f);
	//!	テクスチャ描画
	//!	@param	[in]	position	座標
	//!	@param	[in]	pTexture	テクスチャポインタ
	//!	@param	[in]	uv			uv default:(0.0f,0.0f,1.0f,1.0f)
	void RenderTexture(const Vector2& position, Texture* pTexture, const Vector4& uv = Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	//!	四角描画
	//!	@param	[in]	position	座標
	//!	@param	[in]	size		文字サイズ
	//!	@param	[in]	color		色
	void RenderRect(const Vector2& position, const Vector2& size, const Color& color);

protected:
	//!	フレームバッファ初期化
	//!	@param	[in]	width	横
	//!	@param	[in]	height	縦
	bool InitFrameBuffer(u32 width, u32 height);

	//!	バッファ開始
	bool BindFrameBuffer();
	//!	バッファ終了
	void UnBindFrameBuffer();

	//!	オブジェクトの描画
	void RenderObjects();
	//!	バッファ描画
	void RenderBuffer();
private:
	class FrameBuffer* _pFrameBuffer;	//!< フレームバッファ

	Hud::RenderObjects<Hud::RenderRectObject,	 RENDER_ARRAY_MAX> _rects;		//!< 四角
	Hud::RenderObjects<Hud::RenderStringObject,  RENDER_ARRAY_MAX> _strings;	//!< 文字
	Hud::RenderObjects<Hud::RenderTextureObject, RENDER_ARRAY_MAX> _textures;	//!< テクスチャ
};

//-----------------------------------------------------------------------------
//!	カメラオブジェクト
//-----------------------------------------------------------------------------
class CameraObject : public GameObjectBase
{
public:
	friend class CameraManager;
	struct Desc
	{
		//!	カメラ位置行列
		//!	@param	[in]	position	座標
		//!	@param	[in]	lookAt		視点
		//!	@param	[in]	worldUp		カメラ上の方向	default:(0,1,0)
		void LookAt(const Vector3& position, const Vector3& lookAt, const Vector3& worldUp = Vector3(0, 1, 0))
		{
			_position	= position;
			_lookAt		= lookAt;
			_worldUp	= worldUp;
		}

		//!	透視投影
		//!	@param	[in]	fovy	縦の視野角
		//!	@param	[in]	width	横
		//!	@param	[in]	height	縦
		//!	@param	[in]	znear	手前
		//!	@param	[in]	zfar	奥行き
		void Perspective(f32 fovy, s32 width, s32 height, f32 znear, f32 zfar)
		{
			_fovy	= fovy;
			_width	= width;
			_height	= height;
			_znear	= znear;
			_zfar	= zfar;
		}

		Vector3	_position;		//!< カメラ座標
		Vector3	_lookAt;		//!< 視点
		Vector3	_worldUp;		//!< カメラ上方向
		f32		_fovy;			//!< 縦視野角
		s32		_width;			//!< 横
		s32		_height;		//!< 縦
		f32		_znear;			//!< 手前
		f32		_zfar;			//!< 奥行き
	};

	//!	コンストラクタ
	CameraObject();
	//!	コンストラクタ
	CameraObject(const Desc& desc);
	//!	デストラクタ
	virtual ~CameraObject();

public:
	//!	更新
	void Update();

	//!	ディファードレンダリングクラス作成
	//!	@tparam	T		DeferredRendererクラス
	//!	@retval	true	成功
	//!	@retval	false	失敗
	template<typename T>
	bool Set(const Desc& desc)
	{
		SetCamera(desc);
		_pDeferredRenderer = new T();
		if(!_pDeferredRenderer->Init(_width, _height))
		{
			SafeDelete(_pDeferredRenderer);
		}
		return _pDeferredRenderer != nullptr;
	}

	//!	ディファードレンダリングクラス作成
	//!	@tparam	T		DeferredRendererクラス
	//!	@retval	true	成功
	//!	@retval	false	失敗
	template<typename T>
	bool InitDeferredRenderer()
	{
		_pDeferredRenderer = new T();
		if(!_pDeferredRenderer->Init(_width, _height))
		{
			SafeDelete(_pDeferredRenderer);
		}
		return _pDeferredRenderer != nullptr;
	}

	//!	Hud初期化
	//!	@tparam	T		Hudオブジェクト default:HudObject
	//!	@retval	true	成功
	//!	@retval	false	失敗
	template<typename T = HudObject>
	bool InitHUDObject()
	{
		_pHudObject = CreateRefPointer<T>();
		if(!_pHudObject)return false;
		return _pHudObject->InitFrameBuffer(_width, _height);
	}

	//!	ディファードレンダリングクラス削除
	void DeleteDeferredRenderer();

	//!	開始
	void Begin();
	//!	終了
	void End();

	//!	バッファ表示
	void RenderBuffer();
	//!	HUD表示
	void RenderHUD();

	//--------------------------------------------------------------
	//! @name    取得関数
	//--------------------------------------------------------------
	//@{
	//!	カメラの向き取得
	//!	@return	カメラのベクトル
	Vector3 GetCameraVector()const
	{
		return Vector3(_matView[2].x, _matView[2].y, _matView[2].z);
	}

	//!	縦視野角
	//!	@return	縦視野角
	f32 GetFovy()const
	{
		return _fovy;
	}
	//!	画面比率
	//!	@return	画面比率
	f32 GetAspect()const
	{
		return _aspect;
	}
	//!	横
	//!	@return 横サイズ
	s32 GetWidth()const
	{
		return _width;
	}
	//!	縦
	//!	@return	縦サイズ
	s32 GetHeight()const
	{
		return _height;
	}
	//!	手前
	//!	@return	手前
	f32 GetZnear()const
	{
		return _znear;
	}
	//!	奥行き
	//!	@return	奥行き
	f32 GetZfar()const
	{
		return _zfar;
	}

	//!	ビュー行列
	//!	@return	ビュー行列
	const Matrix& GetViewMatrix() const
	{
		return _matView;
	}
	//!	プロジェクション行列
	//!	@return	プロジェクション行列
	const Matrix& GetProjectionMatrix() const
	{
		return _matProj;
	}

	//!	ディファードレンダリング
	//!	@return	ディファードレンダリングオブジェクトのポインタ
	BaseDeferredRenderer* GetDeferredRenderer()const
	{
		return _pDeferredRenderer;
	}

	//!	バッファ取得
	//!	@return	ディファードレンダリングのテクスチャ
	Texture* GetBufferTexture()const;

	//!	Hudオブジェクト
	//!	@return	Hudオブジェクトのポインタ
	HudObject* GetHud()
	{
		return _pHudObject.get();
	}

	//@}

	//--------------------------------------------------------------
	//! @name    設定関数
	//--------------------------------------------------------------
	//@{
	//!	カメラ位置行列
	//!	@param	[in]	position	座標
	//!	@param	[in]	lookAt		視点
	//!	@param	[in]	worldUp		カメラ上の方向	default:(0,1,0)
	void LookAt(const Vector3& position, const Vector3& lookAt, const Vector3& worldUp = Vector3(0, 1, 0));

	//!	透視投影
	//!	@param	[in]	fovy	縦の視野角
	//!	@param	[in]	width	横
	//!	@param	[in]	height	縦
	//!	@param	[in]	znear	手前
	//!	@param	[in]	zfar	奥行き
	void Perspective(f32 fovy, s32 width, s32 height, f32 znear, f32 zfar);

	//!	設定
	//!@param	[in]	desc	情報
	void SetCamera(const CameraObject::Desc& desc);

	//!	縦視野角
	//!	@param	[in]	fovy	縦視野角
	void SetFovy(f32 fovy)
	{
		_fovy = fovy;
	}
	//!	画面比率
	//!	@param	[in]	width	横
	//!	@param	[in]	height	縦
	void SetAspect(s32 width, s32 height)
	{
		_width = width;
		_height = height;
		_aspect = (f32)_width / (f32)_height;
	}
	//!	横
	//!	@param	[in]	width	横
	void SetWidth(s32 width)
	{
		_width = width;
		_aspect = (f32)_width / (f32)_height;
	}
	//!	縦
	//!	@param	[in]	height	縦
	void SetHeight(s32 height)
	{
		_height = height;
		_aspect = (f32)_width / (f32)_height;
	}
	//!	手前
	//!	@param	[in]	znear	手前
	void SetZnear(f32 znear)
	{
		_znear = znear;
	}
	//!	奥行き
	//!	@param	[in]	zfar	奥行き
	void SetZfar(f32 zfar)
	{
		_zfar = zfar;
	}

	//!	カメラ振動
	//!	@param	[in]	quakeScale	カメラ振動スケール
	void SetQuakeScale(f32 quakeScale)
	{
		_quakeScale = quakeScale;
	}

	//@}

private:
	Vector3	_lookAt;		//!< 視点
	Vector3	_worldUp;		//!< カメラ上方向

	f32		_fovy;			//!< 縦視野角
	f32		_aspect;		//!< 横視野角
	f32		_znear;			//!< 手前
	f32		_zfar;			//!< 奥行き

	s32		_width;			//!< 横
	s32		_height;		//!< 縦

	Matrix	_matProj;		//!< プロジェクション行列
	Matrix	_matView;		//!< ビュー行列
	f32		_quakeScale;	//!< カメラ振動

	BaseDeferredRenderer*	_pDeferredRenderer;	//!< ディファードレンダリングオブジェクト
	RefPointer<HudObject>	_pHudObject;		//!< Hudオブジェクト

};


//-----------------------------------------------------------------------------
//!	カメラ管理クラス
//-----------------------------------------------------------------------------
class CameraManager : public ManagerBase
{
public:
	friend CameraObject;
public:
	//friend Camera;
	//!	コンストラクタ
	CameraManager();
	//!	デストラクタ
	virtual ~CameraManager();

	//!	カメラ設定
	//!	@param	[in]	camera	有効にするカメラ
	void ChangeCamera(CameraObject* camera);

	//!	有効になってるカメラ
	//!	@return	有効なカメラのポインタ
	CameraObject* GetRunningCamera()
	{
		return _pRunningCamera;
	}


private:
	//!	カメラ解放
	//!	これを通して_pRunningCameraのチェックをする
	//!	@param	[in]	解放するカメラ
	void ReleaseCamera(CameraObject* pCamera);
private:
	CameraObject* _pRunningCamera;		//!< 現在動いてるカメラ
};

MANAGER_INTERFACE(CameraManager, Camera);
