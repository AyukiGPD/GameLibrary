//=============================================================================
//!
//!	@file	BufferObject.h
//!	@brief	Vbo Ibo Vao
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//!	Vbo Ibo Vao
namespace BufferObject
{

//-----------------------------------------------------------------------------
//!	データ型
//-----------------------------------------------------------------------------
enum class ELEMENT_TYPE
{
	UNKNOWN	= 0,
	S8		= GL_BYTE,
	U8		= GL_UNSIGNED_BYTE,
	S16		= GL_SHORT,
	U16		= GL_UNSIGNED_SHORT,
	S32		= GL_INT,
	U32		= GL_UNSIGNED_INT,
	F32		= GL_FLOAT,
};

//-----------------------------------------------------------------------------
//!	描画方法
//-----------------------------------------------------------------------------
enum class PRIMITIVE_TYPE
{
	UNKNOWN,
	POINTS,
	LINE,
	LINE_LOOP,
	LINE_STRIP,
	TRIANGLES,
	TRIANGLE_FAN,
	TRIANGLE_STRIP,
	POLYGON,
};

//-----------------------------------------------------------------------------
//!	インデックスの型
//-----------------------------------------------------------------------------
enum class INDEX_TYPE
{
	TYPE_16,
	TYPE_32
};

//-----------------------------------------------------------------------------
//!	バッファ情報
//-----------------------------------------------------------------------------
struct Element
{
	u32				_locationIndex;
	u32				_count;
	ELEMENT_TYPE	_elementType;
	bool			_isNormalize;
	size_t			_strideOffset;
};

class Ibo;

//=============================================================================
//!	VertexBufferObject
//=============================================================================
class Vbo
{
public:
	friend class Vao;	//	Vaoで扱うためにフレンドにする

	//------------------------------------------------------------------
	//!	バッファデータ
	//------------------------------------------------------------------
	struct Desc
	{
		//!	コンストラクタ
		Desc();
		//!	コンストラクタ
		//!	@param	[in]	stride	データ一つ分のサイズ
		//!	@param	[in]	count	配列サイズ
		//!	@param	[in]	pData	データポインタ
		//!	@param	[in]	list	バッファ情報リスト
		Desc(u32 vertexStride,
			 u32 vertexCount,
			 const void* pVertexData,
			 const std::vector<Element>& list);
		//!	頂点設定
		//!	@param	[in]	stride	データ一つ分のサイズ
		//!	@param	[in]	count	配列サイズ
		//!	@param	[in]	pData	データポインタ
		void SetVertexData(u32 stride, u32 count, const void* pData);
		//!	バッファ情報追加
		//!	@param	[in]	pList			リストデータ
		//!	@param	[in]	elementCount	配列サイズ
		void SetElementList(const Element* pList, u32 elementCount);
		//!	バッファ情報追加
		//!	@param	[in]	list	バッファ情報リスト
		void SetElementList(const std::vector<Element>& list);

		u32						_vertexStride;	//!< データ一つのサイズ
		u32						_vertexCount;	//!< データサイズ
		const void*				_pVertexData;	//!< データのポインタ

		std::vector<Element>	_elementList;	//!< バッファ情報リスト
	};

public:
	//!	コンストラクタ
	Vbo();
	//!	デストラクタ
	virtual ~Vbo();

	//!	データ作成
	//!	@param	[in]	desc			バッファデータ
	//!	@param	[in]	primitiveType	描画方法
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool Create(const Desc& desc, PRIMITIVE_TYPE primitiveType);

	//!	バッファ有効化
	void Bind();
	//!	バッファ無効化
	void UnBind();

	//!	内部で入力レイアウトを呼び出す
	void CallInputLayout();

	//!	id取得
	//!	@return VboのID
	u32 GetId(){ return _vboId; }

	//!	描画方法設定
	//!	@param	[in]	type	タイプ
	//!	@retval	true	成功
	//!	@retval	false	失敗 _primitiveTypeは0になる
	bool SetPrimitiveType(PRIMITIVE_TYPE type);

