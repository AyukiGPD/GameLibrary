//=============================================================================
//!
//!	@file	Vector.cpp
//!	@brief	ベクター構造体
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

const Vector2 Vector2::ZERO  ( 0.0f,  0.0f);
const Vector2 Vector2::ONE   ( 1.0f,  1.0f);
const Vector2 Vector2::MINUS (-1.0f, -1.0f);

const Vector3 Vector3::ZERO   ( 0.0f,  0.0f,  0.0f);
const Vector3 Vector3::ONE    ( 1.0f,  1.0f,  1.0f);
const Vector3 Vector3::MINUS  (-1.0f, -1.0f, -1.0f);
const Vector3 Vector3::AXIS_X ( 1.0f,  0.0f,  0.0f);
const Vector3 Vector3::AXIS_Y ( 0.0f,  1.0f,  0.0f);
const Vector3 Vector3::AXIS_Z ( 0.0f,  0.0f,  1.0f);

const Vector4 Vector4::ZERO  ( 0.0f,  0.0f,  0.0f,  0.0f);
const Vector4 Vector4::ONE   ( 1.0f,  1.0f,  1.0f,  1.0f);
const Vector4 Vector4::MINUS(-1.0f, -1.0f, -1.0f, -1.0f);


const Angle Angle::AXIS_X(1.0f, 0.0f, 0.0f, 0.0f);
const Angle Angle::AXIS_Y(0.0f, 1.0f, 0.0f, 0.0f);
const Angle Angle::AXIS_Z(0.0f, 0.0f, 1.0f, 0.0f);
const Angle Angle::ZERO  (0.0f, 0.0f, 0.0f, 0.0f);

const Quaternion Quaternion::ZERO	  (0.0f, 0.0f, 0.0f, 0.0f);
const Quaternion Quaternion::ONE	  (1.0f, 1.0f, 1.0f, 1.0f);
const Quaternion Quaternion::IDENTITY (0.0f, 0.0f, 0.0f, 1.0f);


const Vector2i Vector2i::ZERO  ( 0,  0);
const Vector2i Vector2i::ONE   ( 1,  1);
const Vector2i Vector2i::MINUS (-1, -1);

const Vector3i Vector3i::ZERO  ( 0,  0,  0);
const Vector3i Vector3i::ONE   ( 1,  1,  1);
const Vector3i Vector3i::MINUS (-1, -1, -1);



namespace Math
{
Vector2 RandomVector2()
{
	return Vector2(Randomf(), Randomf());
}
Vector3 RandomVector3()
{
	return Vector3(Randomf(), Randomf(), Randomf());
}
Vector4 RandomVector4()
{
	return Vector4(Randomf(), Randomf(), Randomf(), Randomf());
}

}




