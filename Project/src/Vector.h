//=============================================================================
//!
//!	@file	Vector.h
//!	@brief	ƒxƒNƒ^[\‘¢‘Ì
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once


struct Vector3;
struct Vector4;
struct Angle;
struct Quaternion;
struct Matrix;

//=============================================================================
//!	Vector2
//=============================================================================
struct Vector2
{
	//!	ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	MY_INLINE	Vector2();
	//!	ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	MY_INLINE	Vector2(f32 x, f32 y);

	//!	ƒZƒbƒgŠÖ”
	MY_INLINE	void Set(f32 x, f32 y);

	//!	ƒxƒNƒgƒ‹‚Ì’·‚³
	MY_INLINE	f32 LengthSq() const;
	//!	ƒxƒNƒgƒ‹‚Ì’·‚³
	MY_INLINE	f32 Length() const;
	//!	³‹K‰»
	MY_INLINE	static Vector2 Normalize(const Vector2& vec);
	MY_INLINE	static Vector2 Normalize(const Vector3& vec);
	MY_INLINE	static Vector2 Normalize(const Vector4& vec);

	MY_INLINE	f32* GetPtr();
	MY_INLINE	const f32* GetPtr() const;

	MY_INLINE	f32& operator[](s32 index);
	MY_INLINE	f32  operator[](s32 index) const;

	//	‰‰ZŠÖ”
	//!	‰ÁZ
	MY_INLINE	void Add(const Vector2& v);
	//!	Œ¸Z
	MY_INLINE	void Subtract(const Vector2& v);
	//!	æZ
	MY_INLINE	void Multi(const Vector2& v);
	//!	œZ
	MY_INLINE	void Div(const Vector2& v);

	//!	‰ÁZ
	MY_INLINE	void Add(f32 v);
	//!	Œ¸Z
	MY_INLINE	void Subtract(f32 v);
	//!	æZ
	MY_INLINE	void Multi(f32 v);
	//!	œZ
	MY_INLINE	void Div(f32 v);

	//	ƒIƒyƒŒ[ƒ^[
	//!	–{‘Ì‚Ì‰ÁZ
	MY_INLINE	Vector2& operator+=(const Vector2& v);
	//!	–{‘Ì‚ÌŒ¸Z
	MY_INLINE	Vector2& operator-=(const Vector2& v);
	//!	–{‘Ì‚ÌæZ
	MY_INLINE	Vector2& operator*=(const Vector2& v);
	//!	–{‘Ì‚ÌœZ
	MY_INLINE	Vector2& operator/=(const Vector2& v);

	//!	‰ÁZŒ‹‰Ê
	MY_INLINE	Vector2 operator+ (const Vector2& v) const;
	//!	Œ¸ZŒ‹‰Ê
	MY_INLINE	Vector2 operator- (const Vector2& v) const;
	//!	æZŒ‹‰Ê
	MY_INLINE	Vector2 operator* (const Vector2& v) const;
	//!	œZŒ‹‰Ê
	MY_INLINE	Vector2 operator/ (const Vector2& v) const;

	//!	–{‘Ì‚Ì‰ÁZ
	MY_INLINE	Vector2& operator+=(f32 v);
	//!	–{‘Ì‚ÌŒ¸Z
	MY_INLINE	Vector2& operator-=(f32 v);
	//!	–{‘Ì‚ÌæZ
	MY_INLINE	Vector2& operator*=(f32 v);
	//!	–{‘Ì‚ÌœZ
	MY_INLINE	Vector2& operator/=(f32 v);

	//!	‰ÁZŒ‹‰Ê
	MY_INLINE	Vector2 operator+ (f32 v) const;
	//!	Œ¸ZŒ‹‰Ê
	MY_INLINE	Vector2 operator- (f32 v) const;
	//!	æZŒ‹‰Ê
	MY_INLINE	Vector2 operator* (f32 v) const;
	//!	œZŒ‹‰Ê
	MY_INLINE	Vector2 operator/ (f32 v) const;

	MY_INLINE	bool operator==(const Vector2& v) const;
	MY_INLINE	bool operator!=(const Vector2& v) const;

	MY_INLINE	operator	   f32*()const;
	MY_INLINE	operator const f32*()const;

	//	stream
	friend MY_INLINE std::ostream& operator<<(std::ostream& os, const Vector2& v);

	//	Bullet
	MY_INLINE	Vector2& operator=(const btVector3& v);
	MY_INLINE	operator btVector3() const;


	static const Vector2 ZERO;
	static const Vector2 ONE;
	static const Vector2 MINUS;

	//-----	param
	f32 x;
	f32 y;
};

//=============================================================================
//!	Vector3
//=============================================================================
struct Vector3
{
	//!	ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	MY_INLINE	Vector3();
	//!	ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	MY_INLINE	Vector3(f32 x, f32 y, f32 z);

	MY_INLINE	Vector3(const Vector2& vec,f32 z);

	MY_INLINE	Vector3(f32 scale);


	MY_INLINE	Vector3(const btVector3& vec);

	//!	ƒZƒbƒgŠÖ”
	MY_INLINE	void Set(f32 x, f32 y, f32 z);
	//!	ƒZƒbƒgŠÖ”
	MY_INLINE	void Set(f32 scale);

	//!	ƒxƒNƒgƒ‹‚Ì’·‚³
	MY_INLINE	f32 LengthSq()const;
	//!	ƒxƒNƒgƒ‹‚Ì’·‚³
	MY_INLINE	f32 Length()const;
	//!	³‹K‰»
	MY_INLINE	static Vector3 Normalize(const Vector2& vec);
	MY_INLINE	static Vector3 Normalize(const Vector3& vec);
	MY_INLINE	static Vector3 Normalize(const Vector4& vec);

	MY_INLINE	Vector3 GetRotate() const;

	MY_INLINE	void SetLength(f32 length);
	//!	ŠOÏ
	MY_INLINE	static Vector3 Cross(const Vector3& a, const Vector3& b);
	//!	“àÏ
	MY_INLINE	static f32 Dot(const Vector3& a, const Vector3& b);

	//!	ƒxƒNƒgƒ‹Šp“x
	MY_INLINE	static f32 Radian(const Vector3& a, const Vector3& b);
	
	//!	üŒ`•âŠÔ
	MY_INLINE	static Vector3 Lerp(const Vector3& a, const Vector3& b,f32 scale);

	MY_INLINE	Vector3 Rotate(const Angle& rotate)const;
	MY_INLINE	Vector3 Rotate(const Vector3& rotate)const;

	MY_INLINE	bool Match(const Vector3& vec, f32 tolerance = FLT_EPSILON);

	MY_INLINE	f32* GetPtr();
	MY_INLINE	const f32* GetPtr() const;

	MY_INLINE	f32& operator[](s32 index);
	MY_INLINE	f32  operator[](s32 index) const;

	//	stream
	friend MY_INLINE std::ostream& operator<<(std::ostream& os, const Vector3& v);


	//	‰‰ZŠÖ”
	//!	‰ÁZ
	MY_INLINE	void Add(const Vector3& v);
	//!	Œ¸Z
	MY_INLINE	void Subtract(const Vector3& v);
	//!	æZ
	MY_INLINE	void Multi(const Vector3& v);
	//!	œZ
	MY_INLINE	void Div(const Vector3& v);

	//!	‰ÁZ
	MY_INLINE	void Add(f32 v);
	//!	Œ¸Z
	MY_INLINE	void Subtract(f32 v);
	//!	æZ
	MY_INLINE	void Multi(f32 v);
	//!	œZ
	MY_INLINE	void Div(f32 v);

