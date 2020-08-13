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
//! @brief  �f�[�^�^
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
//! @brief  �`����@
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
//! @brief  �C���f�b�N�X�̌^
//-----------------------------------------------------------------------------
enum class INDEX_TYPE
{
	TYPE_16,
	TYPE_32
};

//-----------------------------------------------------------------------------
//! @brief  �o�b�t�@���
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
//! @brief  VertexBufferObject
//=============================================================================
class Vbo
{
public:
	friend class Vao;	//	Vao�ň������߂Ƀt�����h�ɂ���

	//------------------------------------------------------------------
	//! @brief  �o�b�t�@�f�[�^
	//------------------------------------------------------------------
	struct Desc
	{
		//! @brief  �R���X�g���N�^
		Desc();

		//! @brief  �R���X�g���N�^
		//!	@param	[in]	stride	�f�[�^����̃T�C�Y
		//!	@param	[in]	count	�z��T�C�Y
		//!	@param	[in]	pData	�f�[�^�|�C���^
		//!	@param	[in]	list	�o�b�t�@��񃊃X�g
		Desc(u32 vertexStride,
			 u32 vertexCount,
			 const void* pVertexData,
			 const std::vector<Element>& list);

		//! @brief  ���_�ݒ�
		//!	@param	[in]	stride	�f�[�^����̃T�C�Y
		//!	@param	[in]	count	�z��T�C�Y
		//!	@param	[in]	pData	�f�[�^�|�C���^
		void SetVertexData(u32 stride, u32 count, const void* pData);

		//! @brief  �o�b�t�@���ǉ�
		//!	@param	[in]	pList			���X�g�f�[�^
		//!	@param	[in]	elementCount	�z��T�C�Y
		void SetElementList(const Element* pList, u32 elementCount);

		//! @brief  �o�b�t�@���ǉ�
		//!	@param	[in]	list	�o�b�t�@��񃊃X�g
		void SetElementList(const std::vector<Element>& list);

		u32						_vertexStride;	//!< �f�[�^��̃T�C�Y
		u32						_vertexCount;	//!< �f�[�^�T�C�Y
		const void*				_pVertexData;	//!< �f�[�^�̃|�C���^

		std::vector<Element>	_elementList;	//!< �o�b�t�@��񃊃X�g
	};

public:
	//! @brief  �R���X�g���N�^
	Vbo();
	//! @brief  �f�X�g���N�^
	virtual ~Vbo();

	//! @brief  �f�[�^�쐬
	//!	@param	[in]	desc			�o�b�t�@�f�[�^
	//!	@param	[in]	primitiveType	�`����@
	//!	@retval	true	����
	//!	@retval	false	���s
	bool Create(const Desc& desc, PRIMITIVE_TYPE primitiveType);

	//! @brief  �`����@�ݒ�
	//!	@param	[in]	type	�^�C�v
	//!	@retval	true	����
	//!	@retval	false	���s _primitiveType��0�ɂȂ�
	bool SetPrimitiveType(PRIMITIVE_TYPE type);

	//! @brief  �o�b�t�@�L����
	void Bind();

	//! @brief  �o�b�t�@������
	void UnBind();

	//! @brief  �����œ��̓��C�A�E�g���Ăяo��
	void CallInputLayout();

	//! @brief  id�擾
	//!	@return Vbo��ID
	u32 GetId(){ return _vboId; }

	//! @brief  �`��
	//!	@param	[in]	ibo	IndexBufferObject
	void Render(const Ibo& ibo);

	//! @brief  �ꕔ�`��
	//!	@param	[in]	ibo		IndexBufferObject
	//!	@param	[in]	start	�J�n���_
	//!	@param	[in]	count	�`�悷�钸�_��
	void Render(const Ibo& ibo, u32 start, u32 count);

	//! @brief  �`��
	//!	@param	[in]	ibo		IndexBufferObject
	//!	@param	[in]	start	�J�n���_
	//!	@param	[in]	count	�`�悷�钸�_��
	//!	@param	[in]	size	�`�悷�鐔
	void InstancedRender(const Ibo& ibo, u32 start, u32 count, u32 size);

private:
	u32						_vboId;			//!< VertexBufferObject ID
	u32						_vertexStride;	//!< �f�[�^��̃T�C�Y
	u32						_primitiveType;	//!< �`��^�C�v
	std::vector<Element>	_elementList;	//!< �o�b�t�@���
};

//=============================================================================
//! @brief  IndexBufferObject
//=============================================================================
class Ibo
{
public:
	friend class Vao;	//	Vao�ň������߂Ƀt�����h�ɂ���
	friend class Vbo;	//	Vbo�ň������߂Ƀt�����h�ɂ���

