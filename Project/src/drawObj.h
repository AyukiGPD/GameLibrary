//=============================================================================
//!
//!	@file	drawObj.h
//!	@brief	�}�`�`��֐��Q
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once


//!	�@���쐬
//!	@param	[in]	A	���_A
//!	@param	[in]	B	���_B
//!	@param	[in]	C	���_C
//!	@return	�@���x�N�g��
Vector3 CreateTriangleNormal(const Vector3& A, const Vector3& B, const Vector3& C);


//!	2D�`��֐����O���
namespace Render2D
{

//!	�l�p�`��
//!	@param	[in]	x		���WX
//!	@param	[in]	y		���WY
//!	@param	[in]	w		�T�C�YW
//!	@param	[in]	h		�T�C�YH
//!	@param	[in]	color	�F
void Rect(f32 x, f32 y, f32 w, f32 h,	const Color& color);

//!	�l�p�`��
//!	@param	[in]	position	���W
//!	@param	[in]	size		�T�C�Y
//!	@param	[in]	color		�F
void Rect(const Vector2& position, const Vector2& size, const Color& color);

//!	�e�N�X�`���`��
//!	@param	[in]	pTexture	�e�N�X�`���̃|�C���^
void RenderBuffer(Texture* pTexture = nullptr);
};

//!	3D�`��֐����O���
namespace Render3D
{

//!	�_�`��
//!	@param	[in]	position	���W
//!	@param	[in]	color		�F		default:��
//!	@param	[in]	size		�T�C�Y	default:1.0f
void Point(const Vector3& position, const Color& color = 255, f32 size = 1.0f);

//!	���`��
//!	@param	[in]	from	�n�_
//!	@param	[in]	to		�I�_
//!	@param	[in]	color	�F		default:��
void Line(const Vector3& from, const Vector3& to, const Color& color = 255);

//!	���`��
//!	@param	[in]	from		�n�_
//!	@param	[in]	to			�I�_
//!	@param	[in]	fromColor	�n�_�F
//!	@param	[in]	toColor		�I�_�F
void Line(const Vector3& from, const Vector3& to, const Color& fromColor, const Color& toColor);

//!	�l�p�`��
//!	@param	[in]	position	���W
//!	@param	[in]	size		�T�C�Y
//!	@param	[in]	color		�F		default:��
void Rect(const Vector3& position, const Vector3& size, const Color& color = 255);

//!	�l�p�`��
//!	@param	[in]	p1		���_�f�[�^
//!	@param	[in]	p2		���_�f�[�^
//!	@param	[in]	p3		���_�f�[�^
//!	@param	[in]	p4		���_�f�[�^
//!	@param	[in]	color	�F		default:��
void Rect(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4, const Color& color = 255);

//!	�l�p�`��
//!	@param	[in]	p1	���_�f�[�^
//!	@param	[in]	p2	���_�f�[�^
//!	@param	[in]	p3	���_�f�[�^
//!	@param	[in]	p4	���_�f�[�^
void Rect(const Vertex& p1, const Vertex& p2, const Vertex& p3, const Vertex& p4);

//!	�l�p�`��
//!	@param	[in]	vertices	���_�f�[�^�z��
void Rect(const std::array<Vertex,4>& vertices);

};
