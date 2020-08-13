//=============================================================================
//!
//!	@file	Ibo.h
//!	@brief	Ibo
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//!	Vbo Ibo Vao
namespace BufferObject
{

//=============================================================================
//! @brief  IndexBufferObject
//=============================================================================
class Ibo
{
public:
	friend class Vao;	//	Vaoで扱うためにフレンドにする
	friend class Vbo;	//	Vboで扱うためにフレンドにする

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
		Desc(u32 stride, u32 count, const void* pData);

		//! @brief  インデックス設定
		//!	@param	[in]	stride	データ一つ分のサイズ
		//!	@param	[in]	count	配列サイズ
		//!	@param	[in]	pData	データポインタ
		void SetIndexData(u32 stride, u32 count, const void* pData);

		u32						_indexStride;	//!< データ一つのサイズ
		u32						_indexCount;	//!< データサイズ
		const void*				_pIndexData;	//!< データのポインタ
	};

public:
	//! @brief  コンストラクタ
	Ibo();

	//! @brief  デストラクタ
	virtual ~Ibo();

	//! @brief  データ作成
	//!	@param	[in]	desc			バッファデータ
	//!	@param	[in]	indexType		インデックスの型
	bool Create(const Desc& desc, INDEX_TYPE indexType);

	//! @brief  IboのID取得
	//!	@return	IBOのID
	u32 GetId(){ return _iboId; }

	//! @brief  バッファ有効化
	void Bind()const;

	//! @brief  バッファ無効化
	void UnBind()const;
private:
	u32		_iboId;			//!< IndexBufferObject ID
	u32		_indexType;		//!< インデックスタイプ
	u32		_indexCount;	//!< インデックスサイズ
};

};	//	namespace BufferObject
