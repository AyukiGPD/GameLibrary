//=============================================================================
//!
//!	@file	Camera.cpp
//!	@brief	�J�����N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

namespace Hud
{

//-----------------------------------------------------------------------------
//!	�ݒ�
//!	@param	[in]	hundler		�t�H���g�f�[�^
//!	@param	[in]	position	���W
//!	@param	[in]	color		�F
//!	@param	[in]	str			�`�敶��
//!	@param	[in]	size		�����T�C�Y(��) default:64.0f
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
//!	�`��
//-----------------------------------------------------------------------------
void RenderStringObject::Render()
{
	if(_str.empty())return;

	if(!_fontHundler)return;

	SetColor(_color);
	_fontHundler.Render(_str, _position);
}

//-----------------------------------------------------------------------------
//!	�ݒ�
//!	@param	[in]	position	���W
//!	@param	[in]	pTexture	�e�N�X�`���|�C���^
//!	@param	[in]	uv			uv default:(0.0f,0.0f,1.0f,1.0f)
//-----------------------------------------------------------------------------
void RenderTextureObject::Set(const Vector2& position, Texture* pTexture, const Vector4& uv)
{
	_position = position;
	_pTexture = pTexture;
	_uv = uv;
}

//-----------------------------------------------------------------------------
//!	�`��
//-----------------------------------------------------------------------------
void RenderTextureObject::Render()
{
	if(!_pTexture)return;

	//	������
}

//-----------------------------------------------------------------------------
//!	�ݒ�
//!	@param	[in]	position	���W
//!	@param	[in]	size		�����T�C�Y
//!	@param	[in]	color		�F
//-----------------------------------------------------------------------------
void RenderRectObject::Set(const Vector2& position, const Vector2& size, const Color& color)
{
	_position = position;
	_size = size;
	_color = color;
}

//-----------------------------------------------------------------------------
//!	�`��
//-----------------------------------------------------------------------------
void RenderRectObject::Render()
{
	Render2D::Rect(_position, _size, _color);
}



}	//	namespace Hud

//-----------------------------------------------------------------------------
//!	�����`��
//!	@param	[in]	hundler		�t�H���g�f�[�^
//!	@param	[in]	position	���W
//!	@param	[in]	color		�F
//!	@param	[in]	str			�`�敶��
//!	@param	[in]	size		�����T�C�Y(��) default:64.0f
//-----------------------------------------------------------------------------
void HudObject::RenderString(const FontHundler& hundler, const Vector2& position, const Color& color, const std::wstring& str, f32 size)
{
	if(str.empty())return;
	Hud::RenderStringObject obj;
	obj.Set(hundler, position, color, str, size);
	_strings.SetObject(obj);
}

//-----------------------------------------------------------------------------
//!	�e�N�X�`���`��
//!	@param	[in]	position	���W
//!	@param	[in]	pTexture	�e�N�X�`���|�C���^
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
//!	�l�p�`��
//!	@param	[in]	position	���W
//!	@param	[in]	size		�����T�C�Y
//!	@param	[in]	color		�F
//-----------------------------------------------------------------------------
void HudObject::RenderRect(const Vector2& position, const Vector2& size, const Color& color)
{
	Hud::RenderRectObject obj;
	obj.Set(position, size, color);
	_rects.SetObject(obj);
}

//-----------------------------------------------------------------------------
//!	�t���[���o�b�t�@������
//!	@param	[in]	width	��
//!	@param	[in]	height	�c
//-----------------------------------------------------------------------------
bool HudObject::InitFrameBuffer(u32 width, u32 height)
{
	_pFrameBuffer = FrameBuffer::Create(width, height, Texture::Param(Texture::FORMAT::RGBA8));
	return _pFrameBuffer != nullptr;
}

//-----------------------------------------------------------------------------
//!	�o�b�t�@�J�n
//-----------------------------------------------------------------------------
bool HudObject::BindFrameBuffer()
{
	if(!_pFrameBuffer)return false;
	_pFrameBuffer->Begin();
	return true;
}

//-----------------------------------------------------------------------------
//!	�o�b�t�@�I��
//-----------------------------------------------------------------------------
void HudObject::UnBindFrameBuffer()
{
	if(!_pFrameBuffer)return;
	_pFrameBuffer->End();
}

