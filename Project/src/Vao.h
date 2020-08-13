//=============================================================================
//!
//!	@file	Vao.h
//!	@brief	Vao
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//!	Vbo Ibo Vao
namespace BufferObject
{

//=============================================================================
//! @brief  VertexArrayObject
//=============================================================================
class Vao : public MeshBase
{
	friend class VaoObject;
public:
	//------------------------------------------------------------------
	//! @brief  バッファデータ
	//------------------------------------------------------------------
	struct Desc
	{
		//! @brief  コンストラクタ
		Desc();

		//! @brief  頂点設定
		//!	@param	[in]	stride	データ一つ分のサイズ
		//!	@param	[in]	count	配列サイズ
		//!	@param	[in]	pData	データポインタ
		void SetVertexData(u32 stride, u32 count, const void* pData);

		//! @brief  インデックス設定
		//!	@param	[in]	stride	データ一つ分のサイズ
		//!	@param	[in]	count	配列サイズ
		//!	@param	[in]	pData	データポインタ
		void SetIndexData(u32 stride, u32 count, const void* pData);

		//! @brief  バッファ情報追加
		//!	@param	[in]	pList			リストデータ
		//!	@param	[in]	elementCount	配列サイズ
		void SetElementList(const Element* pList, u32 elementCount);

		//! @brief  バッファ情報追加
		//!	@param	[in]	list	バッファ情報リスト
		void SetElementList(const std::vector<Element>& list);

		//! @brief  Vboバッファデータ
		operator Vbo::Desc()const;

		//! @brief  Iboバッファデータ
		operator Ibo::Desc()const;

		u32						_vertexStride;	//!< データ一つのサイズ
		u32						_vertexCount;	//!< データサイズ
		const void*				_pVertexData;	//!< データのポインタ

		u32						_indexStride;	//!< データ一つのサイズ
		u32						_indexCount;	//!< データサイズ
		const void*				_pIndexData;	//!< データのポインタ

		std::vector<Element>	_elementList;	//!< バッファ情報リスト
	};

public:
	//! @brief  コンストラクタ
	Vao();

	//! @brief  コンストラクタ
	//!	@param	[in]	desc			バッファデータ
	//!	@param	[in]	primitiveType	描画方法
	//!	@param	[in]	indexType		インデックスの型
	Vao(const Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType);

	//! @brief  デストラクタ
	virtual ~Vao();

	//! @brief  データ作成
	//!	@param	[in]	desc			バッファデータ
	//!	@param	[in]	primitiveType	描画方法
	//!	@param	[in]	indexType		インデックスの型
	bool Init(const Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType);

	//! @brief  データ作成
	//!	@param	[in]	desc			バッファデータ
	//!	@param	[in]	primitiveType	描画方法
	//!	@param	[in]	indexType		インデックスの型
	static Vao* Create(const Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType);

	// @brief  描画
	void Render();

private:
	Vbo*	_pVbo;			//!< Vertex Buffer Object
	Ibo*	_pIbo;			//!< Index  Buffer Object
	u32		_vao;			//!< Vertex Array  Object ID
};

};	//	namespace BufferObject
