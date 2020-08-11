//=============================================================================
//!
//!	@file	Matrix.h
//!	@brief	�}�g���b�N�X�\����
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

struct Matrix
{
	//!	�R���X�g���N�^
	MY_INLINE	Matrix();

	//!	�R���X�g���N�^
	//!	@param	[in]	m11~m44		�}�g���b�N�X
	MY_INLINE	Matrix( f32 m11,f32 m12,f32 m13,f32 m14,
						f32 m21,f32 m22,f32 m23,f32 m24,
						f32 m31,f32 m32,f32 m33,f32 m34,
						f32 m41,f32 m42,f32 m43,f32 m44);

	//!	�R���X�g���N�^
	//!	@param	[in]	mat1~mat4		�}�g���b�N�X
	MY_INLINE	Matrix(const Vector4& mat1, const Vector4& mat2, const Vector4& mat3, const Vector4& mat4);

	//!	�R���X�g���N�^
	//!	@param	[in]	position	���W
	MY_INLINE	Matrix(const Vector3& position);

	//!	�R���X�g���N�^
	//!	@param	[in]	trans	btTransform
	MY_INLINE	Matrix(const btTransform& trans);

	//!	���W�l�擾
	//!	@return	���W
	MY_INLINE	Vector3 GetPosition() const;
	//!	X�x�N�g���擾
	//!	@return	X�x�N�g��
	MY_INLINE	Vector3 GetXVector() const;
	//!	Y�x�N�g���擾
	//!	@return	Y�x�N�g��
	MY_INLINE	Vector3 GetYVector() const;
	//!	Z�x�N�g���擾
	//!	@return	Z�x�N�g��
	MY_INLINE	Vector3 GetZVector() const;
	//!	��]�p�擾(Vector3)
	//!	@return	��]�I�C���[�p
	MY_INLINE	Vector3 GetRotateVector() const;
	//!	��]�p�擾(Quaternion)
	//!	@return	��]�N�H�[�^�j�I��
	MY_INLINE	Quaternion GetRotateQuaternion() const;
	//!	��]�p�擾(Angle)
	//!	@return	��]�A���O��
	MY_INLINE	Angle GetRotateAngle() const;

	//!	�X�P�[���l�擾
	//!	@return	�X�P�[���l
	MY_INLINE	Vector3 GetScale() const;

	//!	���K��
	//!	@return	���K���s��
	MY_INLINE	static Matrix Normalize(const Matrix& matrix);

	//!	X����]
	//!	@param	[in]	angle	��](radian)
	MY_INLINE	static Matrix RotateX(f32 angle);
	//!	Y����]
	//!	@param	[in]	angle	��](radian)
	MY_INLINE	static Matrix RotateY(f32 angle);

	//!	Z����]
	//!	@param	[in]	angle	��](radian)
	MY_INLINE	static Matrix RotateZ(f32 angle);

	//!	�C�ӎ���]
	//!	@param	[in]	v		��]���S��
	//!	@param	[in]	angle	��](radian)
	MY_INLINE	static Matrix	Rotate(const Vector3& v, f32 angle);

	//!	�C�ӎ���]
	//!	@param	[in]	angle	��](radian)
	MY_INLINE	static Matrix	Rotate(const Vector4& rotate);

	//!	�C�ӎ���]
	//!	@param	[in]	angle	��](radian)
	MY_INLINE	static Matrix	Rotate(const Angle& rotate);

	//!	�C�ӎ���]
	//!	@param	[in]	angle	��](radian)
	MY_INLINE	static Matrix	Rotate(const Quaternion& quat);

	//!	��]
	//!	@param	[in]	angle	��](radian)
	MY_INLINE	static Matrix	Rotate(const Vector3& angle);


	//	�A�t�B���ϊ�
	//	@param [in]  position    �ʒu
	//	@param [in]  rotation    ��]�p
	//	@param [in]  scale       �g�k
	//	@return �A�t�B���ϊ����ꂽ�s��
	MY_INLINE	static Matrix Affine(const Vector3& position, const Vector3& rotation, const Vector3& scale);

	//	�A�t�B���ϊ�
	//	@param [in]  position    �ʒu
	//	@param [in]  rotation    ��]�p
	//	@param [in]  scale       �g�k
	//	@return �A�t�B���ϊ����ꂽ�s��
	MY_INLINE	static Matrix Affine(const Vector3& position, const Angle& rotation, const Vector3& scale);

	//	�A�t�B���ϊ�
	//	@param [in]  position    �ʒu
	//	@param [in]  rotation    ��]�p
	//	@param [in]  scale       �g�k
	//	@return �A�t�B���ϊ����ꂽ�s��
	MY_INLINE	static Matrix Affine(const Vector3& position, const Quaternion& rotation, const Vector3& scale);

	//!	���W���o��
	//!	@return	�}�g���b�N�X���W
	MY_INLINE	Vector3 GetVector()	const;
	//!	X���W�ړ�
	//!	@param	[in]	x	���W
	MY_INLINE	static Matrix TranslateX(f32 x);

	//!	Y���W�ړ�
	//!	@param	[in]	y	���W
	MY_INLINE	static Matrix TranslateY(f32 y);

	//!	Z���W�ړ�
	//!	@param	[in]	z	���W
	MY_INLINE	static Matrix TranslateZ(f32 z);
	//!	���W�ړ�
	//!	@param	[in]	pos	���W
	MY_INLINE	static Matrix Translate(const Vector3& pos);
	//!	���W�ړ�
	//!	@param	[in]	x	���W
	//!	@param	[in]	y	���W
	//!	@param	[in]	z	���W
	MY_INLINE	static Matrix Translate(f32 x, f32 y, f32 z);


