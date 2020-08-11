//=============================================================================
//!
//!	@file	Color.h
//!	@brief	カラー構造体
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once


//red = (1 - Math.min(1, cyan * (1 - black) + black)) * 255
//green = (1 - Math.min(1, magenta * (1 - black) + black)) * 255
//blue = (1 - Math.min(1, yellow * (1 - black) + black)) * 255

struct RGBAColor
{
	union {
		struct {
			u8 r : 8;		//!< 赤
			u8 g : 8;		//!< 緑
			u8 b : 8;		//!< 青
			u8 a : 8;		//!< アルファ
		};
		u32 color;
	};
};
struct CMYKAColor
{
	u8 c;		//!< シアン
	u8 m;		//!< マゼンダ
	u8 y;		//!< 黄
	u8 k;		//!< 黒
	u8 a;		//!< アルファ
};

struct Color
{
	Color() {}
	//!	@param	[in]	red		赤
	//!	@param	[in]	green	緑
	//!	@param	[in]	blue	青
	//!	@param	[in]	alpha	アルファ
	Color(u32 red, u32 green, u32 blue, u32 alpha = 255)
	: r(red & 0xff)
	, g(green & 0xff)
	, b(blue & 0xff)
	, a(alpha & 0xff)
	{}
	//!	@param	[in]	_color	グレー値
	//!	@param	[in]	alpha	アルファ値
	Color(u32 color,u32 alpha = 255)
	: r(color & 0xff)
	, g(color & 0xff)
	, b(color & 0xff)
	, a(alpha & 0xff)
	{}

	//!	設定
	//!	@param	[in]	red	赤
	//!	@param	[in]	green	緑
	//!	@param	[in]	blue	青
	//!	@param	[in]	alpha	アルファ
	void Set(u32 red, u32 green, u32 blue, u32 alpha = 255)
	{
		this->r = (red&0xff);
		this->g = (green&0xff);
		this->b = (blue&0xff);
		this->a = (alpha &0xff);
	}

	//!	少数値での設定
	//!	@param	[in]	red		赤
	//!	@param	[in]	green	緑
	//!	@param	[in]	blue	青
	//!	@param	[in]	alpha	アルファ
	void SetFloat(f32 red, f32 green, f32 blue, f32 alpha = 1.0f)
	{
		this->r = (red * 255);
		this->g = (green * 255);
		this->b = (blue * 255);
		this->a = (alpha * 255);
	}

	//!	グレー設定
	//!	@param	[in]	_color	グレー値
	//!	@param	[in]	_alpha	アルファ値
	void Set(u32 _color,u32 _alpha = 255)
	{
		r = (_color&0xff);
		g = (_color&0xff);
		b = (_color&0xff);
		a = (_alpha&0xff);
	}

	//!	設定
	//!	@param	[in]	_color	色
	void Set(const Color& _color)
	{
		r = (_color.r&0xff);
		g = (_color.g&0xff);
		b = (_color.b&0xff);
		a = (_color.a&0xff);
	}

	//!	少数値での設定
	//!	@param	[in]	_color	色(Vector4)
	void Set(const Vector4& _color)
	{
		r = (u8)(_color.x * 255.0f) & 0xff;
		g = (u8)(_color.y * 255.0f) & 0xff;
		b = (u8)(_color.z * 255.0f) & 0xff;
		a = (u8)(_color.w * 255.0f) & 0xff;
	}

	//!	カラーをfloat値に変換
	Vector4 GetColorF()const
	{
		static const f32 inv = 1.0f / 255.0f;
		return Vector4( (f32)r * inv, (f32)g * inv, (f32)b * inv, (f32)a * inv );
	}
	//!	カラーをfloat値に変換
	Vector3 GetColorFRGB()const
	{
		static const f32 inv = 1.0f / 255.0f;
		return Vector3( (f32)r * inv, (f32)g * inv, (f32)b * inv );
	}


	//!	カラーをCMYKに変換
	CMYKAColor GatCMYKColor()const
	{
		CMYKAColor result;
		static const f32 inv = 1.0f / 255.0f;
		f32 red   = (f32)r * inv;
		f32 green = (f32)g * inv;
		f32 blue  = (f32)b * inv;

		result.k = min(1 - red, min(1 - green, 1 - blue));

		result.c = (1.f - red   - result.k) / (1.f - result.k);
		result.m = (1.f - green - result.k) / (1.f - result.k);
		result.y = (1.f - blue  - result.k) / (1.f - result.k);

		result.a = (f32)a * inv;
	}

