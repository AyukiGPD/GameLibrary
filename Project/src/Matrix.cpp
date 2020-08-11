//=============================================================================
//!
//!	@file	Matrix.cpp
//!	@brief	�}�g���b�N�X�\����
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//!	�P�ʍs��
Matrix Matrix::IDENTITY = Matrix( 1.0f, 0.0f, 0.0f, 0.0f,
								  0.0f, 1.0f, 0.0f, 0.0f,
								  0.0f, 0.0f, 1.0f, 0.0f,
								  0.0f, 0.0f, 0.0f, 1.0f);
//!	0�s��
Matrix Matrix::ZERO = Matrix( 0.0f, 0.0f, 0.0f, 0.0f,
							  0.0f, 0.0f, 0.0f, 0.0f,
							  0.0f, 0.0f, 0.0f, 0.0f,
							  0.0f, 0.0f, 0.0f, 0.0f);


//!	�J�����ʒu�r���[�s��
//!	@param	[in]	position	���W
//!	@param	[in]	lookAt		���_
//!	@param	[in]	worldUp		�J������̕���	default:(0,1,0)
Matrix Matrix::LookAtViewMatrix(const Vector3& position,const Vector3& lookAt,const Vector3& worldUp)
{
	Vector3 axisZ = Vector3::Normalize(position - lookAt);
	Vector3 axisX = Vector3::Normalize(Vector3::Cross(worldUp, axisZ));
	Vector3 axisY = Vector3::Cross(axisZ, axisX);

	return Matrix(
		axisX.x, axisY.x, axisZ.x, 0.0f,
		axisX.y, axisY.y, axisZ.y, 0.0f,
		axisX.z, axisY.z, axisZ.z, 0.0f,
		-Vector3::Dot(axisX, position),
		-Vector3::Dot(axisY, position),
		-Vector3::Dot(axisZ, position),
		1.0f);
}

//!	�J�����ʒu�s��
//!	@param	[in]	position	���W
//!	@param	[in]	lookAt		���_
//!	@param	[in]	worldUp		�J������̕���	default:(0,1,0)
Matrix Matrix::LookAtMatrix(const Vector3& position,const Vector3& lookAt,const Vector3& worldUp)
{
	Vector3 axisZ = Vector3::Normalize(position - lookAt);
	Vector3 axisX = Vector3::Normalize(Vector3::Cross(worldUp, axisZ));
	Vector3 axisY = Vector3::Cross(axisZ, axisX);

	return Matrix(
		axisX.x, axisX.y, axisX.z, 0.0f,
		axisY.x, axisY.y, axisY.z, 0.0f,
		axisZ.x, axisZ.y, axisZ.z, 0.0f,
		position.x,
		position.y,
		position.z,
		1.0f);
}


//!	�������e
//!	@param	[in]	fovy	�c�̎���p
//!	@param	[in]	aspect	��ʔ䗦
//!	@param	[in]	znear	��O
//!	@param	[in]	zfar	���s��
Matrix Matrix::PerspectiveMatrix(f32 fovy, f32 aspect, f32 znear, f32 zfar)
{
	f32 radian = 2.f * Math::PI * fovy / 360.0f;
	f32 t = 1.0f / tanf(radian / 2.f);
	Matrix m(
		t / aspect, 0,                                     0,  0,
		         0, t,                                     0,  0,
		         0, 0,       (zfar + znear) / (znear - zfar), -1,
		         0, 0, (2.f * zfar * znear) / (znear - zfar),  0);

	return m;
}

//!	���s���e
//!	@param	[in]	left	��
//!	@param	[in]	right	�E
//!	@param	[in]	bottom	��
//!	@param	[in]	top		��
//!	@param	[in]	znear	��O
//!	@param	[in]	zfar	���s��
Matrix Matrix::OrthoMatrix(f32 left, f32 right, f32 bottom, f32 top, f32 znear, f32 zfar)
{
	Vector3 t;
	t.x = -((right + left  ) / (right - left  ));
	t.y = -((top   + bottom) / (top   - bottom));
	t.z = -((zfar  + znear ) / (zfar  - znear ));
	Matrix m(
		2.f / (right - left),                    0,                    0, 0,
		                   0, 2.f / (top - bottom),                    0, 0,
		                   0,                    0, 2.f / (zfar - znear), 0,
		                 t.x,                  t.y,                  t.z, 1);

	return m;
}


Matrix Matrix::FrustumMatrix(f32 left, f32 right, f32 bottom, f32 top, f32 znear, f32 zfar)
{
	f32 a =  (right + left  ) / (right - left  );
	f32 b =  (top   + bottom) / (top   - bottom);
	f32 c = -(zfar  + znear ) / (zfar  - znear );
	f32 d = -2.0 * zfar * znear / (zfar - znear);
	Matrix m(
		2.f * znear / (right - left),					 0.f, 0.f,  0.f,
								 0.f, 2.0*znear/(top-bottom), 0.f,  0.f, 
								   a,                      b,   c, -1.f, 
								 0.f,                    0.0,   d,  0.f);
	return m;
}