	//!	�g��k�� X
	//!	@param	[in]	x	�X�P�[��
	MY_INLINE	static Matrix ScaleX(f32 x);
	//!	�g��k�� Y
	//!	@param	[in]	y	�X�P�[��
	MY_INLINE	static Matrix ScaleY(f32 y);

	//!	�g��k�� Z
	//!	@param	[in]	z	�X�P�[��
	MY_INLINE	static Matrix ScaleZ(f32 z);
	//!	�g��k��
	//!	@param	[in]	scale	�X�P�[��
	MY_INLINE	static Matrix Scale(const Vector3& scale);
	//!	�g��k��
	//!	@param	[in]	scale	�X�P�[��
	MY_INLINE	static Matrix Scale(f32 sx, f32 sy, f32 sz);
	//!	�g��k��
	//!	@param	[in]	scale	�X�P�[��
	MY_INLINE	static Matrix Scale(f32 scale);

	//!	���W�ݒ�
	//!	@param	[in]	position	���W
	MY_INLINE	void SetTranslate(const Vector3& position);
	//!	��]�ݒ�
	//!	@param	[in]	rotate		��]
	MY_INLINE	void SetRotate(const Vector3& rotate);
	//!	��]�ݒ�
	//!	@param	[in]	rotate		��]
	MY_INLINE	void SetRotate(const Quaternion& rotate);
	//!	��]�ݒ�
	//!	@param	[in]	rotate		��]
	MY_INLINE	void SetRotate(const Angle& rotate);
	//!	�X�P�[���ݒ�
	//!	@param	[in]	scale		�X�P�[��
	MY_INLINE	void SetScale(const Vector3& scale);

	MY_INLINE	Vector3 PostMult(const Vector3& v) const;
	Vector3 operator*(const Vector3& v) const
	{
		return PostMult(v);
	}

	MY_INLINE	Vector4 PostMult(const Vector4& v) const;
	Vector4 operator* (const Vector4& v) const
	{
		return PostMult(v);
	}

	//!	��]���̃}�g���b�N�X
	MY_INLINE	static Matrix RotateMatrix(const Matrix& matrix);
	//!	�t�s��
	MY_INLINE	static Matrix Invert(const Matrix& matrix);
	//!	�t�s��
	MY_INLINE	Matrix GetInvert()const;

	//!	��Z
	//!	@param	[in]	a	�}�g���b�N�X
	//!	@param	[in]	b	�}�g���b�N�X
	//!	@return	�}�g���b�N�X���|�����킹������
	MY_INLINE	static Matrix Mult(const Matrix& a, const Matrix& b);

	//!	�J�����ʒu�r���[�s��
	//!	@param	[in]	position	���W
	//!	@param	[in]	lookAt		���_
	//!	@param	[in]	worldUp		�J������̕���	default:(0,1,0)
	static Matrix LookAtViewMatrix(
			const Vector3& position,
			const Vector3& lookAt,
			const Vector3& worldUp = Vector3(0, 1, 0));

	//!	�J�����ʒu�s��
	//!	@param	[in]	position	���W
	//!	@param	[in]	lookAt		���_
	//!	@param	[in]	worldUp		�J������̕���	default:(0,1,0)
	static Matrix LookAtMatrix(
			const Vector3& position,
			const Vector3& lookAt,
			const Vector3& worldUp = Vector3(0, 1, 0));

	//!	�������e
	//!	@param	[in]	fovy	�c�̎���p
	//!	@param	[in]	aspect	��ʔ䗦
	//!	@param	[in]	znear	��O
	//!	@param	[in]	zfar	���s��
	static Matrix PerspectiveMatrix(f32 fovy, f32 aspect, f32 znear, f32 zfar);
	//!	���s���e
	//!	@param	[in]	left	��
	//!	@param	[in]	right	�E
	//!	@param	[in]	bottom	��
	//!	@param	[in]	top		��
	//!	@param	[in]	znear	��O
	//!	@param	[in]	zfar	���s��
	static Matrix OrthoMatrix(f32 left, f32 right, f32 bottom, f32 top, f32 znear, f32 zfar);

	//!	���s���e
	//!	@param	[in]	left	��
	//!	@param	[in]	right	�E
	//!	@param	[in]	bottom	��
	//!	@param	[in]	top		��
	MY_INLINE	static Matrix Ortho2DMatrix(f32 left, f32 right, f32 bottom, f32 top);

	//!	@param	[in]	left	��
	//!	@param	[in]	right	�E
	//!	@param	[in]	bottom	��
	//!	@param	[in]	top		��
	//!	@param	[in]	znear	��O
	//!	@param	[in]	zfar	���s��
	MY_INLINE	static Matrix FrustumMatrix(f32 left, f32 right, f32 bottom, f32 top, f32 znear, f32 zfar);


	//!	��Z(�I�y���[�^�[)
	MY_INLINE	Matrix  operator* (const Matrix& mat) const;
	//!	��Z(�I�y���[�^�[)
	MY_INLINE	Matrix& operator*=(const Matrix& mat);

	MY_INLINE	bool operator==(const Matrix& mat) const;
	MY_INLINE	bool operator!=(const Matrix& mat) const;