	//	ƒIƒyƒŒ[ƒ^[
	//!	–{‘Ì‚Ì‰ÁZ
	MY_INLINE	Vector3& operator += (const Vector3& v);
	//!	–{‘Ì‚ÌŒ¸Z
	MY_INLINE	Vector3& operator -= (const Vector3& v);
	//!	–{‘Ì‚ÌæZ
	MY_INLINE	Vector3& operator *= (const Vector3& v);
	//!	–{‘Ì‚ÌœZ
	MY_INLINE	Vector3& operator /= (const Vector3& v);

	//!	‰ÁZŒ‹‰Ê
	MY_INLINE	Vector3 operator + (const Vector3& v) const;
	//!	Œ¸ZŒ‹‰Ê
	MY_INLINE	Vector3 operator - (const Vector3& v) const;
	//!	æZŒ‹‰Ê
	MY_INLINE	Vector3 operator * (const Vector3& v) const;
	//!	œZŒ‹‰Ê
	MY_INLINE	Vector3 operator / (const Vector3& v) const;

	//!	–{‘Ì‚Ì‰ÁZ
	MY_INLINE	Vector3& operator += (f32 v);
	//!	–{‘Ì‚ÌŒ¸Z
	MY_INLINE	Vector3& operator -= (f32 v);
	//!	–{‘Ì‚ÌæZ
	MY_INLINE	Vector3& operator *= (f32 v);
	//!	–{‘Ì‚ÌœZ
	MY_INLINE	Vector3& operator /= (f32 v);

	//!	‰ÁZŒ‹‰Ê
	MY_INLINE	Vector3 operator + (f32 v) const;
	//!	Œ¸ZŒ‹‰Ê
	MY_INLINE	Vector3 operator - (f32 v) const;
	//!	æZŒ‹‰Ê
	MY_INLINE	Vector3 operator * (f32 v) const;
	//!	œZŒ‹‰Ê
	MY_INLINE	Vector3 operator / (f32 v) const;

	MY_INLINE	operator Vector2 () const;
	MY_INLINE	operator f32* () const;
	MY_INLINE	operator const f32* () const;


	//	”äŠr‰‰Z
	MY_INLINE	bool operator == (const Vector3& v) const;
	MY_INLINE	bool operator != (const Vector3& v) const;

	//	Bullet
	MY_INLINE	Vector3& operator=(const btVector3& v);
	MY_INLINE	operator btVector3() const;


	static const Vector3 ZERO;
	static const Vector3 ONE;
	static const Vector3 MINUS;
	static const Vector3 AXIS_X;
	static const Vector3 AXIS_Y;
	static const Vector3 AXIS_Z;

	f32 x;
	f32 y;
	f32 z;
};

//=============================================================================
//!	Vector4
//=============================================================================
struct Vector4
{
	//! ƒfƒtƒHƒ‹ƒgƒRƒ“ƒXƒgƒ‰ƒNƒ^
	MY_INLINE	Vector4();
	//! ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	MY_INLINE	Vector4(f32 x, f32 y, f32 z, f32 w);

	// ƒZƒbƒgŠÖ”
	MY_INLINE	void Set(f32 x, f32 y, f32 z, f32 w);

	MY_INLINE	void Set(Vector3 v, f32 w);

	MY_INLINE	f32* GetPtr();
	MY_INLINE	const f32* GetPtr() const;

	MY_INLINE	f32& operator[](s32 index);
	MY_INLINE	f32  operator[](s32 index) const;

	MY_INLINE	f32  LengthSq() const;
	MY_INLINE	f32  Length() const;


	//!	³‹K‰»
	MY_INLINE	static Vector4 Normalize(const Vector2& vec);
	MY_INLINE	static Vector4 Normalize(const Vector3& vec);
	MY_INLINE	static Vector4 Normalize(const Vector4& vec);

	//	stream
	friend MY_INLINE std::ostream& operator<<(std::ostream& os, const Vector4& v);

	//	‰‰ZŠÖ”
	//!	‰ÁZ
	MY_INLINE	void Add(const Vector4& v);
	//!	Œ¸Z
	MY_INLINE	void Subtract(const Vector4& v);
	//!	æZ
	MY_INLINE	void Multi(const Vector4& v);
	//!	œZ
	MY_INLINE	void Div(const Vector4& v);

	//!	‰ÁZ
	MY_INLINE	void Add(f32 v);
	//!	Œ¸Z
	MY_INLINE	void Subtract(f32 v);
	//!	æZ
	MY_INLINE	void Multi(f32 v);
	//!	œZ
	MY_INLINE	void Div(f32 v);

	//	ƒIƒyƒŒ[ƒ^[
	//!	–{‘Ì‚Ì‰ÁZ
	MY_INLINE	Vector4& operator += (const Vector4& v);
	//!	–{‘Ì‚ÌŒ¸Z
	MY_INLINE	Vector4& operator -= (const Vector4& v);
	//!	–{‘Ì‚ÌæZ
	MY_INLINE	Vector4& operator *= (const Vector4& v);
	//!	–{‘Ì‚ÌœZ
	MY_INLINE	Vector4& operator /= (const Vector4& v);

	//!	‰ÁZŒ‹‰Ê
	MY_INLINE	Vector4 operator + (const Vector4& v) const;
	//!	Œ¸ZŒ‹‰Ê
	MY_INLINE	Vector4 operator - (const Vector4& v) const;
	//!	æZŒ‹‰Ê
	MY_INLINE	Vector4 operator * (const Vector4& v) const;
	//!	œZŒ‹‰Ê
	MY_INLINE	Vector4 operator / (const Vector4& v) const;
	//!	‰ÁZŒ‹‰Ê
	MY_INLINE	Vector4 operator + (f32 v) const;
	//!	Œ¸ZŒ‹‰Ê
	MY_INLINE	Vector4 operator - (f32 v) const;
	//!	æZŒ‹‰Ê
	MY_INLINE	Vector4 operator * (f32 v) const;
	//!	œZŒ‹‰Ê
	MY_INLINE	Vector4 operator / (f32 v) const;


	MY_INLINE	operator Vector2()const;
	MY_INLINE	operator Vector3()const;

	MY_INLINE	operator f32* () const;
	MY_INLINE	operator const f32* () const;
	MY_INLINE	bool operator == (const Vector4& v) const;
	MY_INLINE	bool operator != (const Vector4& v) const;


	//	Bullet
	MY_INLINE	Vector4& operator=(const btVector3& v);
	MY_INLINE	operator btVector3() const;

	static const Vector4 ZERO;
	static const Vector4 ONE;
	static const Vector4 MINUS;


	f32 x;
	f32 y;
	f32 z;
	f32 w;
};


//=============================================================================
//!	Angle
//=============================================================================
struct Angle
{
	MY_INLINE	Angle();
	MY_INLINE	Angle(const Angle& angle);
	MY_INLINE	Angle(f32 x, f32 y, f32 z, f32 angle);
	MY_INLINE	Angle(const Vector3& axis, f32 angle);

//				Angle(const Vector3& rotate);


	MY_INLINE	Angle Normalize() const;
	MY_INLINE	void  Normalized();

	MY_INLINE	const Angle operator+ (const Angle& angle);
	MY_INLINE		  Angle operator+=(const Angle& angle);
	MY_INLINE	const Angle operator+ (const Vector3& axis);
	MY_INLINE		  Angle operator+=(const Vector3& axis);
	MY_INLINE	const Angle operator+ (f32 angle);
	MY_INLINE		  Angle operator+=(f32 angle);

	MY_INLINE	Angle& operator=(const Quaternion& v);
	MY_INLINE	operator Quaternion() const;

