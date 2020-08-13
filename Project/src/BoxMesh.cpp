//=============================================================================
//!
//!	@file	BoxMesh.cpp
//!	@brief	ボックス
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

using namespace BufferObject;


static const f32 gBoxSize = 1.0f;

//-----------------------------------------------------------------------------
//! @brief  頂点情報
//-----------------------------------------------------------------------------
static const Vertex gBoxVertices[] =
{
	// 前
	{ Vector3(-gBoxSize, +gBoxSize, +gBoxSize), Vector3(0, 0, +1), Color(255), Vector2(0, 1) },
	{ Vector3(+gBoxSize, +gBoxSize, +gBoxSize), Vector3(0, 0, +1), Color(255), Vector2(1, 1) },
	{ Vector3(-gBoxSize, -gBoxSize, +gBoxSize), Vector3(0, 0, +1), Color(255), Vector2(0, 0) },
	{ Vector3(+gBoxSize, -gBoxSize, +gBoxSize), Vector3(0, 0, +1), Color(255), Vector2(1, 0) },

	// 後
	{ Vector3(+gBoxSize, +gBoxSize, -gBoxSize), Vector3(0, 0, -1), Color(255), Vector2(1, 1) },
	{ Vector3(-gBoxSize, +gBoxSize, -gBoxSize), Vector3(0, 0, -1), Color(255), Vector2(0, 1) },
	{ Vector3(+gBoxSize, -gBoxSize, -gBoxSize), Vector3(0, 0, -1), Color(255), Vector2(1, 0) },
	{ Vector3(-gBoxSize, -gBoxSize, -gBoxSize), Vector3(0, 0, -1), Color(255), Vector2(0, 0) },

	// 左
	{ Vector3(-gBoxSize, +gBoxSize, -gBoxSize), Vector3(-1, 0, 0), Color(255), Vector2(1, 0) },
	{ Vector3(-gBoxSize, +gBoxSize, +gBoxSize), Vector3(-1, 0, 0), Color(255), Vector2(1, 1) },
	{ Vector3(-gBoxSize, -gBoxSize, -gBoxSize), Vector3(-1, 0, 0), Color(255), Vector2(0, 0) },
	{ Vector3(-gBoxSize, -gBoxSize, +gBoxSize), Vector3(-1, 0, 0), Color(255), Vector2(0, 1) },

	// 右
	{ Vector3(+gBoxSize, +gBoxSize, +gBoxSize), Vector3(+1, 0, 0), Color(255), Vector2(1, 1) },
	{ Vector3(+gBoxSize, +gBoxSize, -gBoxSize), Vector3(+1, 0, 0), Color(255), Vector2(1, 0) },
	{ Vector3(+gBoxSize, -gBoxSize, +gBoxSize), Vector3(+1, 0, 0), Color(255), Vector2(0, 1) },
	{ Vector3(+gBoxSize, -gBoxSize, -gBoxSize), Vector3(+1, 0, 0), Color(255), Vector2(0, 0) },

	// 上
	{ Vector3(-gBoxSize, +gBoxSize, -gBoxSize), Vector3(0, +1, 0), Color(255), Vector2(0, 0) },
	{ Vector3(+gBoxSize, +gBoxSize, -gBoxSize), Vector3(0, +1, 0), Color(255), Vector2(1, 0) },
	{ Vector3(-gBoxSize, +gBoxSize, +gBoxSize), Vector3(0, +1, 0), Color(255), Vector2(0, 1) },
	{ Vector3(+gBoxSize, +gBoxSize, +gBoxSize), Vector3(0, +1, 0), Color(255), Vector2(1, 1) },

	// 下
	{ Vector3(-gBoxSize, -gBoxSize, +gBoxSize), Vector3(0, -1, 0), Color(255), Vector2(0, 1) },
	{ Vector3(+gBoxSize, -gBoxSize, +gBoxSize), Vector3(0, -1, 0), Color(255), Vector2(1, 1) },
	{ Vector3(-gBoxSize, -gBoxSize, -gBoxSize), Vector3(0, -1, 0), Color(255), Vector2(0, 0) },
	{ Vector3(+gBoxSize, -gBoxSize, -gBoxSize), Vector3(0, -1, 0), Color(255), Vector2(1, 0) },
};

