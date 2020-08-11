//=============================================================================
//!
//!	@file	Camera.h
//!	@brief	�J�����N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once


class BaseDeferredRenderer;


//-------------------------------------------------------------------
//!	Hud�n�I�u�W�F�N�g
//-------------------------------------------------------------------
namespace Hud
{
//-------------------------------------------------------------------
//!	�`��I�u�W�F�N�g�x�[�X
//-------------------------------------------------------------------
class RenderBase
{
public:
	//!	�R���X�g���N�^
	RenderBase() :_position(Vector2::ZERO){}
	//!	�R���X�g���N�^
	//!	@param	[in]	position	���W
	RenderBase(const Vector2 position) :_position(position){}
	//!	�f�X�g���N�^
	virtual ~RenderBase(){}

	//!	�`��
	virtual void Render(){}
protected:
	Vector2 _position;	//!< ���W
};

//-------------------------------------------------------------------
//!	������`��I�u�W�F�N�g
//-------------------------------------------------------------------
class RenderStringObject : public RenderBase
{
public:
	//!	�R���X�g���N�^
	RenderStringObject()
	: _str(L"")
	, _size(32.f)
	{}
	//!	�f�X�g���N�^
	virtual ~RenderStringObject(){}

	//!	�ݒ�
	//!	@param	[in]	hundler		�t�H���g�f�[�^
	//!	@param	[in]	position	���W
	//!	@param	[in]	color		�F
	//!	@param	[in]	str			�`�敶��
	//!	@param	[in]	size		�����T�C�Y(��) default:64.0f
	void Set(const FontHundler& hundler, const Vector2& position, const Color& color, const std::wstring& str, f32 size = 64.f);

	//!	�`��
	void Render();
private:
	FontHundler		_fontHundler;	//!< �t�H���g�f�[�^
	std::wstring	_str;			//!< ������
	Color			_color;			//!< �F
	f32				_size;			//!< �`��T�C�Y(��)
};

//-------------------------------------------------------------------
//!	�e�N�X�`���`��I�u�W�F�N�g
//-------------------------------------------------------------------
class RenderTextureObject : public RenderBase
{
public:
	//!	�R���X�g���N�^
	RenderTextureObject()
	: _pTexture(nullptr)
	, _uv(Vector4(0.0f, 0.0f, 1.0f, 1.0f))
	, _size(Vector2::ONE)
	{}
	//!	�f�X�g���N�^
	virtual ~RenderTextureObject(){}

	//!	�ݒ�
	//!	@param	[in]	position	���W
	//!	@param	[in]	pTexture	�e�N�X�`���|�C���^
	//!	@param	[in]	uv			uv default:(0.0f,0.0f,1.0f,1.0f)
	void Set(const Vector2& position, Texture* pTexture, const Vector4& uv = Vector4(0.0f, 0.0f, 1.0f, 1.0f));

	//!	�`��
	void Render();
private:
	Texture* _pTexture;	//!< �\���e�N�X�`��
	Vector4 _uv;		//!< UV
	Vector2 _size;		//!< �`��T�C�Y
};

//-------------------------------------------------------------------
//!	�e�N�X�`���`��I�u�W�F�N�g
//-------------------------------------------------------------------
class RenderRectObject : public RenderBase
{
public:
	//!	�R���X�g���N�^
	RenderRectObject()
	: _size(Vector2::ONE)
	, _color(255)
	{}
	//!	�f�X�g���N�^
	virtual ~RenderRectObject(){}

	//!	�ݒ�
	//!	@param	[in]	position	���W
	//!	@param	[in]	size		�����T�C�Y
	//!	@param	[in]	color		�F
	void Set(const Vector2& position, const Vector2& size, const Color& color);

	//!	�`��
	void Render();
private:
	Vector2	_size;		//!< �`��T�C�Y
	Color	_color;		//!< �F
};


//-------------------------------------------------------------------
//!	�`��I�u�W�F�N�g�Ǘ��N���X
//!	@tparam	T	�I�u�W�F�N�g�^
//!	@param		MAX_SIZE	�I�u�W�F�N�g��
//-------------------------------------------------------------------
template <class T, s32 MAX_SIZE>
class RenderObjects
{
public:
	//!	�R���X�g���N�^
	RenderObjects()
	:_arrayCount(0)
	{
	}
	//!	�f�X�g���N�^
	virtual ~RenderObjects()
	{
	}

