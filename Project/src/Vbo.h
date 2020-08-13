//=============================================================================
//!
//!	@file	Vbo.h
//!	@brief	Vbo
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//!	Vbo Ibo Vao
namespace BufferObject
{

class Ibo;

//=============================================================================
//! @brief  VertexBufferObject
//=============================================================================
class Vbo
{
public:
	friend class Vao;	//	Vaoで扱うためにフレンドにする

	//------------------------------------------------------------------
	//! @brief  バッファデータ
	//------------------------------------------------------------------
	struct Desc
	{
		//! @brief  コンストラクタ
		Desc();

		//! @brief  コンストラクタ
		//!	@param	[in]	stride	データ一つ分のサイズ
		//!	@param	[in]	count	配列サイズ
		//!	@param	[in]	pData	データポインタ
		//!	@param	[in]	list	バッファ情報リスト
		Desc(u32 vertexStride,
			 u32 vertexCount,
			 const void* pVertexData,
			 const std::vector<Element>& list);

		//! @brief  頂点設定
		//!	@param	[in]	stride	データ一つ分のサイズ
		//!	@param	[in]	count	配列サイズ
		//!	@param	[in]	pData	データポインタ
		void SetVertexData(u32 stride, u32 count, const void* pData);

		//! @brief  バッファ情報追加
		//!	@param	[in]	pList			リストデータ
		//!	@param	[in]	elementCount	配列サイズ
		void SetElementList(const Element* pList, u32 elementCount);

		//! @brief  バッファ情報追加
		//!	@param	[in]	list	バッファ情報リスト
		void SetElementList(const std::vector<Element>& list);

		u32						_vertexStride;	//!< データ一つのサイズ
		u32						_vertexCount;	//!< データサイズ
		const void*				_pVertexData;	//!< データのポインタ

		std::vector<Element>	_elementList;	//!< バッファ情報リスト
	};

public:
	//! @brief  コンストラクタ
	Vbo();
	//! @brief  デストラクタ
	virtual ~Vbo();

	//! @brief  データ作成
	//!	@param	[in]	desc			バッファデータ
	//!	@param	[in]	primitiveType	描画方法
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool Create(const Desc& desc, PRIMITIVE_TYPE primitiveType);

	//! @brief  描画方法設定
	//!	@param	[in]	type	タイプ
	//!	@retval	true	成功
	//!	@retval	false	失敗 _primitiveTypeは0になる
	bool SetPrimitiveType(PRIMITIVE_TYPE type);

	//! @brief  バッファ有効化
	void Bind();

	//! @brief  バッファ無効化
	void UnBind();

	//! @brief  内部で入力レイアウトを呼び出す
	void CallInputLayout();

	//! @brief  id取得
	//!	@return VboのID
	u32 GetId(){ return _vboId; }

	//! @brief  描画
	//!	@param	[in]	ibo	IndexBufferObject
	void Render(const Ibo& ibo);

	//! @brief  一部描画
	//!	@param	[in]	ibo		IndexBufferObject
	//!	@param	[in]	start	開始頂点
	//!	@param	[in]	count	描画する頂点数
	void Render(const Ibo& ibo, u32 start, u32 count);

	//! @brief  描画
	//!	@param	[in]	ibo		IndexBufferObject
	//!	@param	[in]	start	開始頂点
	//!	@param	[in]	count	描画する頂点数
	//!	@param	[in]	size	描画する数
	void InstancedRender(const Ibo& ibo, u32 start, u32 count, u32 size);

private:
	u32						_vboId;			//!< VertexBufferObject ID
	u32						_vertexStride;	//!< データ一つのサイズ
	u32						_primitiveType;	//!< 描画タイプ
	std::vector<Element>	_elementList;	//!< バッファ情報
};

};	//	namespace BufferObject