	//------------------------------------------------------------------
	//! @brief  �o�b�t�@�f�[�^
	//------------------------------------------------------------------
	struct Desc
	{
		//! @brief  �R���X�g���N�^
		Desc();

		//! @brief  �R���X�g���N�^
		//!	@param	[in]	stride	�f�[�^����̃T�C�Y
		//!	@param	[in]	count	�z��T�C�Y
		//!	@param	[in]	pData	�f�[�^�|�C���^
		Desc(u32 stride, u32 count, const void* pData);

		//! @brief  �C���f�b�N�X�ݒ�
		//!	@param	[in]	stride	�f�[�^����̃T�C�Y
		//!	@param	[in]	count	�z��T�C�Y
		//!	@param	[in]	pData	�f�[�^�|�C���^
		void SetIndexData(u32 stride, u32 count, const void* pData);

		u32						_indexStride;	//!< �f�[�^��̃T�C�Y
		u32						_indexCount;	//!< �f�[�^�T�C�Y
		const void*				_pIndexData;	//!< �f�[�^�̃|�C���^
	};

public:
	//! @brief  �R���X�g���N�^
	Ibo();

	//! @brief  �f�X�g���N�^
	virtual ~Ibo();

	//! @brief  �f�[�^�쐬
	//!	@param	[in]	desc			�o�b�t�@�f�[�^
	//!	@param	[in]	indexType		�C���f�b�N�X�̌^
	bool Create(const Desc& desc, INDEX_TYPE indexType);

	//! @brief  Ibo��ID�擾
	//!	@return	IBO��ID
	u32 GetId(){ return _iboId; }

	//! @brief  �o�b�t�@�L����
	void Bind()const;

	//! @brief  �o�b�t�@������
	void UnBind()const;
private:
	u32		_iboId;			//!< IndexBufferObject ID
	u32		_indexType;		//!< �C���f�b�N�X�^�C�v
	u32		_indexCount;	//!< �C���f�b�N�X�T�C�Y
};


//=============================================================================
//! @brief  VertexArrayObject
//=============================================================================
class Vao : public MeshBase
{
	friend class VaoObject;
public:
	//------------------------------------------------------------------
	//! @brief  �o�b�t�@�f�[�^
	//------------------------------------------------------------------
	struct Desc
	{
		//! @brief  �R���X�g���N�^
		Desc();

		//! @brief  ���_�ݒ�
		//!	@param	[in]	stride	�f�[�^����̃T�C�Y
		//!	@param	[in]	count	�z��T�C�Y
		//!	@param	[in]	pData	�f�[�^�|�C���^
		void SetVertexData(u32 stride, u32 count, const void* pData);

		//! @brief  �C���f�b�N�X�ݒ�
		//!	@param	[in]	stride	�f�[�^����̃T�C�Y
		//!	@param	[in]	count	�z��T�C�Y
		//!	@param	[in]	pData	�f�[�^�|�C���^
		void SetIndexData(u32 stride, u32 count, const void* pData);

		//! @brief  �o�b�t�@���ǉ�
		//!	@param	[in]	pList			���X�g�f�[�^
		//!	@param	[in]	elementCount	�z��T�C�Y
		void SetElementList(const Element* pList, u32 elementCount);

		//! @brief  �o�b�t�@���ǉ�
		//!	@param	[in]	list	�o�b�t�@��񃊃X�g
		void SetElementList(const std::vector<Element>& list);

		//! @brief  Vbo�o�b�t�@�f�[�^
		operator Vbo::Desc()const
		{
			Vbo::Desc desc(_vertexStride, _vertexCount, _pVertexData, _elementList);
			return desc;
		}

		//! @brief  Ibo�o�b�t�@�f�[�^
		operator Ibo::Desc()const
		{
			Ibo::Desc desc(_indexStride, _indexCount, _pIndexData);
			return desc;
		}

		u32						_vertexStride;	//!< �f�[�^��̃T�C�Y
		u32						_vertexCount;	//!< �f�[�^�T�C�Y
		const void*				_pVertexData;	//!< �f�[�^�̃|�C���^

		u32						_indexStride;	//!< �f�[�^��̃T�C�Y
		u32						_indexCount;	//!< �f�[�^�T�C�Y
		const void*				_pIndexData;	//!< �f�[�^�̃|�C���^

		std::vector<Element>	_elementList;	//!< �o�b�t�@��񃊃X�g
	};

public:
	//! @brief  �R���X�g���N�^
	Vao();