	//	stream
	friend MY_INLINE std::ostream& operator<<(std::ostream& os, const Angle& a);

	//	Bullet
	MY_INLINE	Angle& operator=(const btQuaternion& v);
	MY_INLINE	operator btQuaternion() const;

	static const Angle AXIS_X;
	static const Angle AXIS_Y;
	static const Angle AXIS_Z;
	static const Angle ZERO;

	Vector3 _axis;
	f32		_angle;
};


//=============================================================================
//!	Quaternion
//=============================================================================
struct Quaternion
{
	MY_INLINE	Quaternion();
	MY_INLINE	Quaternion(f32 x, f32 y, f32 z, f32 w);
	MY_INLINE	Quaternion(const Vector4& v);
	MY_INLINE	Quaternion(f32 angle, const Vector3& axis);
	MY_INLINE	Quaternion(const Angle& angle);

	MY_INLINE	Quaternion(const btQuaternion& quat);


	//!	‰ñ“]
	MY_INLINE	void Rotate(f32 angle, f32 x, f32 y, f32 z);
	MY_INLINE	void Rotate(f32 angle, const Vector3& axis);
	MY_INLINE	void Rotate(const Angle& angle);

	MY_INLINE	void Rotate(f32 yaw, f32 pitch, f32 roll);
	MY_INLINE	void Rotate(const Vector3& euler);

	//!	‰ñ“]
	MY_INLINE	static Quaternion RotateAngle(f32 angle, f32 x, f32 y, f32 z);
	MY_INLINE	static Quaternion RotateAngle(f32 angle, const Vector3& axis);
	MY_INLINE	static Quaternion RotateAngle(const Angle& angle);

	MY_INLINE	static Quaternion RotateEuler(f32 yaw, f32 pitch, f32 roll);
	MY_INLINE	static Quaternion RotateEuler(const Vector3& euler);

	//!	‹…–ÊüŒ`•âŠÔ
	MY_INLINE	static Quaternion Slerp(const Quaternion& a, const Quaternion& b,f32 scale);


	MY_INLINE	Angle GetAngleRotate()const;

	MY_INLINE	void Normalized();
	MY_INLINE	Quaternion Normalize() const;
	
	MY_INLINE	static f32 Dot(const Quaternion& a, const Quaternion& b);

	MY_INLINE	f32 Length()const;
	MY_INLINE	f32 Length2()const;
	MY_INLINE	const Quaternion Inverse()const;

	//	stream
	friend MY_INLINE std::ostream& operator<<(std::ostream& os, const Quaternion& q);

	//	operator
	MY_INLINE const	Quaternion	operator* (f32 scale) const;
	MY_INLINE		Quaternion& operator*=(f32 scale);
	MY_INLINE const	Quaternion	operator* (const Quaternion& quaternion) const;
	MY_INLINE		Quaternion& operator*=(const Quaternion& quaternion);
	MY_INLINE		Quaternion	operator/ (f32 scale) const;
	MY_INLINE		Quaternion& operator/=(f32 scale);

	MY_INLINE const	Quaternion	operator+ (const Quaternion& quaternion) const;
	//MY_INLINE		Quaternion& operator+=(const Quaternion& quaternion);
	MY_INLINE const	Quaternion	operator- (const Quaternion& quaternion) const;
	//MY_INLINE		Quaternion& operator-=(const Quaternion& quaternion);
	MY_INLINE const	Quaternion	operator- () const;
	MY_INLINE		Vector3		operator* (const Vector3& v) const;



	//MY_INLINE	Quaternion& operator=(const Angle& v);
	//MY_INLINE	operator Angle() const;

	//	Bullet
	MY_INLINE	Quaternion& operator=(const btQuaternion& v);
	MY_INLINE	operator btQuaternion() const;


	static const Quaternion ZERO;
	static const Quaternion ONE;
	static const Quaternion IDENTITY;

	Vector4 _v;
};

//=============================================================================
//!	Vector2
//=============================================================================
//-----------------------------------------------------------------------------
//!	ƒRƒ“ƒXƒgƒ‰ƒNƒ^
//-----------------------------------------------------------------------------
Vector2::Vector2() :x(0), y(0){}
//-----------------------------------------------------------------------------
//!	ƒRƒ“ƒXƒgƒ‰ƒNƒ^
//-----------------------------------------------------------------------------
Vector2::Vector2(f32 _x, f32 _y) : x(_x), y(_y){}

//-----------------------------------------------------------------------------
//!	ƒZƒbƒgŠÖ”
//-----------------------------------------------------------------------------
void Vector2::Set(f32 _x, f32 _y)
{
	this->x = _x;
	this->y = _y;
}

//-----------------------------------------------------------------------------
//!	ƒxƒNƒgƒ‹‚Ì’·‚³
//-----------------------------------------------------------------------------
f32 Vector2::LengthSq() const
{
	return x * x + y * y;
}
//-----------------------------------------------------------------------------
//!	ƒxƒNƒgƒ‹‚Ì’·‚³
//-----------------------------------------------------------------------------
f32 Vector2::Length() const
{
	return sqrtf(LengthSq() + FLT_EPSILON);
}

//-----------------------------------------------------------------------------
//!	³‹K‰»
//-----------------------------------------------------------------------------
//Vector2 Vector2::Normalize()
//{
//	f32 invL = 1.0f / Length();
//	return Vector2(x*invL, y*invL);
//}

Vector2 Vector2::Normalize(const Vector2& vec)
{
	f32 invL = 1.0f / vec.Length();
	return Vector2(vec.x * invL, vec.y * invL);
}
Vector2 Vector2::Normalize(const Vector3& vec)
{
	f32 invL = 1.0f / vec.Length();
	return Vector2(vec.x * invL, vec.y * invL);
}
Vector2 Vector2::Normalize(const Vector4& vec)
{
	f32 invL = 1.0f / vec.Length();
	return Vector2(vec.x * invL, vec.y * invL);
}


f32* Vector2::GetPtr()
{
	return (f32*)&x;
}

const f32* Vector2::GetPtr() const
{
	return (const f32*)&x;
}

f32& Vector2::operator[](s32 index)
{
	return GetPtr()[index];
}

f32  Vector2::operator[](s32 index) const
{
	return GetPtr()[index];
}

//	stream
std::ostream& operator<<(std::ostream& os, const Vector2& v)
{
	os	<< "x:" <<v.x << ","
		<< "y:"<< v.y;

	return os;
}


//	‰‰ZŠÖ”
//-----------------------------------------------------------------------------
//!	‰ÁZ
//-----------------------------------------------------------------------------
void Vector2::Add(const Vector2& v)
{
	x += v.x;
	y += v.y;
}
//-----------------------------------------------------------------------------
//!	Œ¸Z
//-----------------------------------------------------------------------------
void Vector2::Subtract(const Vector2& v)
{
	x -= v.x;
	y -= v.y;
}
//-----------------------------------------------------------------------------
//!	æZ
//-----------------------------------------------------------------------------
void Vector2::Multi(const Vector2& v)
{
	x *= v.x;
	y *= v.y;
}
//-----------------------------------------------------------------------------
//!	œZ
//-----------------------------------------------------------------------------
void Vector2::Div(const Vector2& v)
{
	x /= v.x;
	y /= v.y;
}

