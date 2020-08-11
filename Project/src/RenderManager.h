//=============================================================================
//!
//!	@file	Renderer.h
//!	@brief	�`��Ǘ��N���X
//!			�`����Ǘ�����N���X�B�V�F�[�_�[�̗L���A�r���[�|�[�g�̐ݒ�Ȃ�
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//	http://wisdom.sakura.ne.jp/system/opengl/gl17.html

enum class BLEND_MODE
{
	ZERO				= GL_ZERO,					//	(0, 0, 0, 0)
	ONE					= GL_ONE,					//	(1, 1, 1, 1)
	SRC_COLOR			= GL_SRC_COLOR,				//	(Rs / kR, Gs / kG, Bs / kB, As / kA)
	ONE_MINUS_SRC_COLOR	= GL_ONE_MINUS_SRC_COLOR,	//	(1, 1, 1, 1) - (Rs / kR, Gs / kG, Bs / kB, As / kA)
	DST_COLOR			= GL_DST_COLOR,				//	(Rd / kR, Gd / kG, Bd / kB, Ad / kA)
	ONE_MINUS_DST_COLOR	= GL_ONE_MINUS_DST_COLOR,	//	(1, 1, 1, 1) - (Rd / kR, Gd / kG, Bd / kB, Ad / kA)
	SRC_ALPHA			= GL_SRC_ALPHA,				//	(As / kA, As / kA, As / kA, As / kA)
	ONE_MINUS_SRC_ALPHA	= GL_ONE_MINUS_SRC_ALPHA,	//	(1, 1, 1, 1) - (As / kA, As / kA, As / kA, As / kA)
	DST_ALPHA			= GL_DST_ALPHA,				//	(Ad / kA, Ad / kA, Ad / kA, Ad / kA)
	ONE_MINUS_DST_ALPHA	= GL_ONE_MINUS_DST_ALPHA,	//	(1, 1, 1, 1) - (Ad / kA, Ad / kA, Ad / kA, Ad / kA)
	SRC_ALPHA_SATURATE	= GL_SRC_ALPHA_SATURATE,	//	(i, i, i, 1)
};

enum class MATRIX_MODE
{
	PROJECTION	= GL_PROJECTION,
	MODELVIEW	= GL_MODELVIEW,
};

struct Viewport
{
	s32 x;
	s32 y;
	s32 width;
	s32 height;
};

class RenderManager : public ManagerBase
{
public:
	//!	�R���X�g���N�^
	RenderManager();
	//!	�f�X�g���N�^
	virtual ~RenderManager();
public:
	//!	������
	void Init();
	//!	�X�V
	void Update();
	//!	�`��
	void Render();
	//!	�I��
	void Exit();

	//!	�f�v�X�L��
	void EnableDepthTest();
	//!	�f�v�X����
	void DisableDepthTest();

	//!	�A���t�@�L��
	void EnableAlphaBlending(BLEND_MODE sfactor = BLEND_MODE::SRC_ALPHA, BLEND_MODE dfactor = BLEND_MODE::ONE_MINUS_SRC_ALPHA);
	//!	�A���t�@����
	void DisableAlphaBlending();

	//!	�u�����h�L��
	void EnableBlending(BLEND_MODE sfactor, BLEND_MODE dfactor);
	//!	�u�����h����
	void DisableBlending();


	//!	�V�F�[�_�[�L��
	void EnableShader()	{ _shaderActive = true; }
	//!	�V�F�[�_�[����
	void DisableShader(){ _shaderActive = false; }

	//!	�u�����h
	void BlendFunc(BLEND_MODE sfactor, BLEND_MODE dfactor);

	//!	�s��ݒ�
	void MatrixMode(MATRIX_MODE mode);

	void ClearColor(const Color& color);
	void ClearColor(f32 r, f32 g, f32 b, f32 a = 1.0f);
	void ClearColor(f32 col, f32 a = 1.0f);


	void PushMatrix();
	void PopMatrix();

	//!	�r���[�|�[�g�ݒ�
	void SetViewport(s32 x, s32 y, s32 width, s32 height);
	//!	�r���[�|�[�g�ݒ�
	void SetViewport(const Viewport& viewport);

	//!	�r���[�|�[�g�擾
	Viewport GetViewport();

	//!	�v���W�F�N�V�����s��
	void SetProjectionMatrix(const Matrix& matProj);
	//!	�r���[�s��
	void SetViewMatrix(const Matrix& matView);
	//!	���[���h�s��
	void SetWorldMatrix(const Matrix& matWorld);

	//!	�v���W�F�N�V�����s��
	const Matrix& GetProjectionMatrix()const{ return _matProj; }
	//!	�r���[�s��
	const Matrix& GetViewMatrix()const		{ return _matView; }
	//!	���[���h�s��
	const Matrix& GetWorldMatrix()const		{ return _matWorld; }
	//!	�F
	void SetMaterialColor(const Color& color);

private:
	bool	_shaderActive;	//!< �V�F�[�_�[�t���O
	Matrix	_matProj;
	Matrix	_matView;
	Matrix	_matWorld;


	//BufferObject::Vao*	_pSprite2dVao;		//!< 2DVao	�Ƃ肠���������ɍ��
	//ShaderHundler		_texture2dShader;
};

MANAGER_INTERFACE(RenderManager, Renderer);