	//! @brief  �R���X�g���N�^
	//!	@param	[in]	desc			�o�b�t�@�f�[�^
	//!	@param	[in]	primitiveType	�`����@
	//!	@param	[in]	indexType		�C���f�b�N�X�̌^
	Vao(const Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType)
	{
		Init(desc, primitiveType, indexType);
	}

	//! @brief  �f�X�g���N�^
	virtual ~Vao();

	//! @brief  �f�[�^�쐬
	//!	@param	[in]	desc			�o�b�t�@�f�[�^
	//!	@param	[in]	primitiveType	�`����@
	//!	@param	[in]	indexType		�C���f�b�N�X�̌^
	bool Init(const Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType);

	//! @brief  �f�[�^�쐬
	//!	@param	[in]	desc			�o�b�t�@�f�[�^
	//!	@param	[in]	primitiveType	�`����@
	//!	@param	[in]	indexType		�C���f�b�N�X�̌^
	static Vao* Create(const Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType);

	// @brief  �`��
	void Render();

private:
	Vbo*	_pVbo;			//!< Vertex Buffer Object
	Ibo*	_pIbo;			//!< Index  Buffer Object
	u32		_vao;			//!< Vertex Array  Object ID
};

//-----------------------------------------------------------------------------
//! @brief  Vao�I�u�W�F�N�g
//-----------------------------------------------------------------------------
class VaoObject : public GameObjectBase
{
public:
	friend Vao;
	//! @brief  �R���X�g���N�^
	VaoObject();

	//! @brief  �R���X�g���N�^
	//!	@param	[in]	desc			�o�b�t�@�f�[�^
	//!	@param	[in]	primitiveType	�`����@
	//!	@param	[in]	indexType		�C���f�b�N�X�̌^
	VaoObject(const Vao::Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType, const MaterialData& material);

	//! @brief  �f�X�g���N�^
	virtual ~VaoObject();

	//! @brief  ���
	void Clear();

	//! @brief  �f�[�^�쐬
	//!	@param	[in]	desc			�o�b�t�@�f�[�^
	//!	@param	[in]	primitiveType	�`����@
	//!	@param	[in]	indexType		�C���f�b�N�X�̌^
	bool Init(const Vao::Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType, const MaterialData& materiial);

	//! @brief  �f�[�^�쐬
	//!	@param	[in]	desc			�o�b�t�@�f�[�^
	//!	@param	[in]	primitiveType	�`����@
	//!	@param	[in]	indexType		�C���f�b�N�X�̌^
	static VaoObject* Create(const Vao::Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType, const MaterialData& materiial);

	//! @brief  �`��
	void Render();

	//! @brief  �f�[�^�L��
	bool IsEmpty()
	{
		return _pVao == nullptr;
	}

	//! @brief  Vao�擾
	//!	@return	Vao�̃|�C���^
	Vao* GetVao()
	{
		return _pVao;
	}

private:
	Vao*	_pVao;
};

};	//	namespace BufferObject

//! @brief  �z��T�C�Y
#define ARRAY_COUNT(ARRAY_) (sizeof(ARRAY_) / sizeof(ARRAY_[0]))

//-----------------------------------------------------------------------------
//! @brief  ���_�f�[�^
//-----------------------------------------------------------------------------
struct Vertex
{
	//! @brief  �R���X�g���N�^
	Vertex()
	: _position	(Vector3(0.0f,0.0f,0.0f))
	, _normal	(Vector3(0.0f, 0.0f, 0.0f))
	, _color	(Color(255))
	, _uv		(Vector2(-1.0f,-1.0f))
	{}

	//! @brief  �R���X�g���N�^
	//!	@param	[in]	position	���W
	//!	@param	[in]	normal		�@��
	//!	@param	[in]	color		�F
	//!	@param	[in]	uv			UV���W
	Vertex(const Vector3& position, const Vector3& normal, const Color& color, const Vector2& uv)
	: _position(position)
	, _normal(normal)
	, _color(color)
	, _uv(uv)
	{}

	Vector3 _position;	//!< ���_
	Vector3 _normal;	//!< �@��
	Color	_color;		//!< �F
	Vector2	_uv;		//!< UV���W
};

//=============================================================================
//! @brief  �{�b�N�X
//=============================================================================
class BoxMesh : public MeshBase
{
	//!	�R���X�g���N�^
	BoxMesh();
	//!	�R���X�g���N�^
	//!	@param	[in]	pParent	�e�I�u�W�F�N�g
	BoxMesh(const MaterialData& material);
	//!	�f�X�g���N�^
	virtual ~BoxMesh();

	//!	�`��
	void Render();
private:
	static RefPointer<BufferObject::Vao>	_pStaticBoxMesh;
};