//-----------------------------------------------------------------------------
//!	‰ÁZ
//-----------------------------------------------------------------------------
void Vector2::Add(f32 v)
{
	x += v;
	y += v;
}
//-----------------------------------------------------------------------------
//!	Œ¸Z
//-----------------------------------------------------------------------------
void Vector2::Subtract(f32 v)
{
	x -= v;
	y -= v;
}
//-----------------------------------------------------------------------------
//!	æZ
//-----------------------------------------------------------------------------
void Vector2::Multi(f32 v)
{
	x *= v;
	y *= v;
}
//-----------------------------------------------------------------------------
//!	œZ
//-----------------------------------------------------------------------------
void Vector2::Div(f32 v)
{
	x /= v;
	y /= v;
}

//-----------------------------------------------------------------------------
//	ƒIƒyƒŒ[ƒ^[
//-----------------------------------------------------------------------------
//!	–{‘Ì‚Ì‰ÁZ
Vector2& Vector2::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;

	return *this;
}
//!	–{‘Ì‚ÌŒ¸Z
Vector2& Vector2::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}
//!	–{‘Ì‚ÌæZ
Vector2& Vector2::operator*=(const Vector2& v)
{
	x *= v.x;
	y *= v.y;

	return *this;
}
//!	–{‘Ì‚ÌœZ
Vector2& Vector2::operator/=(const Vector2& v)
{
	x /= v.x;
	y /= v.y;

	return *this;
}

//!	‰ÁZŒ‹‰Ê
Vector2 Vector2::operator+(const Vector2& v) const
{
	Vector2 result;
	result.x = x + v.x;
	result.y = y + v.y;
	return result;
}
//!	Œ¸ZŒ‹‰Ê
Vector2 Vector2::operator-(const Vector2& v) const
{
	Vector2 result;
	result.x = x - v.x;
	result.y = y - v.y;
	return result;
}
//!	æZŒ‹‰Ê
Vector2 Vector2::operator*(const Vector2& v) const
{
	Vector2 result;
	result.x = x * v.x;
	result.y = y * v.y;
	return result;
}
//!	œZŒ‹‰Ê
Vector2 Vector2::operator/(const Vector2& v) const
{
	Vector2 result;
	result.x = x / v.x;
	result.y = y / v.y;
	return result;
}

//!	–{‘Ì‚Ì‰ÁZ
Vector2& Vector2::operator+=(f32 v)
{
	x += v;
	y += v;

	return *this;
}
//!	–{‘Ì‚ÌŒ¸Z
Vector2& Vector2::operator-=(f32 v)
{
	x -= v;
	y -= v;

	return *this;
}
//!	–{‘Ì‚ÌæZ
Vector2& Vector2::operator*=(f32 v)
{
	x *= v;
	y *= v;

	return *this;
}
//!	–{‘Ì‚ÌœZ
Vector2& Vector2::operator/=(f32 v)
{
	x /= v;
	y /= v;

	return *this;
}

//!	‰ÁZŒ‹‰Ê
Vector2 Vector2::operator+(f32 v) const
{
	Vector2 result;
	result.x = x + v;
	result.y = y + v;
	return result;
}
//!	Œ¸ZŒ‹‰Ê
Vector2 Vector2::operator-(f32 v) const
{
	Vector2 result;
	result.x = x - v;
	result.y = y - v;
	return result;
}
//!	æZŒ‹‰Ê
Vector2 Vector2::operator*(f32 v) const
{
	Vector2 result;
	result.x = x * v;
	result.y = y * v;
	return result;
}
//!	œZŒ‹‰Ê
Vector2 Vector2::operator/(f32 v) const
{
	Vector2 result;
	result.x = x / v;
	result.y = y / v;
	return result;
}

bool Vector2::operator==(const Vector2& v) const
{
	return (x == v.x && y == v.y);
}

bool Vector2::operator!=(const Vector2& v) const
{
	return (x != v.x || y != v.y);
}

Vector2::operator f32* () const
{
	return (f32*)this;
}
Vector2::operator const f32* () const
{
	return (f32*)this;
}

//	Bullet
Vector2& Vector2::operator=(const btVector3& v)
{
	x = v.x();
	y = v.y();
	return *this;
}

Vector2::operator btVector3() const
{
	return btVector3(x, y, 0.f);
}


//-----------------------------------------------------------------------------

//=============================================================================
//!	Vector3
//=============================================================================
//!	ƒRƒ“ƒXƒgƒ‰ƒNƒ^
Vector3::Vector3():x(0), y(0), z(0){}
//!	ƒRƒ“ƒXƒgƒ‰ƒNƒ^
Vector3::Vector3(f32 _x, f32 _y, f32 _z) : x(_x), y(_y), z(_z){}
//!	ƒRƒ“ƒXƒgƒ‰ƒNƒ^
Vector3::Vector3(const Vector2& vec, f32 _z) : x(vec.x), y(vec.y), z(_z){}
//!	ƒRƒ“ƒXƒgƒ‰ƒNƒ^
Vector3::Vector3(f32 scale) : x(scale), y(scale), z(scale){}

Vector3::Vector3(const btVector3& vec)
{
	*this = vec;
}


//!	ƒZƒbƒgŠÖ”
void Vector3::Set(f32 _x, f32 _y, f32 _z)
{
	this->x = _x;
	this->y = _y;
	this->z = _z;
}

//!	ƒZƒbƒgŠÖ”
void Vector3::Set(f32 scale)
{
	x = scale;
	y = scale;
	z = scale;
}

//!	ƒxƒNƒgƒ‹‚Ì’·‚³
f32 Vector3::LengthSq()const
{
	return x * x + y * y + z * z;
}
//!	ƒxƒNƒgƒ‹‚Ì’·‚³
f32 Vector3::Length()const
{
	return sqrtf(LengthSq() + FLT_EPSILON);
}

Vector3 Vector3::Normalize(const Vector2& vec)
{
	f32 invL = 1.0f / vec.Length();
	return Vector3(vec.x * invL, vec.y * invL, 0.0f);
}
Vector3 Vector3::Normalize(const Vector3& vec)
{
	f32 invL = 1.0f / vec.Length();
	return Vector3(vec.x * invL, vec.y * invL, vec.z * invL);
}
Vector3 Vector3::Normalize(const Vector4& vec)
{
	f32 invL = 1.0f / vec.Length();
	return Vector3(vec.x * invL, vec.y * invL, vec.z * invL);
}


Vector3 Vector3::GetRotate() const
{
	Vector3 result;
	result.x = atan2f(y, z);
	result.y = atan2f(z, x);
	result.z = atan2f(y, x);

	return result;
}

void Vector3::SetLength(f32 length)
{
	*this = Vector3::Normalize(*this);
	x = x * length;
	y = y * length;
	z = z * length;
}

//!	ŠOÏ
Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)
{
	return Vector3(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
}

//!	“àÏ
f32 Vector3::Dot(const Vector3& a, const Vector3& b)
{
	return	a.x * b.x + a.y * b.y + a.z * b.z;
}

f32 Vector3::Radian(const Vector3& a, const Vector3& b)
{
	f32 aLength = a.Length();
	f32 bLength = b.Length();
	if (aLength == 0.0f || bLength == 0.0f){ return 0.0f; }

	f32 length = 1.0f / (aLength * bLength);
	f32 cosSita = Vector3::Dot(a, b) * length;

	f32 result = acosf(cosSita);
	return result;
}

//!	üŒ`•âŠÔ
Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, f32 scale)
{
	//if(scale <= 0.0f)
	//{
	//	return a;
	//}
	//else if(scale >= 1.0f)
	//{
	//	return b;
	//}
	//else
	{
		return Vector3(a + (b - a) * scale);
	}
}