//-----------------------------------------------------------------------------
//! @brief  頂点インデックス
//-----------------------------------------------------------------------------
static const u32 gBoxIndices[] =
{
	0,  1,  2,  1,  3,  2,
	4,  5,  6,  5,  7,  6,
	8,  9,  10, 9,  11, 10,
	12, 13, 14, 13, 15, 14,
	16, 17, 18, 17, 19, 18,
	20, 21, 22, 21, 23, 22, 
};

//-----------------------------------------------------------------------------
//! @brief  バッファ情報
//-----------------------------------------------------------------------------
BufferObject::Element gBoxElements[] =
{
	{ 0, 3, BufferObject::ELEMENT_TYPE::F32, false, offsetof(Vertex, _position) },
	{ 1, 3, BufferObject::ELEMENT_TYPE::F32, false, offsetof(Vertex, _normal) },
	{ 2, 4, BufferObject::ELEMENT_TYPE::U8,  true , offsetof(Vertex, _color) },
	{ 3, 2, BufferObject::ELEMENT_TYPE::F32, false, offsetof(Vertex, _uv) },
};

RefPointer<BufferObject::Vao> BoxMesh::_pStaticBoxMesh = nullptr;

//-----------------------------------------------------------------------------
//  @brief  コンストラクタ
//-----------------------------------------------------------------------------
BoxMesh::BoxMesh()
{
	if (!_pStaticBoxMesh)
	{
		const int vCount = ARRAY_COUNT(gBoxVertices);
		const int iCount = ARRAY_COUNT(gBoxIndices);
		const int eCount = ARRAY_COUNT(gBoxElements);

		BufferObject::Vao::Desc desc;
		desc.SetVertexData(sizeof(Vertex), vCount, &gBoxVertices[0]);
		desc.SetIndexData(sizeof(u32), iCount, &gBoxIndices[0]);
		desc.SetElementList(gBoxElements, eCount);

		_pStaticBoxMesh = CreateRefPointerInit<BufferObject::Vao>(
				desc,
				BufferObject::PRIMITIVE_TYPE::TRIANGLES,
				BufferObject::INDEX_TYPE::TYPE_32
			);
	}
}

//-----------------------------------------------------------------------------
//  @brief  デストラクタ
//-----------------------------------------------------------------------------
BoxMesh::~BoxMesh()
{
}

//-----------------------------------------------------------------------------
//  @brief  コンストラクタ
//  @param	[in]	pParent	親オブジェクト
//-----------------------------------------------------------------------------
BoxMesh::BoxMesh(const MaterialData& material)
{
	if (!_pStaticBoxMesh)
	{
		const int vCount = ARRAY_COUNT(gBoxVertices);
		const int iCount = ARRAY_COUNT(gBoxIndices);
		const int eCount = ARRAY_COUNT(gBoxElements);

		BufferObject::Vao::Desc desc;
		desc.SetVertexData(sizeof(Vertex), vCount, &gBoxVertices[0]);
		desc.SetIndexData(sizeof(u32), iCount, &gBoxIndices[0]);
		desc.SetElementList(gBoxElements, eCount);

		_pStaticBoxMesh = CreateRefPointerInit<BufferObject::Vao>(
			desc,
			BufferObject::PRIMITIVE_TYPE::TRIANGLES,
			BufferObject::INDEX_TYPE::TYPE_32
			);
	}

	SetMaterial(0, material);
}

//-----------------------------------------------------------------------------
//  @brief  描画
//-----------------------------------------------------------------------------
void BoxMesh::Render()
{
	if (!_pStaticBoxMesh){ assert(false); }

	BindMaterial(0);
	_pStaticBoxMesh->Render();

}


