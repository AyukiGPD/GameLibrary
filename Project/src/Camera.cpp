//=============================================================================
//!
//!	@file	Camera.cpp
//!	@brief	カメラクラス
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

namespace Hud
{

//-----------------------------------------------------------------------------
//!	設定
//!	@param	[in]	hundler		フォントデータ
//!	@param	[in]	position	座標
//!	@param	[in]	color		色
//!	@param	[in]	str			描画文字
//!	@param	[in]	size		文字サイズ(仮) default:64.0f
//-----------------------------------------------------------------------------
void RenderStringObject::Set(const FontHundler& hundler, const Vector2& position, const Color& color, const std::wstring& str, f32 size)
{
	_fontHundler = hundler;
	_position = position;
	_color = color;
	_str = str;
	_size = size;
}

//-----------------------------------------------------------------------------
//!	描画
//-----------------------------------------------------------------------------
void RenderStringObject::Render()
{
	if(_str.empty())return;

	if(!_fontHundler)return;

	SetColor(_color);
	_fontHundler.Render(_str, _position);
}

//-----------------------------------------------------------------------------
//!	設定
//!	@param	[in]	position	座標
//!	@param	[in]	pTexture	テクスチャポインタ
//!	@param	[in]	uv			uv default:(0.0f,0.0f,1.0f,1.0f)
//-----------------------------------------------------------------------------
void RenderTextureObject::Set(const Vector2& position, Texture* pTexture, const Vector4& uv)
{
	_position = position;
	_pTexture = pTexture;
	_uv = uv;
}

//-----------------------------------------------------------------------------
//!	描画
//-----------------------------------------------------------------------------
void RenderTextureObject::Render()
{
	if(!_pTexture)return;

	//	未実装
}

//-----------------------------------------------------------------------------
//!	設定
//!	@param	[in]	position	座標
//!	@param	[in]	size		文字サイズ
//!	@param	[in]	color		色
//-----------------------------------------------------------------------------
void RenderRectObject::Set(const Vector2& position, const Vector2& size, const Color& color)
{
	_position = position;
	_size = size;
	_color = color;
}

//-----------------------------------------------------------------------------
//!	描画
//-----------------------------------------------------------------------------
void RenderRectObject::Render()
{
	Render2D::Rect(_position, _size, _color);
}



}	//	namespace Hud

//-----------------------------------------------------------------------------
//!	文字描画
//!	@param	[in]	hundler		フォントデータ
//!	@param	[in]	position	座標
//!	@param	[in]	color		色
//!	@param	[in]	str			描画文字
//!	@param	[in]	size		文字サイズ(仮) default:64.0f
//-----------------------------------------------------------------------------
void HudObject::RenderString(const FontHundler& hundler, const Vector2& position, const Color& color, const std::wstring& str, f32 size)
{
	if(str.empty())return;
	Hud::RenderStringObject obj;
	obj.Set(hundler, position, color, str, size);
	_strings.SetObject(obj);
}

//-----------------------------------------------------------------------------
//!	テクスチャ描画
//!	@param	[in]	position	座標
//!	@param	[in]	pTexture	テクスチャポインタ
//!	@param	[in]	uv			uv default:(0.0f,0.0f,1.0f,1.0f)
//-----------------------------------------------------------------------------
void HudObject::RenderTexture(const Vector2& position, Texture* pTexture, const Vector4& uv)
{
	if(!pTexture)return;
	Hud::RenderTextureObject obj;
	obj.Set(position, pTexture, uv);
	_textures.SetObject(obj);
}

//-----------------------------------------------------------------------------
//!	四角描画
//!	@param	[in]	position	座標
//!	@param	[in]	size		文字サイズ
//!	@param	[in]	color		色
//-----------------------------------------------------------------------------
void HudObject::RenderRect(const Vector2& position, const Vector2& size, const Color& color)
{
	Hud::RenderRectObject obj;
	obj.Set(position, size, color);
	_rects.SetObject(obj);
}