Vector3 Vector3::Rotate(const Angle& rotate) const
{
	Vector3 result;
	Vector3 ax = Vector3::Normalize(rotate._axis);
	f32 sina = sin(rotate._angle);
	f32 cosa = cos(rotate._angle);
	f32 cosb = 1.0f - cosa;

	f32 nx = x * (ax.x * ax.x * cosb +        cosa) +
			 y * (ax.x * ax.y * cosb - ax.z * sina) +
			 z * (ax.x * ax.z * cosb + ax.y * sina);

	f32 ny = x * (ax.y * ax.x * cosb + ax.z * sina) +
			 y * (ax.y * ax.y * cosb +        cosa) +
			 z * (ax.y * ax.z * cosb - ax.x * sina);

	f32 nz = x * (ax.z * ax.x * cosb - ax.y * sina) +
			 y * (ax.z * ax.y * cosb + ax.x * sina) +
			 z * (ax.z * ax.z * cosb +        cosa);
	result.Set(nx, ny, nz);

	return result;
}
Vector3 Vector3::Rotate(const Vector3& rotate) const
{
	Vector3 result;

	f32 a = cos(rotate.x);
	f32 b = sin(rotate.x);
	f32 c = cos(rotate.y);
	f32 d = sin(rotate.y);
	f32 e = cos(rotate.z);
	f32 f = sin(rotate.z);

	f32 nx =  c * e * x - c * f  * y +  d * z;
	f32 ny = (a * f + b * d * e) * x + (a * e - b * d * f) * y - b * c * z;
	f32 nz = (b * f - a * d * e) * x + (a * d * f + b * e) * y + a * c * z;

	result.Set(nx, ny, nz);

	return result;
}

bool Vector3::Match(const Vector3& vec,f32 tolerance)
{
	return (fabs(x - vec.x) < tolerance)
		&& (fabs(y - vec.y) < tolerance)
		&& (fabs(z - vec.z) < tolerance);
}

f32* Vector3::GetPtr()
{
	return (f32*)&x;
}

const f32* Vector3::GetPtr() const
{
	return (const f32*)&x;
}

f32& Vector3::operator[](s32 index)
{
	return GetPtr()[index];
}

f32  Vector3::operator[](s32 index) const
{
	return GetPtr()[index];
}

std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
	os	<< "x:" << v.x << ","
		<< "y:" << v.y << ","
		<< "z:" << v.z;

	return os;
}


//	‰‰ZŠÖ”
//!	‰ÁZ
void Vector3::Add(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}
//!	Œ¸Z
void Vector3::Subtract(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}
//!	æZ
void Vector3::Multi(const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
}
//!	œZ
void Vector3::Div(const Vector3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
}

//!	‰ÁZ
void Vector3::Add(f32 v)
{
	x += v;
	y += v;
	z += v;
}
//!	Œ¸Z
void Vector3::Subtract(f32 v)
{
	x -= v;
	y -= v;
	z -= v;
}
//!	æZ
void Vector3::Multi(f32 v)
{
	x *= v;
	y *= v;
	z *= v;
}
//!	œZ
void Vector3::Div(f32 v)
{
	x /= v;
	y /= v;
	z /= v;
}

//	ƒIƒyƒŒ[ƒ^[
//!	–{‘Ì‚Ì‰ÁZ
Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}
//!	–{‘Ì‚ÌŒ¸Z
Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}
//!	–{‘Ì‚ÌæZ
Vector3& Vector3::operator*=(const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}
//!	–{‘Ì‚ÌœZ
Vector3& Vector3::operator/=(const Vector3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;

	return *this;
}

//!	‰ÁZŒ‹‰Ê
Vector3 Vector3::operator+(const Vector3& v) const
{
	Vector3 result;
	result.x = x + v.x;
	result.y = y + v.y;
	result.z = z + v.z;
	return result;
}
//!	Œ¸ZŒ‹‰Ê
Vector3 Vector3::operator-(const Vector3& v) const
{
	Vector3 result;
	result.x = x - v.x;
	result.y = y - v.y;
	result.z = z - v.z;
	return result;
}
//!	æZŒ‹‰Ê
Vector3 Vector3::operator*(const Vector3& v) const
{
	Vector3 result;
	result.x = x * v.x;
	result.y = y * v.y;
	result.z = z * v.z;
	return result;
}
//!	œZŒ‹‰Ê
Vector3 Vector3::operator/(const Vector3& v) const
{
	Vector3 result;
	result.x = x / v.x;
	result.y = y / v.y;
	result.z = z / v.z;
	return result;
}

//!	–{‘Ì‚Ì‰ÁZ
Vector3& Vector3::operator+=(f32 v)
{
	x += v;
	y += v;
	z += v;

	return *this;
}
//!	–{‘Ì‚ÌŒ¸Z
Vector3& Vector3::operator-=(f32 v)
{
	x -= v;
	y -= v;
	z -= v;

	return *this;
}
//!	–{‘Ì‚ÌæZ
Vector3& Vector3::operator*=(f32 v)
{
	x *= v;
	y *= v;
	z *= v;

	return *this;
}
//!	–{‘Ì‚ÌœZ
Vector3& Vector3::operator/=(f32 v)
{
	x /= v;
	y /= v;
	z /= v;

	return *this;
}

//!	‰ÁZŒ‹‰Ê
Vector3 Vector3::operator+(f32 v) const
{
	Vector3 result;
	result.x = x + v;
	result.y = y + v;
	result.z = z + v;
	return result;
}
//!	Œ¸ZŒ‹‰Ê
Vector3 Vector3::operator-(f32 v) const
{
	Vector3 result;
	result.x = x - v;
	result.y = y - v;
	result.z = z - v;
	return result;
}
//!	æZŒ‹‰Ê
Vector3 Vector3::operator*(f32 v) const
{
	Vector3 result;
	result.x = x * v;
	result.y = y * v;
	result.z = z * v;
	return result;
}
//!	œZŒ‹‰Ê
Vector3 Vector3::operator/(f32 v) const
{
	Vector3 result;
	result.x = x / v;
	result.y = y / v;
	result.z = z / v;
	return result;
}

Vector3::operator Vector2() const
{
	return Vector2(x, y);
}

Vector3::operator f32*() const
{
	return (f32*)this;
}

Vector3::operator const f32*() const
{
	return (f32*)this;
}


//	”äŠr‰‰Z
bool Vector3::operator==(const Vector3& v) const
{
	return (x == v.x && y == v.y && z == v.z);
}

bool Vector3::operator!=(const Vector3& v) const
{
	return (x != v.x || y != v.y || z != v.z);
}

//	Bullet
Vector3& Vector3::operator=(const btVector3& v)
{
	x = v.x();
	y = v.y();
	z = v.z();
	return *this;
}

Vector3::operator btVector3() const
{
	return btVector3(x, y, z);
}

//=============================================================================
//!	Vector4
//=============================================================================
//! ƒRƒ“ƒXƒgƒ‰ƒNƒ^
Vector4::Vector4() :x(0), y(0), z(0), w(0){}
//! ƒRƒ“ƒXƒgƒ‰ƒNƒ^
Vector4::Vector4(f32 _x, f32 _y, f32 _z, f32 _w) : x(_x), y(_y), z(_z), w(_w){}

// ƒZƒbƒgŠÖ”
void Vector4::Set(f32 _x, f32 _y, f32 _z, f32 _w)
{
	this->x = _x;
	this->y = _y;
	this->z = _z;
	this->w = _w;
}

void Vector4::Set(Vector3 v, f32 _w)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = _w;
}

f32 Vector4::LengthSq() const
{
	return x * x + y * y + z * z + w * w;
}
f32 Vector4::Length() const
{
	return sqrtf(LengthSq() + FLT_EPSILON);
}