	//! 描画
	//!	@param	[in]	ibo	IndexBufferObject
	void Render(const Ibo& ibo);
	//! 描画
	//!	@param	[in]	ibo		IndexBufferObject
	//!	@param	[in]	start	開始頂点
	//!	@param	[in]	count	描画する頂点数
	void Render(const Ibo& ibo, u32 start, u32 count);
	//! 描画
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

//=============================================================================
//!	IndexBufferObject
//=============================================================================
class Ibo
{
public:
	friend class Vao;	//	Vaoで扱うためにフレンドにする
	friend class Vbo;	//	Vboで扱うためにフレンドにする

	//------------------------------------------------------------------
	//!	バッファデータ
	//------------------------------------------------------------------
	struct Desc
	{
		//!	コンストラクタ
		Desc();
		//!	コンストラクタ
		//!	@param	[in]	stride	データ一つ分のサイズ
		//!	@param	[in]	count	配列サイズ
		//!	@param	[in]	pData	データポインタ
		Desc(u32 stride, u32 count, const void* pData);
		//!	インデックス設定
		//!	@param	[in]	stride	データ一つ分のサイズ
		//!	@param	[in]	count	配列サイズ
		//!	@param	[in]	pData	データポインタ
		void SetIndexData(u32 stride, u32 count, const void* pData);

		u32						_indexStride;	//!< データ一つのサイズ
		u32						_indexCount;	//!< データサイズ
		const void*				_pIndexData;	//!< データのポインタ
	};

public:
	//!	コンストラクタ
	Ibo();
	//!	デストラクタ
	virtual ~Ibo();

	//!	データ作成
	//!	@param	[in]	desc			バッファデータ
	//!	@param	[in]	indexType		インデックスの型
	bool Create(const Desc& desc, INDEX_TYPE indexType);

	//!	IboのID取得
	//!	@return	IBOのID
	u32 GetId(){ return _iboId; }

	//!	バッファ有効化
	void Bind()const;
	//!	バッファ無効化
	void UnBind()const;
private:
	u32		_iboId;			//!< IndexBufferObject ID
	u32		_indexType;		//!< インデックスタイプ
	u32		_indexCount;	//!< インデックスサイズ
};


//=============================================================================
//!	VertexArrayObject
//=============================================================================
class Vao : public MeshBase
{
	friend class VaoObject;
public:
	//------------------------------------------------------------------
	//!	バッファデータ
	//------------------------------------------------------------------
	struct Desc
	{
		//!	コンストラクタ
		Desc();
		//!	頂点設定
		//!	@param	[in]	stride	データ一つ分のサイズ
		//!	@param	[in]	count	配列サイズ
		//!	@param	[in]	pData	データポインタ
		void SetVertexData(u32 stride, u32 count, const void* pData);
		//!	インデックス設定
		//!	@param	[in]	stride	データ一つ分のサイズ
		//!	@param	[in]	count	配列サイズ
		//!	@param	[in]	pData	データポインタ
		void SetIndexData(u32 stride, u32 count, const void* pData);
		//!	バッファ情報追加
		//!	@param	[in]	pList			リストデータ
		//!	@param	[in]	elementCount	配列サイズ
		void SetElementList(const Element* pList, u32 elementCount);
		//!	バッファ情報追加
		//!	@param	[in]	list	バッファ情報リスト
		void SetElementList(const std::vector<Element>& list);

		//!	Vboバッファデータ
		operator Vbo::Desc()const
		{
			Vbo::Desc desc(_vertexStride, _vertexCount, _pVertexData, _elementList);
			return desc;
		}

		//!	Iboバッファデータ
		operator Ibo::Desc()const
		{
			Ibo::Desc desc(_indexStride, _indexCount, _pIndexData);
			return desc;
		}