//-----------------------------------------------------------------------------
//!	フレームバッファ初期化
//!	@param	[in]	width	横
//!	@param	[in]	height	縦
//-----------------------------------------------------------------------------
bool HudObject::InitFrameBuffer(u32 width, u32 height)
{
	_pFrameBuffer = FrameBuffer::Create(width, height, Texture::Param(Texture::FORMAT::RGBA8));
	return _pFrameBuffer != nullptr;
}

//-----------------------------------------------------------------------------
//!	バッファ開始
//-----------------------------------------------------------------------------
bool HudObject::BindFrameBuffer()
{
	if(!_pFrameBuffer)return false;
	_pFrameBuffer->Begin();
	return true;
}

//-----------------------------------------------------------------------------
//!	バッファ終了
//-----------------------------------------------------------------------------
void HudObject::UnBindFrameBuffer()
{
	if(!_pFrameBuffer)return;
	_pFrameBuffer->End();
}

//-----------------------------------------------------------------------------
//!	オブジェクトの描画
//-----------------------------------------------------------------------------
void HudObject::RenderObjects()
{
	_rects.Render();
	_strings.Render();
	_textures.Render();
}

//-----------------------------------------------------------------------------
//!	バッファ描画
//-----------------------------------------------------------------------------
void HudObject::RenderBuffer()
{
	if(!_pFrameBuffer)return;
	Render2D::RenderBuffer(_pFrameBuffer->GetTexture(0));
}




//=============================================================================
//!	カメラオブジェクト
//=============================================================================
//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
CameraObject::CameraObject()
: _fovy				(0)
, _aspect			(0)
, _znear			(0)
, _zfar				(0)
, _width			(0)
, _height			(0)
, _matProj			(Matrix::IDENTITY)
, _matView			(Matrix::IDENTITY)
, _quakeScale		(0.0f)
, _pDeferredRenderer(nullptr)
{
	AddTag("Camera");
}

//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
CameraObject::CameraObject(const CameraObject::Desc& desc)
: _fovy				(desc._fovy)
, _aspect			((f32)desc._width / (f32)desc._height)
, _znear			(desc._znear)
, _zfar				(desc._zfar)
, _width			(desc._width)
, _height			(desc._height)
, _matProj			(Matrix::IDENTITY)
, _matView			(Matrix::IDENTITY)
, _quakeScale		(0.0f)
, _pDeferredRenderer(nullptr)
{
	LookAt(desc._position, desc._lookAt, desc._worldUp);
	AddTag("Camera");
}

//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
CameraObject::~CameraObject()
{
	SafeDelete(_pDeferredRenderer);
	Manager::Camera()->ReleaseCamera(this);
}

//-----------------------------------------------------------------------------
//!	カメラ位置行列
//!	@param	[in]	position	座標
//!	@param	[in]	lookAt		視点
//!	@param	[in]	worldUp		カメラ上の方向	default:(0,1,0)
//-----------------------------------------------------------------------------
void CameraObject::LookAt(const Vector3& position, const Vector3& lookAt, const Vector3& worldUp)
{
	_localPosition	= position;
	_lookAt			= lookAt;
	_worldUp		= worldUp;

	_matView		= Matrix::LookAtViewMatrix(position, lookAt, worldUp);

	_localMatrix	= Matrix::LookAtMatrix(position, lookAt, worldUp);

	_localRotate	= _localMatrix.GetRotateQuaternion();
	_localScale		= Vector3(1.0f,1.0f,1.0f);
}

//-----------------------------------------------------------------------------
//!	透視投影
//!	@param	[in]	fovy	縦の視野角
//!	@param	[in]	width	横
//!	@param	[in]	height	縦
//!	@param	[in]	znear	手前
//!	@param	[in]	zfar	奥行き
//-----------------------------------------------------------------------------
void CameraObject::Perspective(f32 fovy, s32 width, s32 height, f32 znear, f32 zfar)
{
	_fovy	= fovy;
	_width	= width;
	_height = height;
	_aspect = (f32)_width / (f32)_height;
	_znear	= znear;
	_zfar	= zfar;
}