Vector4 Vector4::Normalize(const Vector2& vec)
{
	f32 invL = 1.0f / vec.Length();
	return Vector4(vec.x * invL, vec.y * invL, 0.0f, 0.0f);
}
Vector4 Vector4::Normalize(const Vector3& vec)
{
	f32 invL = 1.0f / vec.Length();
	return Vector4(vec.x * invL, vec.y * invL, vec.z * invL, 0.0f);
}
Vector4 Vector4::Normalize(const Vector4& vec)
{
	f32 invL = 1.0f / vec.Length();
	return Vector4(vec.x * invL, vec.y * invL, vec.z * invL, vec.w * invL);
}



f32* Vector4::GetPtr()
{
	return (f32*)&x;
}

const f32* Vector4::GetPtr() const
{
	return (const f32*)&x;
}

f32& Vector4::operator[](s32 index)
{
	return GetPtr()[index];
}

f32  Vector4::operator[](s32 index) const
{
	return GetPtr()[index];
}

std::ostream& operator<<(std::ostream& os, const Vector4& v)
{
	os	<< "x:" << v.x << ","
		<< "y:" << v.y << ","
		<< "z:" << v.z << ","
		<< "w:" << v.w;

	return os;
}

//	‰‰ZŠÖ”
//!	‰ÁZ
void Vector4::Add(const Vector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}
//!	Œ¸Z
void Vector4::Subtract(const Vector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}
//!	æZ
void Vector4::Multi(const Vector4& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
}
//!	œZ
void Vector4::Div(const Vector4& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
}

//!	‰ÁZ
void Vector4::Add(f32 v)
{
	x += v;
	y += v;
	z += v;
	w += v;
}
//!	Œ¸Z
void Vector4::Subtract(f32 v)
{
	x -= v;
	y -= v;
	z -= v;
	w -= v;
}
//!	æZ
void Vector4::Multi(f32 v)
{
	x *= v;
	y *= v;
	z *= v;
	w *= v;
}
//!	œZ
void Vector4::Div(f32 v)
{
	x /= v;
	y /= v;
	z /= v;
	w /= v;
}

//	ƒIƒyƒŒ[ƒ^[
//!	–{‘Ì‚Ì‰ÁZ
Vector4& Vector4::operator+=(const Vector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;

	return *this;
}
//!	–{‘Ì‚ÌŒ¸Z
Vector4& Vector4::operator-=(const Vector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;

	return *this;
}
//!	–{‘Ì‚ÌæZ
Vector4& Vector4::operator*=(const Vector4& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
}
//!	–{‘Ì‚ÌœZ
Vector4& Vector4::operator/=(const Vector4& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;

	return *this;
}

//!	‰ÁZŒ‹‰Ê
Vector4 Vector4::operator+(const Vector4& v) const
{
	Vector4 result;
	result.x = x + v.x;
	result.y = y + v.y;
	result.z = z + v.z;
	result.w = w + v.w;
	return result;
}
//!	Œ¸ZŒ‹‰Ê
Vector4 Vector4::operator-(const Vector4& v) const
{
	Vector4 result;
	result.x = x - v.x;
	result.y = y - v.y;
	result.z = z - v.z;
	result.w = w - v.w;
	return result;
}
//!	æZŒ‹‰Ê
Vector4 Vector4::operator*(const Vector4& v) const
{
	Vector4 result;
	result.x = x * v.x;
	result.y = y * v.y;
	result.z = z * v.z;
	result.w = w * v.w;
	return result;
}
//!	œZŒ‹‰Ê
Vector4 Vector4::operator/(const Vector4& v) const
{
	Vector4 result;
	result.x = x / v.x;
	result.y = y / v.y;
	result.z = z / v.z;
	result.w = w / v.w;
	return result;
}
//!	‰ÁZŒ‹‰Ê
Vector4 Vector4::operator+(f32 v) const
{
	Vector4 result;
	result.x = x + v;
	result.y = y + v;
	result.z = z + v;
	result.w = w + v;
	return result;
}
//!	Œ¸ZŒ‹‰Ê
Vector4 Vector4::operator-(f32 v) const
{
	Vector4 result;
	result.x = x - v;
	result.y = y - v;
	result.z = z - v;
	result.w = w - v;
	return result;
}
//!	æZŒ‹‰Ê
Vector4 Vector4::operator*(f32 v) const
{
	Vector4 result;
	result.x = x * v;
	result.y = y * v;
	result.z = z * v;
	result.w = w * v;
	return result;
}
//!	œZŒ‹‰Ê
Vector4 Vector4::operator/(f32 v) const
{
	Vector4 result;
	result.x = x / v;
	result.y = y / v;
	result.z = z / v;
	result.w = w / v;
	return result;
}

Vector4::operator Vector2()const
{
	return Vector2(x, y);
}

Vector4::operator Vector3()const
{
	return Vector3(x, y, z);
}

Vector4::operator f32* () const
{
	return (f32*)this;
}

Vector4::operator const f32* () const
{
	return (f32*)this;
}

bool Vector4::operator==(const Vector4& v) const
{
	return (x == v.x && y == v.y && z == v.z && w == v.w);
}

bool Vector4::operator!=(const Vector4& v) const
{
	return (x != v.x || y != v.y || z != v.z || w != v.w);
}

//	Bullet
Vector4& Vector4::operator=(const btVector3& v)
{
	x = v.x();
	y = v.y();
	z = v.z();
	w = 0.f;
	return *this;
}

Vector4::operator btVector3() const
{
	return btVector3(x, y, z);
}


//=============================================================================
//!	Angle
//=============================================================================
Angle::Angle()
: _axis(Vector3::ZERO)
, _angle(0)
{
}
Angle::Angle(f32 x, f32 y, f32 z, f32 angle)
: _axis(Vector3::Normalize(Vector3(x, y, z)))
, _angle(angle)
{
}
Angle::Angle(const Vector3& axis, f32 angle)
: _axis(Vector3::Normalize(axis))
, _angle(angle)
{
}
Angle::Angle(const Angle& angle)
: _axis(Vector3::Normalize(angle._axis))
, _angle(angle._angle)
{
}


Angle Angle::Normalize() const
{
	Angle result = *this;
	result.Normalized();
	return result;
}
void Angle::Normalized()
{
	_axis = Vector3::Normalize(_axis);
}

std::ostream& operator<<(std::ostream& os, const Angle& a)
{
	s32 w = 10;
	os	<< "Angle "<< a._angle << "," << std::setw(w)
		<< a._axis;

	return os;
}

const Angle Angle::operator+ (const Angle& angle)
{
	Angle result;
	result._angle = _angle + angle._angle;
	result._axis = Vector3::Normalize(_axis + angle._axis);
	return result;
}
Angle Angle::operator+=(const Angle& angle)
{
	(*this) = (*this) + angle;
	return (*this);
}
const Angle Angle::operator+ (const Vector3& axis)
{
	Angle result(*this);
	result._axis += axis;
	result._axis = Vector3::Normalize(result._axis);
	return result;
}
Angle Angle::operator+=(const Vector3& axis)
{
	_axis += axis;
	_axis = Vector3::Normalize(_axis);
	return (*this);
}
const Angle Angle::operator+ (f32 angle)
{
	Angle result(*this);
	result._angle += angle;
	return result;
}
Angle Angle::operator+=(f32 angle)
{
	_angle += angle;
	return (*this);
}

Angle& Angle::operator=(const Quaternion& v)
{
	*this = v.GetAngleRotate();
	return *this;
}

Angle::operator Quaternion() const
{
	Quaternion result(*this);
	return result;
}

