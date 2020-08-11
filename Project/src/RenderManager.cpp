//=============================================================================
//!
//!	@file	Renderer.cpp
//!	@brief	描画管理クラス
//!			描画を管理するクラス。シェーダーの有無、ビューポートの設定など
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
RenderManager::RenderManager()
: _shaderActive(false)
, _matProj(Matrix::IDENTITY)
, _matView(Matrix::IDENTITY)
{
}

//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
RenderManager::~RenderManager()
{
}


//-----------------------------------------------------------------------------
//!	初期化
//-----------------------------------------------------------------------------
void RenderManager::Init()
{
}
//-----------------------------------------------------------------------------
//!	更新
//-----------------------------------------------------------------------------
void RenderManager::Update()
{
}
//-----------------------------------------------------------------------------
//!	描画
//-----------------------------------------------------------------------------
void RenderManager::Render()
{
}
//-----------------------------------------------------------------------------
//!	終了
//-----------------------------------------------------------------------------
void RenderManager::Exit()
{
}

//-----------------------------------------------------------------------------
//!	デプス有効
//-----------------------------------------------------------------------------
void RenderManager::EnableDepthTest()
{
	glEnable(GL_DEPTH_TEST);
}
//-----------------------------------------------------------------------------
//!	デプス無効
//-----------------------------------------------------------------------------
void RenderManager::DisableDepthTest()
{
	glDisable(GL_DEPTH_TEST);
}

//-----------------------------------------------------------------------------
//!	アルファ有効
//-----------------------------------------------------------------------------
void RenderManager::EnableAlphaBlending(BLEND_MODE sfactor, BLEND_MODE dfactor)
{
	//	アルファ有効化
	glEnable(GL_BLEND);
	BlendFunc(sfactor, dfactor);
}
//-----------------------------------------------------------------------------
//!	アルファ無効
//-----------------------------------------------------------------------------
void RenderManager::DisableAlphaBlending()
{
	glDisable(GL_BLEND);
}

//-----------------------------------------------------------------------------
//!	ブレンド有効
//-----------------------------------------------------------------------------
void RenderManager::EnableBlending(BLEND_MODE sfactor, BLEND_MODE dfactor)
{
	glEnable(GL_BLEND);
	BlendFunc(sfactor, dfactor);
}
//-----------------------------------------------------------------------------
//!	ブレンド無効
//-----------------------------------------------------------------------------
void RenderManager::DisableBlending()
{
	glDisable(GL_BLEND);
}


//-----------------------------------------------------------------------------
//!	ブレンド
//-----------------------------------------------------------------------------
void RenderManager::BlendFunc(BLEND_MODE sfactor, BLEND_MODE dfactor)
{
	glBlendFunc((GLenum)sfactor, (GLenum)dfactor);
}

//-----------------------------------------------------------------------------
//!	行列設定
//-----------------------------------------------------------------------------
void RenderManager::MatrixMode(MATRIX_MODE mode)
{
	glMatrixMode((GLenum)mode);
}

void RenderManager::ClearColor(const Color& color)
{
	Vector4 c = color.GetColorF();
	glClearColor(c.x, c.y, c.z, c.w);
}
void RenderManager::ClearColor(f32 r, f32 g, f32 b, f32 a)
{
	glClearColor(r, g, b, a);
}
void RenderManager::ClearColor(f32 col, f32 a)
{
	glClearColor(col, col, col, a);
}


//-----------------------------------------------------------------------------
//!	行列保存
//-----------------------------------------------------------------------------
void RenderManager::PushMatrix()
{
	glPushMatrix();
}
//-----------------------------------------------------------------------------
//!	行列復元
//-----------------------------------------------------------------------------
void RenderManager::PopMatrix()
{
	glPopMatrix();
}

//-----------------------------------------------------------------------------
//!	ビューポート設定
//-----------------------------------------------------------------------------
void RenderManager::SetViewport(s32 x, s32 y, s32 width, s32 height)
{
	glViewport(x, y, width, height);
}
//-----------------------------------------------------------------------------
//!	ビューポート設定
//-----------------------------------------------------------------------------
void RenderManager::SetViewport(const Viewport& viewport)
{
	SetViewport(viewport.x, viewport.y, viewport.width, viewport.height);
}

//-----------------------------------------------------------------------------
//!	ビューポート取得
//-----------------------------------------------------------------------------
Viewport RenderManager::GetViewport()
{
	Viewport result;
	glGetIntegerv(GL_VIEWPORT, (GLint*)&result);
	return result;
}

//-----------------------------------------------------------------------------
//!	プロジェクション行列
//-----------------------------------------------------------------------------
void RenderManager::SetProjectionMatrix(const Matrix& matProj)
{
	if(Manager::Shader()->IsRunShader())
	{
		ShaderManager* pShader = Manager::Shader();
		pShader->SetUniform(0, matProj);
	}
	else
	{
		MatrixMode(MATRIX_MODE::PROJECTION);
		LoadMatrix(matProj);
	}
	_matProj = matProj;
}
//-----------------------------------------------------------------------------
//!	ビュー行列
//-----------------------------------------------------------------------------
void RenderManager::SetViewMatrix(const Matrix& matView)
{
	if(Manager::Shader()->IsRunShader())
	{
		ShaderManager* pShader = Manager::Shader();
		pShader->SetUniform(1, matView);
	}
	else
	{
		MatrixMode(MATRIX_MODE::MODELVIEW);
		LoadMatrix(matView);
	}
	_matView = matView;
}

//-----------------------------------------------------------------------------
//!	ワールド行列
//-----------------------------------------------------------------------------
void RenderManager::SetWorldMatrix(const Matrix& matWorld)
{
	if(Manager::Shader()->IsRunShader())
	{
		ShaderManager* pShader = Manager::Shader();
		pShader->SetUniform(2, matWorld);
	}
	else
	{
		MatrixMode(MATRIX_MODE::MODELVIEW);
		LoadMatrix(matWorld * _matView);
	}
	_matWorld = matWorld;
}

//-----------------------------------------------------------------------------
//!	色
//-----------------------------------------------------------------------------
void RenderManager::SetMaterialColor(const Color& color)
{
	if (_shaderActive == true)
	{
		ShaderManager* pShader = Manager::Shader();

		//	materialColor
		pShader->SetUniform(11, color);
		pShader->SetUniform(12, true);
	}
	else
	{
		Vector4 c = color.GetColorF();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, (f32*)&c);
	}
}
