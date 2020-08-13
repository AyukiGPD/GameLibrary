//=============================================================================
//!
//!	@file	BoxMesh.h
//!	@brief	ボックス
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//=============================================================================
//! @brief  ボックス
//=============================================================================
class BoxMesh : public MeshBase
{
	//!	コンストラクタ
	BoxMesh();
	//!	コンストラクタ
	//!	@param	[in]	pParent	親オブジェクト
	BoxMesh(const MaterialData& material);
	//!	デストラクタ
	virtual ~BoxMesh();

	//!	描画
	void Render();
private:
	static RefPointer<BufferObject::Vao>	_pStaticBoxMesh;
};