//	Bullet
Angle& Angle::operator=(const btQuaternion& v)
{
	_axis  = v.getAxis();
	_angle = v.getAngle();
	return *this;
}

Angle::operator btQuaternion() const
{
	btQuaternion result;
	result.setRotation(_axis, _angle);
	return result;
}


//=============================================================================
//!	Quaternion
//=============================================================================
Quaternion::Quaternion()
:_v(0.f, 0.f, 0.f, 1.f)
{
}
Quaternion::Quaternion(f32 x, f32 y, f32 z, f32 w)
: _v(x, y, z, w)
{
}
Quaternion::Quaternion(const Vector4& v)
: _v(v)
{
}
Quaternion::Quaternion(f32 angle, const Vector3& axis)
: _v(0.f, 0.f, 0.f, 1.f)
{
	Rotate(angle, axis);
}
Quaternion::Quaternion(const Angle& angle)
{
	Rotate(angle);
}

Quaternion::Quaternion(const btQuaternion& quat)
: _v(quat.x(), quat.y(), quat.z(), quat.w())
{

}


void Quaternion::Rotate(f32 angle, f32 x, f32 y, f32 z)
{
	f32 length = sqrtf(x * x + y * y + z * z);
	if(length < FLT_EPSILON)
	{
		*this = Quaternion();
		return;
	}

	f32 inversenorm = 1.0f / length;
	f32 coshalfangle = cosf(0.5f * angle);
	f32 sinhalfangle = sinf(0.5f * angle);

	_v.x = x * sinhalfangle * inversenorm;
	_v.y = y * sinhalfangle * inversenorm;
	_v.z = z * sinhalfangle * inversenorm;
	_v.w = coshalfangle;

}
void Quaternion::Rotate(f32 angle, const Vector3& axis)
{
	Rotate(angle, axis.x, axis.y, axis.z);
}

void Quaternion::Rotate(const Angle& angle)
{
	Rotate(angle._angle, angle._axis.x, angle._axis.y, angle._axis.z);
}

void Quaternion::Rotate(f32 yaw, f32 pitch, f32 roll)
{
	f32 halfYaw = yaw   * 0.5f;
	f32 halfPitch = pitch * 0.5f;
	f32 halfRoll = roll  * 0.5f;

	f32 cosYaw = cosf(halfYaw);
	f32 sinYaw = sinf(halfYaw);
	f32 cosPitch = cosf(halfPitch);
	f32 sinPitch = sinf(halfPitch);
	f32 cosRoll = cosf(halfRoll);
	f32 sinRoll = sinf(halfRoll);

	_v.x = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
	_v.y = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
	_v.z = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
	_v.w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;

}

void Quaternion::Rotate(const Vector3& euler)
{
	Rotate(euler.x, euler.y, euler.z);
}


//!	‰ñ“]
Quaternion Quaternion::RotateAngle(f32 angle, f32 x, f32 y, f32 z)
{
	Quaternion result;
	result.Rotate(angle, x, y, z);
	return result;
}
Quaternion Quaternion::RotateAngle(f32 angle, const Vector3& axis)
{
	Quaternion result;
	result.Rotate(angle, axis);
	return result;
}
Quaternion Quaternion::RotateAngle(const Angle& angle)
{
	Quaternion result;
	result.Rotate(angle);
	return result;
}

Quaternion Quaternion::RotateEuler(f32 yaw, f32 pitch, f32 roll)
{
	Quaternion result;
	result.Rotate(yaw, pitch, roll);
	return result;
}
Quaternion Quaternion::RotateEuler(const Vector3& euler)
{
	Quaternion result;
	result.Rotate(euler);
	return result;
}

//!	‹…–ÊüŒ`•âŠÔ
Quaternion Quaternion::Slerp(const Quaternion& a, const Quaternion& b, f32 scale)
{
	f32 magnitude = sqrtf(a.Length2() * b.Length2());
	assert(magnitude > 0.f);

	f32 product = Quaternion::Dot(a,b) / magnitude;
	if(fabsf(product) < 1.f)
	{
		// Take care of long angle case see http://en.wikipedia.org/wiki/Slerp
		const f32 sign = (product < 0.f) ? -1.f : 1.f;

		const f32 theta = acosf(sign * product);
		const f32 s1    = sinf(sign * scale * theta);
		const f32 d     = 1.f / sinf(theta);
		const f32 s0    = sinf((1.f - scale) * theta);

		//return Quaternion(
		//	(a._v.x * s0 + b._v.x * s1) * d,
		//	(a._v.y * s0 + b._v.y * s1) * d,
		//	(a._v.z * s0 + b._v.z * s1) * d,
		//	(a._v.w * s0 + b._v.w * s1) * d);
		return ((a * s0) + (b * s1)) * d;
	}

	return a;
}

Angle Quaternion::GetAngleRotate()const
{
	Angle result;
	f32 squared = 1.f - _v.w * _v.w;
	if (squared < 10.f * FLT_EPSILON)
	{
		return Angle::AXIS_X;
	}
	f32 sinhalfangle = sqrtf(squared);

	f64 sha = 1.f / sinhalfangle;
	result._axis.x = _v.x * sha;
	result._axis.y = _v.y * sha;
	result._axis.z = _v.z * sha;

	f32 angle = 2.f * acosf(_v.w);
	result._angle = angle;
	result.Normalized();
	return result;
}
void Quaternion::Normalized()
{
	f32 len = _v.w*_v.w + _v.x*_v.x + _v.y*_v.y + _v.z*_v.z;
	f32 factor = 1.0f / sqrt(len + FLT_EPSILON);
	_v.x *= factor;
	_v.y *= factor;
	_v.z *= factor;
	_v.w *= factor;
}

Quaternion Quaternion::Normalize() const
{
	Quaternion result = *this;
	result.Normalized();
	return result;
}

f32 Quaternion::Dot(const Quaternion& a, const Quaternion& b)
{
	f32 result = a._v.x * b._v.x +
				 a._v.y * b._v.y +
				 a._v.z * b._v.z +
				 a._v.w * b._v.w;
	return result;
}
f32 Quaternion::Length()const
{
	return sqrtf(Length2());
}
f32 Quaternion::Length2()const
{
	return Quaternion::Dot(*this, *this);
}


const Quaternion Quaternion::Inverse() const
{
	return Quaternion(-_v.x, -_v.y, -_v.z, -_v.w);
}

std::ostream& operator<<(std::ostream& os, const Quaternion& q)
{
	os	<< q._v;
	return os;
}


const Quaternion Quaternion::operator* (f32 scale) const
{
	return Quaternion(_v.x * scale, _v.y * scale, _v.z * scale, _v.w * scale);
}
Quaternion& Quaternion::operator*=(f32 scale)
{
	(*this) = (*this) * scale;
	return *this;
}
const Quaternion Quaternion::operator* (const Quaternion& quat) const
{
	return Quaternion(quat._v.w*_v.x + quat._v.x*_v.w + quat._v.y*_v.z - quat._v.z*_v.y,
		quat._v.w*_v.y - quat._v.x*_v.z + quat._v.y*_v.w + quat._v.z*_v.x,
		quat._v.w*_v.z + quat._v.x*_v.y - quat._v.y*_v.x + quat._v.z*_v.w,
		quat._v.w*_v.w - quat._v.x*_v.x - quat._v.y*_v.y - quat._v.z*_v.z);
}
Quaternion& Quaternion::operator*=(const Quaternion& quat)
{
	(*this) = (*this) * quat;
	return *this;
}
Quaternion Quaternion::operator/ (f32 scale) const
{
	f32 div = 1.0f / scale;
	return Quaternion(_v.x * div, _v.y * div, _v.z * div, _v.w * div);
}
Quaternion& Quaternion::operator/=(f32 scale)
{
	(*this) = (*this) / scale;
	return *this;
}