	MY_INLINE	Vector4& operator[](s32 index);
	MY_INLINE	Vector4  operator[](s32 index)const;

	MY_INLINE	operator f32* ();
	MY_INLINE	operator const f32* ()const;

	//	stream
	friend MY_INLINE std::ostream& operator<<(std::ostream& os, const Matrix& m);
	friend MY_INLINE std::istream& operator>>(std::istream& is, Matrix& m);


	//	bulet
	MY_INLINE	Matrix& operator=(const btTransform& trans);



	//!	�P�ʍs��
	static Matrix IDENTITY;
	//!	0�s��
	static Matrix ZERO;

private:
	Vector4 _m[4];			//!< �}�g���b�N�X�v�f

};

//!	�R���X�g���N�^
Matrix::Matrix()
{
	_m[0].Set(1.0f, 0.0f, 0.0f, 0.0f);
	_m[1].Set(0.0f, 1.0f, 0.0f, 0.0f);
	_m[2].Set(0.0f, 0.0f, 1.0f, 0.0f);
	_m[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
}

//!	�R���X�g���N�^
//!	@param	[in]	m11~m44		�}�g���b�N�X
Matrix::Matrix( f32 m11, f32 m12, f32 m13, f32 m14,
				f32 m21, f32 m22, f32 m23, f32 m24,
				f32 m31, f32 m32, f32 m33, f32 m34,
				f32 m41, f32 m42, f32 m43, f32 m44)
{
	_m[0].Set(m11, m12, m13, m14);
	_m[1].Set(m21, m22, m23, m24);
	_m[2].Set(m31, m32, m33, m34);
	_m[3].Set(m41, m42, m43, m44);
}

//!	�R���X�g���N�^
//!	@param	[in]	mat1~mat4		�}�g���b�N�X
Matrix::Matrix(const Vector4& mat1, const Vector4& mat2, const Vector4& mat3, const Vector4& mat4)
{
	_m[0] = mat1;
	_m[1] = mat2;
	_m[2] = mat3;
	_m[3] = mat4;
}

//!	���W�}�g���b�N�X
//!	@param	[in]	position	���W
Matrix::Matrix(const Vector3& position)
{
	_m[0].Set(1.0f, 0.0f, 0.0f, 0.0f);
	_m[1].Set(0.0f, 1.0f, 0.0f, 0.0f);
	_m[2].Set(0.0f, 0.0f, 1.0f, 0.0f);
	_m[3].Set(position.x, position.y, position.z, 1.0f);
}

//!	�R���X�g���N�^
//!	@param	[in]	trans	btTransform
Matrix::Matrix(const btTransform& trans)
{
	Matrix m;
	trans.getOpenGLMatrix(m);
	*this = m;
}


//!	���W�l�擾
//!	@return	���W
Vector3 Matrix::GetPosition() const
{
	return Vector3(_m[3]);
}
//!	X�x�N�g���擾
//!	@return	X�x�N�g��
Vector3 Matrix::GetXVector() const
{
	return Vector3(_m[0]);
}
//!	Y�x�N�g���擾
//!	@return	Y�x�N�g��
Vector3 Matrix::GetYVector() const
{
	return Vector3(_m[1]);
}
//!	Z�x�N�g���擾
//!	@return	Z�x�N�g��
Vector3 Matrix::GetZVector() const
{
	return Vector3(_m[2]);
}

//!	��]�p�擾(Vector3)
//!	@return	��]�I�C���[�p
Vector3 Matrix::GetRotateVector() const
{
	static const Vector3 xidty = Matrix::IDENTITY.GetXVector();
	static const Vector3 yidty = Matrix::IDENTITY.GetYVector();
	static const Vector3 zidty = Matrix::IDENTITY.GetZVector();

	Vector3 result;
	Vector3 xv = GetXVector();
	Vector3 yv = GetYVector();
	Vector3 zv = GetZVector();

	f32 x = Vector3::Radian(xidty, xv);
	f32 y = Vector3::Radian(yidty, yv);
	f32 z = Vector3::Radian(zidty, zv);

	Vector3 vec = (xv + yv + zv);

	result = vec.GetRotate();

	return Vector3(x,y,z);
}


//!	��]�p�擾(Quaternion)
//!	@return	��]�N�H�[�^�j�I��
Quaternion Matrix::GetRotateQuaternion() const
{
	Quaternion result;

	Matrix mat = *this;
	Vector3 vs = mat.GetScale();
	mat.Scale(1.f / vs.x, 1.f / vs.y, 1.f / vs.z);

//	Vector4* m = mat.m;

	f32 tr, s;
	f32 tq[4];
	s32 i, j, k;

	s32 nxt[3] = { 1, 2, 0 };

	tr = mat[0][0] + mat[1][1] + mat[2][2] + 1.0;

	if (tr > 0.0)
	{
		s = sqrtf(tr);
		result._v.w = s / 2.0;
		s = 0.5 / s;
		result._v.x = (mat[1][2] - mat[2][1]) * s;
		result._v.y = (mat[2][0] - mat[0][2]) * s;
		result._v.z = (mat[0][1] - mat[1][0]) * s;
	}
	else
	{
		i = 0;
		if (mat[1][1] > mat[0][0])
			i = 1;
		if (mat[2][2] > mat[i][i])
			i = 2;
		j = nxt[i];
		k = nxt[j];

		s = sqrtf((mat[i][i] - (mat[j][j] + mat[k][k])) + 1.0);

		tq[i] = s * 0.5;

		if (s != 0.0)
			s = 0.5 / s;

		tq[3] = (mat[j][k] - mat[k][j]) * s;
		tq[j] = (mat[i][j] + mat[j][i]) * s;
		tq[k] = (mat[i][k] + mat[k][i]) * s;

		result._v.x = tq[0];
		result._v.y = tq[1];
		result._v.z = tq[2];
		result._v.w = tq[3];
	}

	return result;
}

//!	��]�p�擾(Angle)
//!	@return	��]�A���O��
Angle Matrix::GetRotateAngle() const
{
	return GetRotateQuaternion().GetAngleRotate();
}

//!	�X�P�[���l�擾
//!	@return	�X�P�[���l
Vector3 Matrix::GetScale() const
{
	Vector3 x(_m[0][0], _m[1][0], _m[2][0]);
	Vector3 y(_m[0][1], _m[1][1], _m[2][1]);
	Vector3 z(_m[0][2], _m[1][2], _m[2][2]);
	return Vector3(x.Length(), y.Length(), z.Length());
}


//!	���K��
//!	@return	���K���s��
Matrix Matrix::Normalize(const Matrix& matrix)
{
	Matrix result = matrix;

	result[0] = Vector3::Normalize(result[0]);
	result[1] = Vector3::Normalize(result[1]);
	result[2] = Vector3::Normalize(result[2]);

	return result;
}

//!	X����]
//!	@param	[in]	angle	��](radian)
Matrix Matrix::RotateX(f32 angle)
{
	f32	c = cosf(angle);
	f32	s = sinf(angle);

	return Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f,    c,    s, 0.0f,
		0.0f,   -s,    c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
}

//!	Y����]
//!	@param	[in]	angle	��](radian)
Matrix Matrix::RotateY(f32 angle)
{
	f32	c = cosf(angle);
	f32	s = sinf(angle);

	return Matrix(
		   c, 0.0f,   -s, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		   s, 0.0f,    c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
}

//!	Z����]
//!	@param	[in]	angle	��](radian)
Matrix Matrix::RotateZ(f32 angle)
{
	f32	c = cosf(angle);
	f32	s = sinf(angle);

	return Matrix(
		   c,    s, 0.0f, 0.0f,
		  -s,    c, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
}

//!	�C�ӎ���]
//!	@param	[in]	v		��]���S��
//!	@param	[in]	angle	��](radian)
Matrix Matrix::Rotate(const Vector3& v, f32 angle)
{
	f32	c = cosf(angle);
	f32	s = sinf(-angle);
	f32	x = v.x;
	f32	y = v.y;
	f32	z = v.z;

	return Matrix(
			x * x * (1.0f - c) + c    , x * y * (1.0f - c) - z * s, z * x * (1.0f - c) + y * s, 0.0f,
			x * y * (1.0f - c) + z * s, y * y * (1.0f - c) + c    , y * z * (1.0f - c) - x * s, 0.0f,
			z * x * (1.0f - c) - y * s, y * z * (1.0f - c) + x * s, z * z * (1.0f - c) + c    , 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
}

//!	�C�ӎ���]
//!	@param	[in]	angle	��](radian)
Matrix Matrix::Rotate(const Vector4& rotate)
{
	return (Rotate(Vector3(rotate.x, rotate.y, rotate.z), rotate.w));
}

//!	�C�ӎ���]
//!	@param	[in]	angle	��](radian)
Matrix Matrix::Rotate(const Angle& rotate)
{
	return (Rotate(rotate._axis, rotate._angle));
}

//!	�C�ӎ���]
//!	@param	[in]	angle	��](radian)
Matrix Matrix::Rotate(const Quaternion& rotation)
{
	f32 d = rotation.Length2();
	assert(d != 0.0f);
	f32 s = 2.0f / d;

	Matrix result;

	f32 xs = rotation._v.x * s, ys  = rotation._v.y * s, zs  = rotation._v.z * s;

	f32 wx = rotation._v.w * xs, wy = rotation._v.w * ys, wz = rotation._v.w * zs;
	f32 xx = rotation._v.x * xs, xy = rotation._v.x * ys, xz = rotation._v.x * zs;
	f32 yy = rotation._v.y * ys, yz = rotation._v.y * zs, zz = rotation._v.z * zs;

	result[0].Set( (1.0f - (yy + zz)),
						   (xy + wz) ,
						   (xz - wy) ,
						   0.0f);

	result[1].Set(		   (xy - wz) ,
				   (1.0f - (xx + zz)),
						   (yz + wx) ,
						   0.0f);

	result[2].Set(		   (xz + wy) ,
						   (yz - wx) ,
				   (1.0f - (xx + yy)),
						   0.0f);

	result[3].Set(0.0f, 0.0f, 0.0f, 1.0f);

	return result;
//	return Rotate(q.GetAngleRotate());
}


//!	��]
//!	@param	[in]	angle	��](radian)
Matrix Matrix::Rotate(const Vector3& rotation)
{
	Vector3 s(sin(rotation.x), sin(rotation.y), sin(rotation.z));
	Vector3 c(cos(rotation.x), cos(rotation.y), cos(rotation.z));

	Matrix mat;
	mat[0].x = (c.y * c.z);
	mat[0].y = (c.y * s.z);
	mat[0].z = (-s.y);
	mat[0].w = 0.0f;

	mat[1].x = ((s.x * s.y * c.z) - (c.x * s.z));
	mat[1].y = ((s.x * s.y * s.z) + (c.x * c.z));
	mat[1].z = ( s.x * c.y);
	mat[1].w = 0.0f;

	mat[2].x = ((c.x * s.y * c.z) + (s.x * s.z));
	mat[2].y = ((c.x * s.y * s.z) - (s.x * c.z));
	mat[2].z = ( c.x * c.y);
	mat[2].w = 0.0f;

	mat[3].x = 0.0f;
	mat[3].y = 0.0f;
	mat[3].z = 0.0f;
	mat[3].w = 1.0f;

	return mat;
}


//	�A�t�B���ϊ�
//	@param [in]  position    �ʒu
//	@param [in]  rotation    ��]�p
//	@param [in]  scale       �g�k
//	@return �A�t�B���ϊ����ꂽ�s��
Matrix Matrix::Affine(const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
	Vector3 s(sin(rotation.x), sin(rotation.y), sin(rotation.z));
	Vector3 c(cos(rotation.x), cos(rotation.y), cos(rotation.z));

	Matrix result;
	result[0].x = (c.y * c.z)	* scale.x;
	result[0].y = (c.y * s.z)	* scale.x;
	result[0].z = (-s.y)		* scale.x;
	result[0].w = 0.0f;

	result[1].x = ((s.x * s.y * c.z) - (c.x * s.z)) * scale.y;
	result[1].y = ((s.x * s.y * s.z) + (c.x * c.z)) * scale.y;
	result[1].z = (s.x * c.y)						 * scale.y;
	result[1].w = 0.0f;

	result[2].x = ((c.x * s.y * c.z) + (s.x * s.z)) * scale.z;
	result[2].y = ((c.x * s.y * s.z) - (s.x * c.z)) * scale.z;
	result[2].z = (c.x * c.y)						 * scale.z;
	result[2].w = 0.0f;

	result[3].x = position.x;
	result[3].y = position.y;
	result[3].z = position.z;
	result[3].w = 1.0f;

	return result;
}


//	�A�t�B���ϊ�
//	@param [in]  position    �ʒu
//	@param [in]  rotation    ��]�p
//	@param [in]  scale       �g�k
//	@return �A�t�B���ϊ����ꂽ�s��
Matrix Matrix::Affine(const Vector3& position, const Angle& rotation, const Vector3& scale)
{

	f32	c = cosf(rotation._angle);
	f32	s = sinf(-rotation._angle);
	f32	x = rotation._axis.x;
	f32	y = rotation._axis.y;
	f32	z = rotation._axis.z;
	Matrix result;
	result[0].Set(
		(x * x * (1.0f - c) + c    ) * scale.x,
		(x * y * (1.0f - c) - z * s) * scale.x,
		(z * x * (1.0f - c) + y * s) * scale.x,
		0.0f
	);

	result[1].Set(
		(x * y * (1.0f - c) + z * s) * scale.y,
		(y * y * (1.0f - c) + c    ) * scale.y,
		(y * z * (1.0f - c) - x * s) * scale.y,
		0.0f
	);

	result[2].Set(
		(z * x * (1.0f - c) - y * s) * scale.z,
		(y * z * (1.0f - c) + x * s) * scale.z,
		(z * z * (1.0f - c) + c    ) * scale.z,
		0.0f
	);

	result[3].Set(position, 1.0f);

	return result;
}


//	�A�t�B���ϊ�
//	@param [in]  position    �ʒu
//	@param [in]  rotation    ��]�p
//	@param [in]  scale       �g�k
//	@return �A�t�B���ϊ����ꂽ�s��
Matrix Matrix::Affine(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
{
	f32 d = rotation.Length2();
	assert(d != 0.0f);
	f32 s = 2.0f / d;

	Matrix result;

	f32 xs = rotation._v.x * s, ys  = rotation._v.y * s, zs  = rotation._v.z * s;

	f32 wx = rotation._v.w * xs, wy = rotation._v.w * ys, wz = rotation._v.w * zs;
	f32 xx = rotation._v.x * xs, xy = rotation._v.x * ys, xz = rotation._v.x * zs;
	f32 yy = rotation._v.y * ys, yz = rotation._v.y * zs, zz = rotation._v.z * zs;

	result[0].Set( (1.0f - (yy + zz)) * scale.x,
						   (xy + wz)  * scale.x,
						   (xz - wy)  * scale.x,
						   0.0f);

	result[1].Set(		   (xy - wz)  * scale.y,
				   (1.0f - (xx + zz)) * scale.y,
						   (yz + wx)  * scale.y,
						   0.0f);

	result[2].Set(		   (xz + wy)  * scale.z,
						   (yz - wx)  * scale.z,
				   (1.0f - (xx + yy)) * scale.z,
						   0.0f);

	result[3].Set(position, 1.0f);

	return result;

}



//!	���W���o��
//!	@return	�}�g���b�N�X���W
Vector3 Matrix::GetVector()	const
{
	return Vector3(_m[3].x, _m[3].y, _m[3].z);
}


//!	X���W�ړ�
//!	@param	[in]	x	���W
Matrix Matrix::TranslateX(f32 x)
{
	return Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		   x, 0.0f, 0.0f, 1.0f
		);
}

//!	Y���W�ړ�
//!	@param	[in]	y	���W
Matrix Matrix::TranslateY(f32 y)
{
	return Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f,    y, 0.0f, 1.0f
		);
}

//!	Z���W�ړ�
//!	@param	[in]	z	���W
Matrix Matrix::TranslateZ(f32 z)
{
	return Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f,    z, 1.0f
		);
}
//!	���W�ړ�
//!	@param	[in]	pos	���W
Matrix Matrix::Translate(const Vector3& pos)
{
	return Matrix(
		 1.0f,  0.0f,  0.0f, 0.0f,
		 0.0f,  1.0f,  0.0f, 0.0f,
		 0.0f,  0.0f,  1.0f, 0.0f,
		pos.x, pos.y, pos.z, 1.0f
		);
}

//!	���W�ړ�
//!	@param	[in]	x	���W
//!	@param	[in]	y	���W
//!	@param	[in]	z	���W
Matrix Matrix::Translate(f32 x, f32 y, f32 z)
{
	return Translate(Vector3(x, y, z));
}


//!	�g��k�� X
//!	@param	[in]	x	�L�k�l
Matrix Matrix::ScaleX(f32 x)
{
	return Matrix(
		   x, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

//!	�g��k�� Y
//!	@param	[in]	y	�L�k�l
Matrix Matrix::ScaleY(f32 y)
{
	return Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f,    y, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

//!	�g��k�� Z
//!	@param	[in]	z	�L�k�l
Matrix Matrix::ScaleZ(f32 z)
{
	return Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f,    z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

//!	�g��k��
//!	@param	[in]	scale	�L�k�l
Matrix Matrix::Scale(const Vector3& scale)
{
	return Matrix(
		scale.x,    0.0f,    0.0f, 0.0f,
		   0.0f, scale.y,    0.0f, 0.0f,
		   0.0f,    0.0f, scale.z, 0.0f,
		   0.0f,    0.0f,    0.0f, 1.0f);
}

//!	�g��k��
//!	@param	[in]	scale	�L�k�l
Matrix Matrix::Scale(f32 sx, f32 sy, f32 sz)
{
	return Matrix::Scale(Vector3(sx, sy, sz));
}

//!	�g��k��
//!	@param	[in]	scale	�L�k�l
Matrix Matrix::Scale(f32 scale)
{
	return Matrix::Scale(Vector3(scale, scale, scale));
}

//!	���W�ݒ�
//!	@param	[in]	position	���W
void Matrix::SetTranslate(const Vector3& pos)
{
	_m[3].Set(pos, 1.0f);
}

//!	��]�ݒ�
//!	@param	[in]	rotate		��]
void Matrix::SetRotate(const Angle& rotate)
{
	Vector3 scale = GetScale();
	Vector3 position = GetPosition();
	Matrix matrix = Matrix::Rotate(rotate);
	matrix *= Matrix::Scale(scale);
	matrix.SetTranslate(position);
	*this = matrix;
}

//!	��]�ݒ�
//!	@param	[in]	rotate		��]
void Matrix::SetRotate(const Vector3& rotate)
{
	Vector3 scale = GetScale();
	Vector3 position = GetPosition();
	Matrix matrix = Matrix::Rotate(rotate);
	matrix *= Matrix::Scale(scale);
	matrix.SetTranslate(position);
	*this = matrix;
}
//!	��]�ݒ�
//!	@param	[in]	rotate		��]
void Matrix::SetRotate(const Quaternion& rotate)
{
	Vector3 scale = GetScale();
	Vector3 position = GetPosition();
	Matrix matrix = Matrix::Rotate(rotate);
	matrix *= Matrix::Scale(scale);
	matrix.SetTranslate(position);
	*this = matrix;
}

//!	�X�P�[���ݒ�
//!	@param	[in]	scale		�X�P�[��
void Matrix::SetScale(const Vector3& scale)
{
	Quaternion rotate = GetRotateQuaternion();
	Vector3 position = GetPosition();
	Matrix matrix = Matrix::Scale(scale);
	matrix *= Matrix::Rotate(rotate);
	matrix.SetTranslate(position);
	*this = matrix;
}


Vector3 Matrix::PostMult(const Vector3& v) const
{
	float d = 1.0f / (_m[3][0] * v.x + _m[3][1] * v.y + _m[3][2] * v.z + _m[3][3]);
	d = 1.0f;
	return Vector3(
		(_m[0][0] * v.x + _m[0][1] * v.y + _m[0][2] * v.z + _m[0][3]) * d,
		(_m[1][0] * v.x + _m[1][1] * v.y + _m[1][2] * v.z + _m[1][3]) * d,
		(_m[2][0] * v.x + _m[2][1] * v.y + _m[2][2] * v.z + _m[2][3]) * d
	);
}

//Vector3 Matrix::PreMult(const Vector3& v) const
//{
//	float d = 1.0f / (m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3]);
//	d = 1.0f;
//	return Vector3(
//		(m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0]) * d,
//		(m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1]) * d,
//		(m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2]) * d);
//}

Vector4 Matrix::PostMult(const Vector4& v) const
{
	return Vector4(
		(_m[0][0] * v.x + _m[0][1] * v.y + _m[0][2] * v.z + _m[0][3] * v.w),
		(_m[1][0] * v.x + _m[1][1] * v.y + _m[1][2] * v.z + _m[1][3] * v.w),
		(_m[2][0] * v.x + _m[2][1] * v.y + _m[2][2] * v.z + _m[2][3] * v.w),
		(_m[3][0] * v.x + _m[3][1] * v.y + _m[3][2] * v.z + _m[3][3] * v.w));
}

//Vector4 Matrix::PreMult(const Vector4& v) const
//{
//	return Vector4(
//		(m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w),
//		(m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w),
//		(m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w),
//		(m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w));
//}



//!	��]���̃}�g���b�N�X
Matrix Matrix::RotateMatrix(const Matrix& matrix)
{
	Matrix result = matrix;
	result[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
	return result;
}

//!	�t�s��
Matrix Matrix::Invert(const Matrix& matrix)
{
	f32* m = (f32*)&matrix;
	f32 inv[16];

	inv[0]  =  m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
	inv[4]  = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
	inv[8]  =  m[4] * m[9]  * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
	inv[12] = -m[4] * m[9]  * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
	inv[1]  = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
	inv[5]  =  m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
	inv[9]  = -m[0] * m[9]  * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
	inv[13] =  m[0] * m[9]  * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
	inv[2]  =  m[1] * m[6]  * m[15] - m[1] * m[7]  * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7]  - m[13] * m[3] * m[6];
	inv[6]  = -m[0] * m[6]  * m[15] + m[0] * m[7]  * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7]  + m[12] * m[3] * m[6];
	inv[10] =  m[0] * m[5]  * m[15] - m[0] * m[7]  * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7]  - m[12] * m[3] * m[5];
	inv[14] = -m[0] * m[5]  * m[14] + m[0] * m[6]  * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6]  + m[12] * m[2] * m[5];
	inv[3]  = -m[1] * m[6]  * m[11] + m[1] * m[7]  * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9]  * m[2] * m[7]  + m[9]  * m[3] * m[6];
	inv[7]  =  m[0] * m[6]  * m[11] - m[0] * m[7]  * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8]  * m[2] * m[7]  - m[8]  * m[3] * m[6];
	inv[11] = -m[0] * m[5]  * m[11] + m[0] * m[7]  * m[9]  + m[4] * m[1] * m[11] - m[4] * m[3] * m[9]  - m[8]  * m[1] * m[7]  + m[8]  * m[3] * m[5];
	inv[15] =  m[0] * m[5]  * m[10] - m[0] * m[6]  * m[9]  - m[4] * m[1] * m[10] + m[4] * m[2] * m[9]  + m[8]  * m[1] * m[6]  - m[8]  * m[2] * m[5];

	f32 det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0)
		return Matrix::ZERO;

	det = 1.f / det;

	Matrix result;

	result[0].Set(inv[0]  * det, inv[1]  * det, inv[2]  * det, inv[3]  * det);
	result[1].Set(inv[4]  * det, inv[5]  * det, inv[6]  * det, inv[7]  * det);
	result[2].Set(inv[8]  * det, inv[9]  * det, inv[10] * det, inv[11] * det);
	result[3].Set(inv[12] * det, inv[13] * det, inv[14] * det, inv[15] * det);

	return result;

}