		u32						_vertexStride;	//!< データ一つのサイズ
		u32						_vertexCount;	//!< データサイズ
		const void*				_pVertexData;	//!< データのポインタ

		u32						_indexStride;	//!< データ一つのサイズ
		u32						_indexCount;	//!< データサイズ
		const void*				_pIndexData;	//!< データのポインタ

		std::vector<Element>	_elementList;	//!< バッファ情報リスト
	};

public:
	//!	コンストラクタ
	Vao();
	//!	コンストラクタ
	//!	@param	[in]	desc			バッファデータ
	//!	@param	[in]	primitiveType	描画方法
	//!	@param	[in]	indexType		インデックスの型
	Vao(const Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType)
	{
		Init(desc, primitiveType, indexType);
	}
	//!	デストラクタ
	virtual ~Vao();

	//!	データ作成
	//!	@param	[in]	desc			バッファデータ
	//!	@param	[in]	primitiveType	描画方法
	//!	@param	[in]	indexType		インデックスの型
	bool Init(const Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType);

	//!	データ作成
	//!	@param	[in]	desc			バッファデータ
	//!	@param	[in]	primitiveType	描画方法
	//!	@param	[in]	indexType		インデックスの型
	static Vao* Create(const Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType);

	// 描画
	void Render();

private:
	Vbo*	_pVbo;			//!< Vertex Buffer Object
	Ibo*	_pIbo;			//!< Index  Buffer Object
	u32		_vao;			//!< Vertex Array  Object ID
};

//!	Vaoオブジェクト
class VaoObject : public GameObjectBase
{
public:
	friend Vao;
	//!	コンストラクタ
	VaoObject();
	//!	コンストラクタ
	//!	@param	[in]	desc			バッファデータ
	//!	@param	[in]	primitiveType	描画方法
	//!	@param	[in]	indexType		インデックスの型
	VaoObject(const Vao::Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType, const MaterialData& material);
	//!	デストラクタ
	virtual ~VaoObject();

	//!	解放
	void Clear();

	//!	データ作成
	//!	@param	[in]	desc			バッファデータ
	//!	@param	[in]	primitiveType	描画方法
	//!	@param	[in]	indexType		インデックスの型
	bool Init(const Vao::Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType, const MaterialData& materiial);

	//!	データ作成
	//!	@param	[in]	desc			バッファデータ
	//!	@param	[in]	primitiveType	描画方法
	//!	@param	[in]	indexType		インデックスの型
	static VaoObject* Create(const Vao::Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType, const MaterialData& materiial);

	//!	描画
	void Render();

	//!	データ有無
	bool IsEmpty()
	{
		return _pVao == nullptr;
	}

	//!	Vao取得
	//!	@return	Vaoのポインタ
	Vao* GetVao()
	{
		return _pVao;
	}

private:
	Vao*	_pVao;
};

};	//	namespace BufferObject


#define ARRAY_COUNT(ARRAY_) (sizeof(ARRAY_) / sizeof(ARRAY_[0]))


//-----------------------------------------------------------------------------
//!	頂点データ
//-----------------------------------------------------------------------------
struct Vertex
{
	//!	コンストラクタ
	Vertex()
	: _position	(Vector3(0.0f,0.0f,0.0f))
	, _normal	(Vector3(0.0f, 0.0f, 0.0f))
	, _color	(Color(255))
	, _uv		(Vector2(-1.0f,-1.0f))
	{}
	//!	コンストラクタ
	//!	@param	[in]	position	座標
	//!	@param	[in]	normal		法線
	//!	@param	[in]	color		色
	//!	@param	[in]	uv			UV座標
	Vertex(const Vector3& position, const Vector3& normal, const Color& color, const Vector2& uv)
	: _position(position)
	, _normal(normal)
	, _color(color)
	, _uv(uv)
	{}
	Vector3 _position;	//!< 頂点
	Vector3 _normal;	//!< 法線
	Color	_color;		//!< 色
	Vector2	_uv;		//!< UV座標
};



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