	//!	反対色取得
	//!	@return	カラーの反対色
	Color RevColor()	const
	{
		Color result;
		u8 maxVal = r;
		if (maxVal < g){ maxVal = g; }
		if (maxVal < b){ maxVal = b; }
		u8 minVal = r;
		if (minVal > g){ minVal = g; }
		if (minVal > b){ minVal = b; }
		u32 minmax = maxVal + minVal;
		result.Set(minmax - r, minmax - g, minmax - b, a);
		return result;
	}
	//	オペレーター
	//!	本体の加算
	Color operator += (const Color& c)
	{
		r = (r + c.r) & 0xff;
		g = (g + c.g) & 0xff;
		b = (b + c.b) & 0xff;
		a = (a + c.a) & 0xff;

		return *this;
	}
	//!	本体の減算
	Color operator -= (const Color& c)
	{
		r = (r - c.r) & 0xff;
		g = (g - c.g) & 0xff;
		b = (b - c.b) & 0xff;
		a = (a - c.a) & 0xff;

		return *this;
	}
	//!	本体の乗算
	Color operator *= (const Color& c)
	{
		r = (u32)((f32)r * (f32)c.r) & 0xff;
		g = (u32)((f32)g * (f32)c.g) & 0xff;
		b = (u32)((f32)b * (f32)c.b) & 0xff;
		a = (u32)((f32)a * (f32)c.a) & 0xff;

		return *this;
	}
	//!	本体の除算
	Color operator /= (const Color& c)
	{
		r = (u32)((f32)r / (f32)c.r) & 0xff;
		g = (u32)((f32)g / (f32)c.g) & 0xff;
		b = (u32)((f32)b / (f32)c.b) & 0xff;
		a = (u32)((f32)a / (f32)c.a) & 0xff;

		return *this;
	}

	//!	本体の加算
	Color operator += (s32 c)
	{
		r = (r + c) & 0xff;
		g = (g + c) & 0xff;
		b = (b + c) & 0xff;
		a = (a + c) & 0xff;

		return *this;
	}
	//!	本体の減算
	Color operator -= (s32 c)
	{
		r = (r - c) & 0xff;
		g = (g - c) & 0xff;
		b = (b - c) & 0xff;
		a = (a - c) & 0xff;

		return *this;
	}
	//!	本体の乗算
	Color operator *= (f32 c)
	{
		r = (u32)((f32)r * c) & 0xff;
		g = (u32)((f32)g * c) & 0xff;
		b = (u32)((f32)b * c) & 0xff;
		a = (u32)((f32)a * c) & 0xff;

		return *this;
	}
	//!	本体の除算
	Color operator /= (f32 c)
	{
		r = (u32)((f32)r / c) & 0xff;
		g = (u32)((f32)g / c) & 0xff;
		b = (u32)((f32)b / c) & 0xff;
		a = (u32)((f32)a / c) & 0xff;

		return *this;
	}

	static const f32	TO_FLOAT;

	static const Color WHITE;		//!< 白
	static const Color GRAY;		//!< 灰
	static const Color BLACK;		//!< 黒
	static const Color RED;			//!< 赤
	static const Color GREEN;		//!< 緑
	static const Color BLUE;		//!< 青
	static const Color YELLOW;		//!< 黄
	static const Color MAGENTA;		//!< マゼンダ
	static const Color CYAN;		//!< シアン
	static const Color ORANGE;		//!< 橙
	static const Color INDIGO;		//!< 藍
	static const Color PURPLE;		//!< 紫

	union {
		struct {
			u8 r : 8;		//!< 赤
			u8 g : 8;		//!< 緑
			u8 b : 8;		//!< 青
			u8 a : 8;		//!< アルファ
		};
		u32 color;
	};

};

namespace Math
{
Color RandomColor();
}

//	GL関数

MY_FORCEINLINE
void SetColor(const Color& color)
{
	glColor4ub((GLubyte)color.r, (GLubyte)color.g, (GLubyte)color.b, (GLubyte)color.a);
}
MY_FORCEINLINE
void SetColor(u32 r, u32 g, u32 b, u32 a = 255)
{
	glColor4ub((GLubyte)r, (GLubyte)g, (GLubyte)b, (GLubyte)a);
}
MY_FORCEINLINE
void SetColor(u32 color, u32 alpha = 255)
{
	glColor4ub((GLubyte)color, (GLubyte)color, (GLubyte)color, (GLubyte)alpha);
}

MY_FORCEINLINE
void SetColorF(f32 r, f32 g, f32 b, f32 a = 1.0f)
{
	glColor4f(r,g,b,a);
}
MY_FORCEINLINE
void SetColorF(f32 color, f32 alpha = 1.0f)
{
	glColor4f(color,color,color,alpha);
}

MY_FORCEINLINE
void SetClearColor(f32 r, f32 g, f32 b, f32 a = 1.0f)
{
	glClearColor(r,g,b,a);
}
MY_FORCEINLINE
void SetClearColor(f32 color, f32 alpha = 1.0f)
{
	glClearColor(color,color,color,alpha);
}
void SetClearColor(const Color& color)
{
	Vector4 colorF = color.GetColorF();
	glClearColor(colorF.x,colorF.y,colorF.z,colorF.w);
}


