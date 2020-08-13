//=============================================================================
//!
//!	@file	VaoObject.cpp
//!	@brief	Vaoオブジェクト
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

namespace BufferObject
{

//-----------------------------------------------------------------------------
//! @brief  Vaoオブジェクト
//-----------------------------------------------------------------------------
class VaoObject : public GameObjectBase
{
public:
	friend Vao;
	//! @brief  コンストラクタ
	VaoObject();

	//! @brief  コンストラクタ
	//!	@param	[in]	desc			バッファデータ
	//!	@param	[in]	primitiveType	描画方法
	//!	@param	[in]	indexType		インデックスの型
	VaoObject(const Vao::Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType, const MaterialData& material);

	//! @brief  デストラクタ
	virtual ~VaoObject();

	//! @brief  解放
	void Clear();

	//! @brief  データ作成
	//!	@param	[in]	desc			バッファデータ
	//!	@param	[in]	primitiveType	描画方法
	//!	@param	[in]	indexType		インデックスの型
	bool Init(const Vao::Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType, const MaterialData& materiial);

	//! @brief  データ作成
	//!	@param	[in]	desc			バッファデータ
	//!	@param	[in]	primitiveType	描画方法
	//!	@param	[in]	indexType		インデックスの型
	static VaoObject* Create(const Vao::Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType, const MaterialData& materiial);

	//! @brief  描画
	void Render();

	//! @brief  データ有無
	bool IsEmpty();

	//! @brief  Vao取得
	//!	@return	Vaoのポインタ
	Vao* GetVao();

private:
	Vao*	_pVao;
};

};	//	namespace BufferObject