	//!	�I�u�W�F�N�g�ݒ�
	//!	@param	[in]	val	�I�u�W�F�N�g
	void SetObject(const T& val)
	{
		if (_arrayCount >= MAX_SIZE)return;
		u32 index = _arrayCount;
		++_arrayCount;
		_renderArray[index] = val;
	}

	//!	�`��
	void Render()
	{
		for (u32 i = 0; i < _arrayCount; ++i)
		{
			_renderArray[i].Render();
		}
		_arrayCount = 0;
	}
private:
	std::array<T, MAX_SIZE> _renderArray;	//!< �I�u�W�F�N�g
	u32 _arrayCount;						//!< �I�u�W�F�N�g��
};

};	//	namespace Hud




static const u32 RENDER_ARRAY_MAX = 64;

//-------------------------------------------------------------------
//!	HUD�I�u�W�F�N�g �I�[�o�[���C�h��
//-------------------------------------------------------------------
class HudObject : public BasicBase
{
	friend class CameraObject;
public:
	//!	�R���X�g���N�^
	HudObject()
	: _pFrameBuffer(nullptr)
	{
	}
	//!	�f�X�g���N�^
	virtual ~HudObject()
	{
		SafeDelete(_pFrameBuffer);
	}

	//!	�����`��
	//!	@param	[in]	hundler		�t�H���g�f�[�^
	//!	@param	[in]	position	���W
	//!	@param	[in]	color		�F
	//!	@param	[in]	str			�`�敶��
	//!	@param	[in]	size		�����T�C�Y(��) default:64.0f
	void RenderString(const FontHundler& hundler, const Vector2& position, const Color& color, const std::wstring& str, f32 size = 64.f);
	//!	�e�N�X�`���`��
	//!	@param	[in]	position	���W
	//!	@param	[in]	pTexture	�e�N�X�`���|�C���^
	//!	@param	[in]	uv			uv default:(0.0f,0.0f,1.0f,1.0f)
	void RenderTexture(const Vector2& position, Texture* pTexture, const Vector4& uv = Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	//!	�l�p�`��
	//!	@param	[in]	position	���W
	//!	@param	[in]	size		�����T�C�Y
	//!	@param	[in]	color		�F
	void RenderRect(const Vector2& position, const Vector2& size, const Color& color);

protected:
	//!	�t���[���o�b�t�@������
	//!	@param	[in]	width	��
	//!	@param	[in]	height	�c
	bool InitFrameBuffer(u32 width, u32 height);

	//!	�o�b�t�@�J�n
	bool BindFrameBuffer();
	//!	�o�b�t�@�I��
	void UnBindFrameBuffer();

	//!	�I�u�W�F�N�g�̕`��
	void RenderObjects();
	//!	�o�b�t�@�`��
	void RenderBuffer();
private:
	class FrameBuffer* _pFrameBuffer;	//!< �t���[���o�b�t�@

	Hud::RenderObjects<Hud::RenderRectObject,	 RENDER_ARRAY_MAX> _rects;		//!< �l�p
	Hud::RenderObjects<Hud::RenderStringObject,  RENDER_ARRAY_MAX> _strings;	//!< ����
	Hud::RenderObjects<Hud::RenderTextureObject, RENDER_ARRAY_MAX> _textures;	//!< �e�N�X�`��
};

//-----------------------------------------------------------------------------
//!	�J�����I�u�W�F�N�g
//-----------------------------------------------------------------------------
class CameraObject : public GameObjectBase
{
public:
	friend class CameraManager;
	struct Desc
	{
		//!	�J�����ʒu�s��
		//!	@param	[in]	position	���W
		//!	@param	[in]	lookAt		���_
		//!	@param	[in]	worldUp		�J������̕���	default:(0,1,0)
		void LookAt(const Vector3& position, const Vector3& lookAt, const Vector3& worldUp = Vector3(0, 1, 0))
		{
			_position	= position;
			_lookAt		= lookAt;
			_worldUp	= worldUp;
		}