//!	�t�s��
Matrix Matrix::GetInvert()const
{
	return Matrix::Invert(*this);
}


//!	��Z
//!	@param	[in]	a	�}�g���b�N�X
//!	@param	[in]	b	�}�g���b�N�X
//!	@return	�}�g���b�N�X���|�����킹������
Matrix Matrix::Mult(const Matrix& a, const Matrix& b)
{
	Matrix c;
	c[0].x = (a[0].x * b[0].x) + (a[0].y * b[1].x) + (a[0].z * b[2].x) + (a[0].w * b[3].x);
	c[0].y = (a[0].x * b[0].y) + (a[0].y * b[1].y) + (a[0].z * b[2].y) + (a[0].w * b[3].y);
	c[0].z = (a[0].x * b[0].z) + (a[0].y * b[1].z) + (a[0].z * b[2].z) + (a[0].w * b[3].z);
	c[0].w = (a[0].x * b[0].w) + (a[0].y * b[1].w) + (a[0].z * b[2].w) + (a[0].w * b[3].w);

	c[1].x = (a[1].x * b[0].x) + (a[1].y * b[1].x) + (a[1].z * b[2].x) + (a[1].w * b[3].x);
	c[1].y = (a[1].x * b[0].y) + (a[1].y * b[1].y) + (a[1].z * b[2].y) + (a[1].w * b[3].y);
	c[1].z = (a[1].x * b[0].z) + (a[1].y * b[1].z) + (a[1].z * b[2].z) + (a[1].w * b[3].z);
	c[1].w = (a[1].x * b[0].w) + (a[1].y * b[1].w) + (a[1].z * b[2].w) + (a[1].w * b[3].w);

	c[2].x = (a[2].x * b[0].x) + (a[2].y * b[1].x) + (a[2].z * b[2].x) + (a[2].w * b[3].x);
	c[2].y = (a[2].x * b[0].y) + (a[2].y * b[1].y) + (a[2].z * b[2].y) + (a[2].w * b[3].y);
	c[2].z = (a[2].x * b[0].z) + (a[2].y * b[1].z) + (a[2].z * b[2].z) + (a[2].w * b[3].z);
	c[2].w = (a[2].x * b[0].w) + (a[2].y * b[1].w) + (a[2].z * b[2].w) + (a[2].w * b[3].w);

	c[3].x = (a[3].x * b[0].x) + (a[3].y * b[1].x) + (a[3].z * b[2].x) + (a[3].w * b[3].x);
	c[3].y = (a[3].x * b[0].y) + (a[3].y * b[1].y) + (a[3].z * b[2].y) + (a[3].w * b[3].y);
	c[3].z = (a[3].x * b[0].z) + (a[3].y * b[1].z) + (a[3].z * b[2].z) + (a[3].w * b[3].z);
	c[3].w = (a[3].x * b[0].w) + (a[3].y * b[1].w) + (a[3].z * b[2].w) + (a[3].w * b[3].w);

	return c;
}

