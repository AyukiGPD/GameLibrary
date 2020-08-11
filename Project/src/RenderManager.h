//=============================================================================
//!
//!	@file	Renderer.h
//!	@brief	描画管理クラス
//!			描画を管理するクラス。シェーダーの有無、ビューポートの設定など
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
	//!	コンストラクタ
	RenderManager();
	//!	デストラクタ
	virtual ~RenderManager();
public:
	//!	初期化
	void Init();
	//!	更新
	void Update();
	//!	描画
	void Render();
	//!	終了
	void Exit();

	//!	デプス有効
	void EnableDepthTest();
	//!	デプス無効
	void DisableDepthTest();

	//!	アルファ有効
	void EnableAlphaBlending(BLEND_MODE sfactor = BLEND_MODE::SRC_ALPHA, BLEND_MODE dfactor = BLEND_MODE::ONE_MINUS_SRC_ALPHA);
	//!	アルファ無効
	void DisableAlphaBlending();

	//!	ブレンド有効
	void EnableBlending(BLEND_MODE sfactor, BLEND_MODE dfactor);
	//!	ブレンド無効
	void DisableBlending();


	//!	シェーダー有効
	void EnableShader()	{ _shaderActive = true; }
	//!	シェーダー無効
	void DisableShader(){ _shaderActive = false; }

	//!	ブレンド
	void BlendFunc(BLEND_MODE sfactor, BLEND_MODE dfactor);

	//!	行列設定
	void MatrixMode(MATRIX_MODE mode);

	void ClearColor(const Color& color);
	void ClearColor(f32 r, f32 g, f32 b, f32 a = 1.0f);
	void ClearColor(f32 col, f32 a = 1.0f);


	void PushMatrix();
	void PopMatrix();

	//!	ビューポート設定
	void SetViewport(s32 x, s32 y, s32 width, s32 height);
	//!	ビューポート設定
	void SetViewport(const Viewport& viewport);

	//!	ビューポート取得
	Viewport GetViewport();

	//!	プロジェクション行列
	void SetProjectionMatrix(const Matrix& matProj);
	//!	ビュー行列
	void SetViewMatrix(const Matrix& matView);
	//!	ワールド行列
	void SetWorldMatrix(const Matrix& matWorld);

	//!	プロジェクション行列
	const Matrix& GetProjectionMatrix()const{ return _matProj; }
	//!	ビュー行列
	const Matrix& GetViewMatrix()const		{ return _matView; }
	//!	ワールド行列
	const Matrix& GetWorldMatrix()const		{ return _matWorld; }
	//!	色
	void SetMaterialColor(const Color& color);

private:
	bool	_shaderActive;	//!< シェーダーフラグ
	Matrix	_matProj;
	Matrix	_matView;
	Matrix	_matWorld;


	//BufferObject::Vao*	_pSprite2dVao;		//!< 2DVao	とりあえずここに作る
	//ShaderHundler		_texture2dShader;
};

MANAGER_INTERFACE(RenderManager, Renderer);