		//!	�������e
		//!	@param	[in]	fovy	�c�̎���p
		//!	@param	[in]	width	��
		//!	@param	[in]	height	�c
		//!	@param	[in]	znear	��O
		//!	@param	[in]	zfar	���s��
		void Perspective(f32 fovy, s32 width, s32 height, f32 znear, f32 zfar)
		{
			_fovy	= fovy;
			_width	= width;
			_height	= height;
			_znear	= znear;
			_zfar	= zfar;
		}

		Vector3	_position;		//!< �J�������W
		Vector3	_lookAt;		//!< ���_
		Vector3	_worldUp;		//!< �J���������
		f32		_fovy;			//!< �c����p
		s32		_width;			//!< ��
		s32		_height;		//!< �c
		f32		_znear;			//!< ��O
		f32		_zfar;			//!< ���s��
	};

	//!	�R���X�g���N�^
	CameraObject();
	//!	�R���X�g���N�^
	CameraObject(const Desc& desc);
	//!	�f�X�g���N�^
	virtual ~CameraObject();

public:
	//!	�X�V
	void Update();

	//!	�f�B�t�@�[�h�����_�����O�N���X�쐬
	//!	@tparam	T		DeferredRenderer�N���X
	//!	@retval	true	����
	//!	@retval	false	���s
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

	//!	�f�B�t�@�[�h�����_�����O�N���X�쐬
	//!	@tparam	T		DeferredRenderer�N���X
	//!	@retval	true	����
	//!	@retval	false	���s
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

	//!	Hud������
	//!	@tparam	T		Hud�I�u�W�F�N�g default:HudObject
	//!	@retval	true	����
	//!	@retval	false	���s
	template<typename T = HudObject>
	bool InitHUDObject()
	{
		_pHudObject = CreateRefPointer<T>();
		if(!_pHudObject)return false;
		return _pHudObject->InitFrameBuffer(_width, _height);
	}

	//!	�f�B�t�@�[�h�����_�����O�N���X�폜
	void DeleteDeferredRenderer();

	//!	�J�n
	void Begin();
	//!	�I��
	void End();

	//!	�o�b�t�@�\��
	void RenderBuffer();
	//!	HUD�\��
	void RenderHUD();

	//--------------------------------------------------------------
	//! @name    �擾�֐�
	//--------------------------------------------------------------
	//@{
	//!	�J�����̌����擾
	//!	@return	�J�����̃x�N�g��
	Vector3 GetCameraVector()const
	{
		return Vector3(_matView[2].x, _matView[2].y, _matView[2].z);
	}

	//!	�c����p
	//!	@return	�c����p
	f32 GetFovy()const
	{
		return _fovy;
	}
	//!	��ʔ䗦
	//!	@return	��ʔ䗦
	f32 GetAspect()const
	{
		return _aspect;
	}
	//!	��
	//!	@return ���T�C�Y
	s32 GetWidth()const
	{
		return _width;
	}
	//!	�c
	//!	@return	�c�T�C�Y
	s32 GetHeight()const
	{
		return _height;
	}
	//!	��O
	//!	@return	��O
	f32 GetZnear()const
	{
		return _znear;
	}
	//!	���s��
	//!	@return	���s��
	f32 GetZfar()const
	{
		return _zfar;
	}

	//!	�r���[�s��
	//!	@return	�r���[�s��
	const Matrix& GetViewMatrix() const
	{
		return _matView;
	}
	//!	�v���W�F�N�V�����s��
	//!	@return	�v���W�F�N�V�����s��
	const Matrix& GetProjectionMatrix() const
	{
		return _matProj;
	}

	//!	�f�B�t�@�[�h�����_�����O
	//!	@return	�f�B�t�@�[�h�����_�����O�I�u�W�F�N�g�̃|�C���^
	BaseDeferredRenderer* GetDeferredRenderer()const
	{
		return _pDeferredRenderer;
	}

	//!	�o�b�t�@�擾
	//!	@return	�f�B�t�@�[�h�����_�����O�̃e�N�X�`��
	Texture* GetBufferTexture()const;

	//!	Hud�I�u�W�F�N�g
	//!	@return	Hud�I�u�W�F�N�g�̃|�C���^
	HudObject* GetHud()
	{
		return _pHudObject.get();
	}