//!	���s���e
//!	@param	[in]	left	��
//!	@param	[in]	right	�E
//!	@param	[in]	bottom	��
//!	@param	[in]	top		��
Matrix Matrix::Ortho2DMatrix(f32 left, f32 right, f32 bottom, f32 top)
{
	return OrthoMatrix(left, right, bottom, top, -1.0f, 1.0f);
}


//!	��Z(�I�y���[�^�[)
Matrix  Matrix::operator*(const Matrix& mat) const
{
	return Mult(*this, mat);
}
//!	��Z(�I�y���[�^�[)
Matrix& Matrix::operator*=(const Matrix& mat)
{
	*this = Mult(*this, mat);
	return *this;
}

bool Matrix::operator==(const Matrix& mat) const
{
	return (_m[0] == mat._m[0] && _m[1] == mat._m[1] && _m[2] == mat._m[2] && _m[3] == mat._m[3]);
}

bool Matrix::operator!=(const Matrix& mat) const
{
	return !(_m[0] == mat._m[0] && _m[1] == mat._m[1] && _m[2] == mat._m[2] && _m[3] == mat._m[3]);
}

Vector4& Matrix::operator[](s32 index)
{
	return _m[index];
}

Vector4 Matrix::operator[](s32 index)const
{
	return _m[index];
}

