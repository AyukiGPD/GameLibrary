//=============================================================================
//!
//!	@file	Renderer.cpp
//!	@brief	�`��Ǘ��N���X
//!			�`����Ǘ�����N���X�B�V�F�[�_�[�̗L���A�r���[�|�[�g�̐ݒ�Ȃ�
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
RenderManager::RenderManager()
: _shaderActive(false)
, _matProj(Matrix::IDENTITY)
, _matView(Matrix::IDENTITY)
{
}

//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
RenderManager::~RenderManager()
{
}


//-----------------------------------------------------------------------------
//!	������
//-----------------------------------------------------------------------------
void RenderManager::Init()
{
}
//-----------------------------------------------------------------------------
//!	�X�V
//-----------------------------------------------------------------------------
void RenderManager::Update()
{
}
//-----------------------------------------------------------------------------
//!	�`��
//-----------------------------------------------------------------------------
void RenderManager::Render()
{
}
//-----------------------------------------------------------------------------
//!	�I��
//-----------------------------------------------------------------------------
void RenderManager::Exit()
{
}

//-----------------------------------------------------------------------------
//!	�f�v�X�L��
//-----------------------------------------------------------------------------
void RenderManager::EnableDepthTest()
{
	glEnable(GL_DEPTH_TEST);
}
//-----------------------------------------------------------------------------
//!	�f�v�X����
//-----------------------------------------------------------------------------
void RenderManager::DisableDepthTest()
{
	glDisable(GL_DEPTH_TEST);
}

//-----------------------------------------------------------------------------
//!	�A���t�@�L��
//-----------------------------------------------------------------------------
void RenderManager::EnableAlphaBlending(BLEND_MODE sfactor, BLEND_MODE dfactor)
{
	//	�A���t�@�L����
	glEnable(GL_BLEND);
	BlendFunc(sfactor, dfactor);
}
//-----------------------------------------------------------------------------
//!	�A���t�@����
//-----------------------------------------------------------------------------
void RenderManager::DisableAlphaBlending()
{
	glDisable(GL_BLEND);
}

//-----------------------------------------------------------------------------
//!	�u�����h�L��
//-----------------------------------------------------------------------------
void RenderManager::EnableBlending(BLEND_MODE sfactor, BLEND_MODE dfactor)
{
	glEnable(GL_BLEND);
	BlendFunc(sfactor, dfactor);
}
//-----------------------------------------------------------------------------
//!	�u�����h����
//-----------------------------------------------------------------------------
void RenderManager::DisableBlending()
{
	glDisable(GL_BLEND);
}


//-----------------------------------------------------------------------------
//!	�u�����h
//-----------------------------------------------------------------------------
void RenderManager::BlendFunc(BLEND_MODE sfactor, BLEND_MODE dfactor)
{
	glBlendFunc((GLenum)sfactor, (GLenum)dfactor);
}

//-----------------------------------------------------------------------------
//!	�s��ݒ�
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
//!	�s��ۑ�
//-----------------------------------------------------------------------------
void RenderManager::PushMatrix()
{
	glPushMatrix();
}
//-----------------------------------------------------------------------------
//!	�s�񕜌�
//-----------------------------------------------------------------------------
void RenderManager::PopMatrix()
{
	glPopMatrix();
}

//-----------------------------------------------------------------------------
//!	�r���[�|�[�g�ݒ�
//-----------------------------------------------------------------------------
void RenderManager::SetViewport(s32 x, s32 y, s32 width, s32 height)
{
	glViewport(x, y, width, height);
}
//-----------------------------------------------------------------------------
//!	�r���[�|�[�g�ݒ�
//-----------------------------------------------------------------------------
void RenderManager::SetViewport(const Viewport& viewport)
{
	SetViewport(viewport.x, viewport.y, viewport.width, viewport.height);
}

//-----------------------------------------------------------------------------
//!	�r���[�|�[�g�擾
//-----------------------------------------------------------------------------
Viewport RenderManager::GetViewport()
{
	Viewport result;
	glGetIntegerv(GL_VIEWPORT, (GLint*)&result);
	return result;
}

//-----------------------------------------------------------------------------
//!	�v���W�F�N�V�����s��
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
//!	�r���[�s��
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
//!	���[���h�s��
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
//!	�F
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