	//@}

	//--------------------------------------------------------------
	//! @name    �ݒ�֐�
	//--------------------------------------------------------------
	//@{
	//!	�J�����ʒu�s��
	//!	@param	[in]	position	���W
	//!	@param	[in]	lookAt		���_
	//!	@param	[in]	worldUp		�J������̕���	default:(0,1,0)
	void LookAt(const Vector3& position, const Vector3& lookAt, const Vector3& worldUp = Vector3(0, 1, 0));

	//!	�������e
	//!	@param	[in]	fovy	�c�̎���p
	//!	@param	[in]	width	��
	//!	@param	[in]	height	�c
	//!	@param	[in]	znear	��O
	//!	@param	[in]	zfar	���s��
	void Perspective(f32 fovy, s32 width, s32 height, f32 znear, f32 zfar);

	//!	�ݒ�
	//!@param	[in]	desc	���
	void SetCamera(const CameraObject::Desc& desc);

	//!	�c����p
	//!	@param	[in]	fovy	�c����p
	void SetFovy(f32 fovy)
	{
		_fovy = fovy;
	}
	//!	��ʔ䗦
	//!	@param	[in]	width	��
	//!	@param	[in]	height	�c
	void SetAspect(s32 width, s32 height)
	{
		_width = width;
		_height = height;
		_aspect = (f32)_width / (f32)_height;
	}
	//!	��
	//!	@param	[in]	width	��
	void SetWidth(s32 width)
	{
		_width = width;
		_aspect = (f32)_width / (f32)_height;
	}
	//!	�c
	//!	@param	[in]	height	�c
	void SetHeight(s32 height)
	{
		_height = height;
		_aspect = (f32)_width / (f32)_height;
	}
	//!	��O
	//!	@param	[in]	znear	��O
	void SetZnear(f32 znear)
	{
		_znear = znear;
	}
	//!	���s��
	//!	@param	[in]	zfar	���s��
	void SetZfar(f32 zfar)
	{
		_zfar = zfar;
	}

	//!	�J�����U��
	//!	@param	[in]	quakeScale	�J�����U���X�P�[��
	void SetQuakeScale(f32 quakeScale)
	{
		_quakeScale = quakeScale;
	}

	//@}

private:
	Vector3	_lookAt;		//!< ���_
	Vector3	_worldUp;		//!< �J���������

	f32		_fovy;			//!< �c����p
	f32		_aspect;		//!< ������p
	f32		_znear;			//!< ��O
	f32		_zfar;			//!< ���s��

	s32		_width;			//!< ��
	s32		_height;		//!< �c

	Matrix	_matProj;		//!< �v���W�F�N�V�����s��
	Matrix	_matView;		//!< �r���[�s��
	f32		_quakeScale;	//!< �J�����U��

	BaseDeferredRenderer*	_pDeferredRenderer;	//!< �f�B�t�@�[�h�����_�����O�I�u�W�F�N�g
	RefPointer<HudObject>	_pHudObject;		//!< Hud�I�u�W�F�N�g

};


//-----------------------------------------------------------------------------
//!	�J�����Ǘ��N���X
//-----------------------------------------------------------------------------
class CameraManager : public ManagerBase
{
public:
	friend CameraObject;
public:
	//friend Camera;
	//!	�R���X�g���N�^
	CameraManager();
	//!	�f�X�g���N�^
	virtual ~CameraManager();

	//!	�J�����ݒ�
	//!	@param	[in]	camera	�L���ɂ���J����
	void ChangeCamera(CameraObject* camera);

	//!	�L���ɂȂ��Ă�J����
	//!	@return	�L���ȃJ�����̃|�C���^
	CameraObject* GetRunningCamera()
	{
		return _pRunningCamera;
	}


private:
	//!	�J�������
	//!	�����ʂ���_pRunningCamera�̃`�F�b�N������
	//!	@param	[in]	�������J����
	void ReleaseCamera(CameraObject* pCamera);
private:
	CameraObject* _pRunningCamera;		//!< ���ݓ����Ă�J����
};

MANAGER_INTERFACE(CameraManager, Camera);