//-----------------------------------------------------------------------------
//!	設定
//!@param	[in]	desc	情報
//-----------------------------------------------------------------------------
void CameraObject::SetCamera(const CameraObject::Desc& desc)
{
	_fovy		= desc._fovy;
	_width		= desc._width;
	_height		= desc._height;
	_aspect		= (f32)_width / (f32)_height;
	_znear		= desc._znear;
	_zfar		= desc._zfar;
	LookAt(desc._position, desc._lookAt, desc._worldUp);

}

//-----------------------------------------------------------------------------
//!	更新
//-----------------------------------------------------------------------------
void CameraObject::Update()
{
	Vector3 quake(0.0f, 0.0f, 0.0f);
	if(_quakeScale > 0.0f)
	{
		quake = Math::RandomVector3();
		quake.Multi(_quakeScale);
		_quakeScale *= 0.8f;
	}
	if(_quakeScale <= 0.2f)
	{
		_quakeScale = 0.0f;
	}
	_matView = Matrix::LookAtViewMatrix(_localPosition + quake, _lookAt + quake, _worldUp);
	_matProj = Matrix::PerspectiveMatrix(_fovy, _aspect, _znear, _zfar);
	MatrixUpdate();

	if(_pDeferredRenderer)
	{
		_pDeferredRenderer->Update();
	}


}

//-----------------------------------------------------------------------------
//!	ディファードレンダリングクラス削除
//-----------------------------------------------------------------------------
void CameraObject::DeleteDeferredRenderer()
{
	SafeDelete(_pDeferredRenderer);
}

//-----------------------------------------------------------------------------
//!	開始
//-----------------------------------------------------------------------------
void CameraObject::Begin()
{
	auto* pRenderer = Manager::Renderer();
	//	今はディファードレンダリングでしか使わない
	pRenderer->EnableDepthTest();
	pRenderer->SetViewport(0, 0, _width, _height);
	pRenderer->ClearColor(0.0f, 1.0f);
	if (_pDeferredRenderer)
	{
		_pDeferredRenderer->BeginGBuffer();
	}
	//	描画設定
	pRenderer->DisableBlending();

	pRenderer->SetProjectionMatrix(_matProj);
	pRenderer->SetViewMatrix(_matView);
}

//-----------------------------------------------------------------------------
//!	終了
//-----------------------------------------------------------------------------
void CameraObject::End()
{
	if(_pDeferredRenderer)
	{
		_pDeferredRenderer->EndGBuffer();
		glDisable(GL_DEPTH_TEST);
		// 光源
		_pDeferredRenderer->Lighting(_matProj,_matView);
		_pDeferredRenderer->Finalize();
	}

}

//-----------------------------------------------------------------------------
//!	バッファ表示
//-----------------------------------------------------------------------------
void CameraObject::RenderBuffer()
{
	if(!_pDeferredRenderer)return;
	_pDeferredRenderer->RenderTexture();
}

//-----------------------------------------------------------------------------
//!	HUD表示
//-----------------------------------------------------------------------------
void CameraObject::RenderHUD()
{
	if(!_pHudObject)return;
	if(!_pHudObject->BindFrameBuffer())return;
	_pHudObject->Render();
	_pHudObject->RenderObjects();
	_pHudObject->UnBindFrameBuffer();

	_pHudObject->RenderBuffer();
}

//-----------------------------------------------------------------------------
//!	バッファ取得
//!	@return	ディファードレンダリングのテクスチャ
//-----------------------------------------------------------------------------
Texture* CameraObject::GetBufferTexture()const
{
	return _pDeferredRenderer->GetBufferTexture();
}

//=============================================================================
//!	カメラ管理クラス
//=============================================================================
//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
CameraManager::CameraManager()
{
}

//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
CameraManager::~CameraManager()
{
}

//-----------------------------------------------------------------------------
//!	カメラ設定
//!	@param	[in]	camera	有効にするカメラ
//-----------------------------------------------------------------------------
void CameraManager::ChangeCamera(CameraObject* camera)
{
	_pRunningCamera = camera;
}

//-----------------------------------------------------------------------------
//!	カメラ解放
//!	@param	[in]	解放するカメラ
//-----------------------------------------------------------------------------
void CameraManager::ReleaseCamera(CameraObject* pCamera)
{
	if(_pRunningCamera == pCamera)
	{
		_pRunningCamera = nullptr;
	}
}
