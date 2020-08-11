//=============================================================================
//!
//!	@file	drawObj.h
//!	@brief	図形描画関数群
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once


//!	法線作成
//!	@param	[in]	A	頂点A
//!	@param	[in]	B	頂点B
//!	@param	[in]	C	頂点C
//!	@return	法線ベクトル
Vector3 CreateTriangleNormal(const Vector3& A, const Vector3& B, const Vector3& C);


//!	2D描画関数名前空間
namespace Render2D
{

//!	四角描画
//!	@param	[in]	x		座標X
//!	@param	[in]	y		座標Y
//!	@param	[in]	w		サイズW
//!	@param	[in]	h		サイズH
//!	@param	[in]	color	色
void Rect(f32 x, f32 y, f32 w, f32 h,	const Color& color);

//!	四角描画
//!	@param	[in]	position	座標
//!	@param	[in]	size		サイズ
//!	@param	[in]	color		色
void Rect(const Vector2& position, const Vector2& size, const Color& color);

//!	テクスチャ描画
//!	@param	[in]	pTexture	テクスチャのポインタ
void RenderBuffer(Texture* pTexture = nullptr);
};

//!	3D描画関数名前空間
namespace Render3D
{

//!	点描画
//!	@param	[in]	position	座標
//!	@param	[in]	color		色		default:白
//!	@param	[in]	size		サイズ	default:1.0f
void Point(const Vector3& position, const Color& color = 255, f32 size = 1.0f);

//!	線描画
//!	@param	[in]	from	始点
//!	@param	[in]	to		終点
//!	@param	[in]	color	色		default:白
void Line(const Vector3& from, const Vector3& to, const Color& color = 255);

//!	線描画
//!	@param	[in]	from		始点
//!	@param	[in]	to			終点
//!	@param	[in]	fromColor	始点色
//!	@param	[in]	toColor		終点色
void Line(const Vector3& from, const Vector3& to, const Color& fromColor, const Color& toColor);

//!	四角描画
//!	@param	[in]	position	座標
//!	@param	[in]	size		サイズ
//!	@param	[in]	color		色		default:白
void Rect(const Vector3& position, const Vector3& size, const Color& color = 255);

//!	四角描画
//!	@param	[in]	p1		頂点データ
//!	@param	[in]	p2		頂点データ
//!	@param	[in]	p3		頂点データ
//!	@param	[in]	p4		頂点データ
//!	@param	[in]	color	色		default:白
void Rect(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4, const Color& color = 255);

//!	四角描画
//!	@param	[in]	p1	頂点データ
//!	@param	[in]	p2	頂点データ
//!	@param	[in]	p3	頂点データ
//!	@param	[in]	p4	頂点データ
void Rect(const Vertex& p1, const Vertex& p2, const Vertex& p3, const Vertex& p4);

//!	四角描画
//!	@param	[in]	vertices	頂点データ配列
void Rect(const std::array<Vertex,4>& vertices);

};