Matrix::operator f32* ()
{
	return (f32*)this;
}

Matrix::operator const f32* ()const
{
	return (f32*)this;
}

std::ostream& operator<<(std::ostream& os, const Matrix& m)
{
	s32 w = 10;
	os	<< std::setw(w)
		<< m[0].x << "," << std::setw(w)
		<< m[0].y << "," << std::setw(w)
		<< m[0].z << "," << std::setw(w)
		<< m[0].w << std::endl;

	os	<< std::setw(w)
		<< m[1].x << "," << std::setw(w)
		<< m[1].y << "," << std::setw(w)
		<< m[1].z << "," << std::setw(w)
		<< m[1].w << std::endl;

	os	<< std::setw(w)
		<< m[2].x << "," << std::setw(w)
		<< m[2].y << "," << std::setw(w)
		<< m[2].z << "," << std::setw(w)
		<< m[2].w << std::endl;

	os	<< std::setw(w)
		<< m[3].x << "," << std::setw(w)
		<< m[3].y << "," << std::setw(w)
		<< m[3].z << "," << std::setw(w)
		<< m[3].w;

	return os;
}
std::istream& operator>>(std::istream& is, Matrix& m)
{
	is >> m[0].x; is.ignore(2);
	is >> m[0].y; is.ignore(2);
	is >> m[0].z; is.ignore(2);
	is >> m[0].w; is.ignore(1);

	is >> m[1].x; is.ignore(2);
	is >> m[1].y; is.ignore(2);
	is >> m[1].z; is.ignore(2);
	is >> m[1].w; is.ignore(1);

	is >> m[2].x; is.ignore(2);
	is >> m[2].y; is.ignore(2);
	is >> m[2].z; is.ignore(2);
	is >> m[2].w; is.ignore(1);

	is >> m[3].x; is.ignore(2);
	is >> m[3].y; is.ignore(2);
	is >> m[3].z; is.ignore(2);
	is >> m[3].w;

	return is;
}