//-----------------------------------------------------------------------------
//!	�I�u�W�F�N�g�̕`��
//-----------------------------------------------------------------------------
void HudObject::RenderObjects()
{
	_rects.Render();
	_strings.Render();
	_textures.Render();
}

//-----------------------------------------------------------------------------
//!	�o�b�t�@�`��
//-----------------------------------------------------------------------------
void HudObject::RenderBuffer()
{
	if(!_pFrameBuffer)return;
	Render2D::RenderBuffer(_pFrameBuffer->GetTexture(0));
}




//=============================================================================
//!	�J�����I�u�W�F�N�g
//=============================================================================
//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
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
//!	�R���X�g���N�^
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
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
CameraObject::~CameraObject()
{
	SafeDelete(_pDeferredRenderer);
	Manager::Camera()->ReleaseCamera(this);
}

//-----------------------------------------------------------------------------
//!	�J�����ʒu�s��
//!	@param	[in]	position	���W
//!	@param	[in]	lookAt		���_
//!	@param	[in]	worldUp		�J������̕���	default:(0,1,0)
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
//!	�������e
//!	@param	[in]	fovy	�c�̎���p
//!	@param	[in]	width	��
//!	@param	[in]	height	�c
//!	@param	[in]	znear	��O
//!	@param	[in]	zfar	���s��
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
//!	�ݒ�
//!@param	[in]	desc	���
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
//!	�X�V
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
//!	�f�B�t�@�[�h�����_�����O�N���X�폜
//-----------------------------------------------------------------------------
void CameraObject::DeleteDeferredRenderer()
{
	SafeDelete(_pDeferredRenderer);
}

//-----------------------------------------------------------------------------
//!	�J�n
//-----------------------------------------------------------------------------
void CameraObject::Begin()
{
	auto* pRenderer = Manager::Renderer();
	//	���̓f�B�t�@�[�h�����_�����O�ł����g��Ȃ�
	pRenderer->EnableDepthTest();
	pRenderer->SetViewport(0, 0, _width, _height);
	pRenderer->ClearColor(0.0f, 1.0f);
	if (_pDeferredRenderer)
	{
		_pDeferredRenderer->BeginGBuffer();
	}
	//	�`��ݒ�
	pRenderer->DisableBlending();

	pRenderer->SetProjectionMatrix(_matProj);
	pRenderer->SetViewMatrix(_matView);
}

//-----------------------------------------------------------------------------
//!	�I��
//-----------------------------------------------------------------------------
void CameraObject::End()
{
	if(_pDeferredRenderer)
	{
		_pDeferredRenderer->EndGBuffer();
		glDisable(GL_DEPTH_TEST);
		// ����
		_pDeferredRenderer->Lighting(_matProj,_matView);
		_pDeferredRenderer->Finalize();
	}

}

//-----------------------------------------------------------------------------
//!	�o�b�t�@�\��
//-----------------------------------------------------------------------------
void CameraObject::RenderBuffer()
{
	if(!_pDeferredRenderer)return;
	_pDeferredRenderer->RenderTexture();
}

//-----------------------------------------------------------------------------
//!	HUD�\��
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
//!	�o�b�t�@�擾
//!	@return	�f�B�t�@�[�h�����_�����O�̃e�N�X�`��
//-----------------------------------------------------------------------------
Texture* CameraObject::GetBufferTexture()const
{
	return _pDeferredRenderer->GetBufferTexture();
}

//=============================================================================
//!	�J�����Ǘ��N���X
//=============================================================================
//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
CameraManager::CameraManager()
{
}

//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
CameraManager::~CameraManager()
{
}

//-----------------------------------------------------------------------------
//!	�J�����ݒ�
//!	@param	[in]	camera	�L���ɂ���J����
//-----------------------------------------------------------------------------
void CameraManager::ChangeCamera(CameraObject* camera)
{
	_pRunningCamera = camera;
}

//-----------------------------------------------------------------------------
//!	�J�������
//!	@param	[in]	�������J����
//-----------------------------------------------------------------------------
void CameraManager::ReleaseCamera(CameraObject* pCamera)
{
	if(_pRunningCamera == pCamera)
	{
		_pRunningCamera = nullptr;
	}
}