const Quaternion Quaternion::operator+ (const Quaternion& quat) const
{
	return Quaternion(_v.x + quat._v.x, _v.y + quat._v.y,
		_v.z + quat._v.z, _v.w + quat._v.w);
}
//Quaternion& Quaternion::operator+=(const Quaternion& quat)
//{
//	(*this) = (*this) + quat;
//	return *this;
//}
const Quaternion Quaternion::operator- (const Quaternion& quat) const
{
	return Quaternion(_v.x - quat._v.x, _v.y - quat._v.y,
		_v.z - quat._v.z, _v.w - quat._v.w);
}
//Quaternion& Quaternion::operator-=(const Quaternion& quat)
//{
//	(*this) = (*this) - quat;
//	return *this;
//}
const Quaternion Quaternion::operator- () const
{
	return Quaternion(-_v.x, -_v.y, -_v.z, -_v.w);
}

Vector3 Quaternion::operator* (const Vector3& v) const
{
	Vector3 uv, uuv;
	Vector3 qvec(_v.x, _v.y, _v.z);
	uv = Vector3::Cross(qvec, v);
	uuv = Vector3::Cross(qvec, uv);
	uv *= (2.0f * _v.w);
	uuv *= 2.0f;
	return v + uv + uuv;
}
//
//Quaternion& Quaternion::operator = (const Angle& v)
//{
//	Rotate(v);
//	return *this;
//}
//
//Quaternion::operator Angle() const
//{
//	return GetAngleRotate();
//}

//	Bullet
Quaternion& Quaternion::operator=(const btQuaternion& v)
{
	_v.x = v.x();
	_v.y = v.y();
	_v.z = v.z();
	_v.w = v.w();
	return *this;
}

Quaternion::operator btQuaternion() const
{
	btQuaternion result(_v.x, _v.y, _v.z, _v.w);
	return result;
}


// TODO Vector2‚ğTemplate‚É‚µ‚Ä‹¤’Ê‰»‚·‚é

//----	int

struct Vector2i
{
	Vector2i()
	:x(0)
	,y(0)
	{}
	Vector2i(s32 _x,s32 _y)
	:x(_x)
	,y(_y)
	{}

	void Set(s32 _x,s32 _y)
	{
		this->x = _x;
		this->y = _y;
	}

	void Set(const Vector2i& vec)
	{
		Set(vec.x, vec.y);
	}

	Vector2 GetFloat()const
	{
		return Vector2((f32)x,(f32)y);
	}

	//	‰‰ZŠÖ”
	//!	‰ÁZ
	void Add(const Vector2i& v)
	{
		x += v.x;
		y += v.y;
	}
	//!	Œ¸Z
	void Subtract(const Vector2i& v)
	{
		x -= v.x;
		y -= v.y;
	}
	//!	æZ
	void Multi(const Vector2i& v)
	{
		x *= v.x;
		y *= v.y;
	}

	//!	‰ÁZ
	void Add(s32 v)
	{
		x += v;
		y += v;
	}
	//!	Œ¸Z
	void Subtract(s32 v)
	{
		x -= v;
		y -= v;
	}
	//!	æZ
	void Multi(s32 v)
	{
		x *= v;
		y *= v;
	}

	//	ƒIƒyƒŒ[ƒ^[
	//!	–{‘Ì‚Ì‰ÁZ
	Vector2i operator += (const Vector2i& v)
	{
		x += v.x;
		y += v.y;

		return *this;
	}
	//!	–{‘Ì‚ÌŒ¸Z
	Vector2i operator -= (const Vector2i& v)
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}
	//!	–{‘Ì‚ÌæZ
	Vector2i operator *= (const Vector2i& v)
	{
		x *= v.x;
		y *= v.y;

		return *this;
	}

	//!	‰ÁZŒ‹‰Ê
	Vector2i operator + (const Vector2i& v) const
	{
		Vector2i result;
		result.x = x + v.x;
		result.y = y + v.y;
		return result;
	}
	//!	Œ¸ZŒ‹‰Ê
	Vector2i operator - (const Vector2i& v) const
	{
		Vector2i result;
		result.x = x - v.x;
		result.y = y - v.y;
		return result;
	}
	//!	æZŒ‹‰Ê
	Vector2i operator * (const Vector2i& v) const
	{
		Vector2i result;
		result.x = x * v.x;
		result.y = y * v.y;
		return result;
	}

	operator Vector2()const
	{
		return Vector2((f32)x, (f32)y);
	}


	static const Vector2i ZERO;
	static const Vector2i ONE;
	static const Vector2i MINUS;

	s32 x;
	s32 y;
};

struct Vector3i
{
	Vector3i()
	:x(0)
	,y(0)
	,z(0)
	{}
	Vector3i(s32 _x,s32 _y,s32 _z)
	:x(_x)
	,y(_y)
	,z(_z)
	{}

	void Set(s32 _x,s32 _y,s32 _z)
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
	}

	void Set(const Vector3i& vec)
	{
		Set(vec.x, vec.y, vec.z);
	}

	Vector3 GetFloat()const
	{
		return Vector3((f32)x,(f32)y,(f32)z);
	}


	//	‰‰ZŠÖ”
	//!	‰ÁZ
	void Add(const Vector3i& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}
	//!	Œ¸Z
	void Subtract(const Vector3i& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}
	//!	æZ
	void Multi(const Vector3i& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
	}

	//!	‰ÁZ
	void Add(s32 v)
	{
		x += v;
		y += v;
		z += v;
	}
	//!	Œ¸Z
	void Subtract(s32 v)
	{
		x -= v;
		y -= v;
		z -= v;
	}
	//!	æZ
	void Multi(s32 v)
	{
		x *= v;
		y *= v;
		z *= v;
	}

	//	ƒIƒyƒŒ[ƒ^[
	//!	–{‘Ì‚Ì‰ÁZ
	Vector3i operator += (const Vector3i& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}
	//!	–{‘Ì‚ÌŒ¸Z
	Vector3i operator -= (const Vector3i& v)
	{
		y -= v.y;
		x -= v.x;
		z -= v.z;

		return *this;
	}
	//!	–{‘Ì‚ÌæZ
	Vector3i operator *= (const Vector3i& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;

		return *this;
	}

	//!	‰ÁZŒ‹‰Ê
	Vector3i operator + (const Vector3i& v) const
	{
		Vector3i result;
		result.x = x + v.x;
		result.y = y + v.y;
		result.z = z + v.z;
		return result;
	}
	//!	Œ¸ZŒ‹‰Ê
	Vector3i operator - (const Vector3i& v) const
	{
		Vector3i result;
		result.x = x - v.x;
		result.y = y - v.y;
		result.z = z - v.z;
		return result;
	}
	//!	æZŒ‹‰Ê
	Vector3i operator * (const Vector3i& v) const
	{
		Vector3i result;
		result.x = x * v.x;
		result.y = y * v.y;
		result.z = z * v.z;
		return result;
	}


	operator Vector3()const
	{
		return Vector3((f32)x, (f32)y, (f32)z);
	}
	static const Vector3i ZERO;
	static const Vector3i ONE;
	static const Vector3i MINUS;

	s32 x;
	s32 y;
	s32 z;
};



namespace Math
{
Vector2 RandomVector2();
Vector3 RandomVector3();
Vector4 RandomVector4();
}