//	bulet
Matrix& Matrix::operator=(const btTransform& trans)
{
	trans.getOpenGLMatrix((f32*)this);
	return *this;
}


struct Transform
{
	Transform()
		: _position(Vector3::ZERO)
		, _rotate(Vector3::ZERO)
		, _scale(Vector3::ZERO)
		, _matrix(Matrix::IDENTITY)
	{
	}
	Transform(const Vector3& position, const Vector3& rotate, const Vector3& scale)
		: _position(position)
		, _rotate(rotate)
		, _scale(scale)
		, _matrix(Matrix::Affine(position, rotate, scale))
	{
	}
	virtual ~Transform(){};

	void Set(const Vector3& position, const Vector3& rotate, const Vector3& scale)
	{
		_position = position;
		_rotate = rotate;
		_scale = scale;

		UpdateMatrix();
	}
	void SetPosition(const Vector3& position)
	{
		_position = position;
		UpdateMatrix();
	}
	void SetRotate(const Vector3& rotate)
	{
		_rotate = rotate;
		UpdateMatrix();
	}
	void SetScale(const Vector3& scale)
	{
		_scale = scale;
		UpdateMatrix();
	}
	void UpdateMatrix()
	{
		_matrix = Matrix::Affine(_position, _rotate, _scale);
	}

	Vector3 _position;
	Vector3 _rotate;
	Vector3 _scale;

	Matrix _matrix;
};





MY_FORCEINLINE
void MultMatrix(const f32* matrix)
{
	glMultMatrixf(matrix);
}
MY_FORCEINLINE
void MultMatrix(const Matrix& matrix)
{
	glMultMatrixf((f32*)&matrix);
}
MY_FORCEINLINE
void LoadMatrix(const f32* matrix)
{
	glLoadMatrixf(matrix);
}
MY_FORCEINLINE
void LoadMatrix(const Matrix& matrix)
{
	glLoadMatrixf((f32*)&matrix);
}

