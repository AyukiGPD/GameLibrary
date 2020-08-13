//=============================================================================
//!
//!	@file	RendererBuffer.cpp
//!	@brief	�`��o�b�t�@
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

using namespace BufferObject;

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//-----------------------------------------------------------------------------
Vbo::Desc::Desc()
: _vertexStride	(0)
, _vertexCount	(0)
, _pVertexData	(nullptr)
{
}

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//  @param	[in]	stride	�f�[�^����̃T�C�Y
//  @param	[in]	count	�z��T�C�Y
//  @param	[in]	pData	�f�[�^�|�C���^
//  @param	[in]	list	�o�b�t�@��񃊃X�g
//-----------------------------------------------------------------------------
Vbo::Desc::Desc( u32 vertexStride,
				 u32 vertexCount,
				 const void* pVertexData,
				 const std::vector<Element>& list)
: _vertexStride	(vertexStride)
, _vertexCount	(vertexCount)
, _pVertexData	(pVertexData)
, _elementList	(list)
{
}

//-----------------------------------------------------------------------------
//  @brief  ���_�ݒ�
//	@param	[in]	stride	�f�[�^����̃T�C�Y
//	@param	[in]	count	�z��T�C�Y
//	@param	[in]	pData	�f�[�^�|�C���^
//-----------------------------------------------------------------------------
void Vbo::Desc::SetVertexData(u32 stride, u32 count, const void* pData)
{
	_vertexStride = stride;
	_vertexCount = count;
	_pVertexData = pData;
}
//-----------------------------------------------------------------------------
//  @brief  �o�b�t�@���ǉ�
//  @param	[in]	pList			���X�g�f�[�^
//  @param	[in]	elementCount	�z��T�C�Y
//-----------------------------------------------------------------------------
void Vbo::Desc::SetElementList(const Element* pList, u32 elementCount)
{
	if (pList == nullptr || elementCount == 0) return;

	_elementList.clear();
	_elementList.resize(0);
	for (u32 i = 0; i<elementCount; ++i)
	{
		_elementList.push_back(pList[i]);
	}
}
//-----------------------------------------------------------------------------
//  @brief  �o�b�t�@���ǉ�
//  @param	[in]	list	�o�b�t�@��񃊃X�g
//-----------------------------------------------------------------------------
void Vbo::Desc::SetElementList(const std::vector<Element>& list)
{
	const size_t count = list.size();
	if (count == 0) return;

	_elementList.clear();
	_elementList.resize(0);
	for (size_t i = 0; i<count; ++i)
	{
		_elementList.push_back(list[i]);
	}
}

//-----------------------------------------------------------------------------
//! @brief  Vbo�o�b�t�@�f�[�^
//-----------------------------------------------------------------------------
BufferObject::Vao::Desc::operator Vbo::Desc() const
{
	Vbo::Desc desc(_vertexStride, _vertexCount, _pVertexData, _elementList);
	return desc;
}

//-----------------------------------------------------------------------------
//! @brief  Ibo�o�b�t�@�f�[�^
//-----------------------------------------------------------------------------
BufferObject::Vao::Desc::operator Ibo::Desc() const
{
	Ibo::Desc desc(_indexStride, _indexCount, _pIndexData);
	return desc;
}

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//-----------------------------------------------------------------------------
Vbo::Vbo()
: _vboId(0)
, _vertexStride(0)
, _primitiveType(0)
{
}

//-----------------------------------------------------------------------------
//  @brief  �f�X�g���N�^
//-----------------------------------------------------------------------------
Vbo::~Vbo()
{
	glDeleteBuffers(1, &_vboId);
}

//-----------------------------------------------------------------------------
//  @brief  �f�[�^�쐬
//  @param	[in]	desc			�o�b�t�@�f�[�^
//  @param	[in]	primitiveType	�`����@
//  @retval	true	����
//  @retval	false	���s
//-----------------------------------------------------------------------------
bool Vbo::Create(const Desc& desc, PRIMITIVE_TYPE primitiveType)
{
	if (!desc._pVertexData)
	{
		return false;
	}

	// Vbo�̍쐬
	{
		glGenBuffers(1, &_vboId);
		glBindBuffer(GL_ARRAY_BUFFER, _vboId);
		glBufferData(
			GL_ARRAY_BUFFER,
			desc._vertexStride * desc._vertexCount,
			desc._pVertexData,
			GL_STATIC_DRAW
			);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	_vertexStride = desc._vertexStride;
	_elementList = desc._elementList;

	SetPrimitiveType(primitiveType);


	return true;
}

//-----------------------------------------------------------------------------
//  @brief  �`����@�ݒ�
//  @param	[in]	type	�^�C�v
//  @retval	true	����
//  @retval	false	���s
//-----------------------------------------------------------------------------
bool Vbo::SetPrimitiveType(PRIMITIVE_TYPE type)
{
	//	��Ɨp�^�C�v
	u32 primitiveType = 0;

	switch (type)
	{
	case PRIMITIVE_TYPE::POINTS:
		primitiveType = GL_POINTS;
		break;
	case PRIMITIVE_TYPE::LINE:
		primitiveType = GL_LINES;
		break;
	case PRIMITIVE_TYPE::LINE_LOOP:
		primitiveType = GL_LINE_LOOP;
		break;
	case PRIMITIVE_TYPE::LINE_STRIP:
		primitiveType = GL_LINE_STRIP;
		break;
	case PRIMITIVE_TYPE::TRIANGLES:
		primitiveType = GL_TRIANGLES;
		break;
	case PRIMITIVE_TYPE::TRIANGLE_FAN:
		primitiveType = GL_TRIANGLE_FAN;
		break;
	case PRIMITIVE_TYPE::TRIANGLE_STRIP:
		primitiveType = GL_TRIANGLE_STRIP;
		break;
	case PRIMITIVE_TYPE::POLYGON:
		primitiveType = GL_POLYGON;
		break;
	case PRIMITIVE_TYPE::UNKNOWN:
	default:
		/* ���w�� */
		primitiveType = 0;
	}

	//	���s��
	if (primitiveType == 0)
	{
		return false;
	}

	//	�������Ă���΍X�V
	_primitiveType = primitiveType;
	return true;
}

//-----------------------------------------------------------------------------
//  @brief  �o�b�t�@�L����
//-----------------------------------------------------------------------------
void Vbo::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vboId);
}
//-----------------------------------------------------------------------------
//  @brief  �o�b�t�@������
//-----------------------------------------------------------------------------
void Vbo::UnBind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//-----------------------------------------------------------------------------
//  @brief  ���̓��C�A�E�g
//-----------------------------------------------------------------------------
void Vbo::CallInputLayout()
{
	const size_t count = _elementList.size();
	for (size_t i = 0; i<count; ++i)
	{
		const Element& p = _elementList[i];

		glVertexAttribPointer(
			p._locationIndex,
			p._count,
			(GLenum)p._elementType,
			p._isNormalize ? GL_TRUE : GL_FALSE,
			_vertexStride,
			(GLvoid*)p._strideOffset
			);

		glEnableVertexAttribArray(p._locationIndex);
	}
}

//-----------------------------------------------------------------------------
//  @brief  �`��
//  @param	[in]	ibo	IndexBufferObject
//-----------------------------------------------------------------------------
void Vbo::Render(const Ibo& ibo)
{
	if (_primitiveType == 0)
	{
		return;
	}
	if (ibo._indexCount == 0 || ibo._indexType == 0)
	{
		return;
	}

	Bind();
	ibo.Bind();

	CallInputLayout();

	glDrawElements(_primitiveType, ibo._indexCount, ibo._indexType, nullptr);

	ibo.UnBind();
	UnBind();
}

//-----------------------------------------------------------------------------
//  @brief  �ꕔ�`�� �ꕔ�̕`��Ȃ̂�Bind�͂��Ȃ�
//  @param	[in]	ibo		IndexBufferObject
//  @param	[in]	start	�J�n���_
//  @param	[in]	count	�`�悷�钸�_��
//-----------------------------------------------------------------------------
void Vbo::Render(const Ibo& ibo,u32 start, u32 count)
{
	if(_primitiveType == 0)
	{
		return;
	}
	if(ibo._indexCount == 0 || ibo._indexType == 0)
	{
		return;
	}

	glDrawRangeElementsBaseVertex(
		_primitiveType,
		start,
		start + count,
		count,
		ibo._indexType,
		nullptr,
		start
		);
}

//-----------------------------------------------------------------------------
//  @brief  �`��
//  @param	[in]	ibo		IndexBufferObject
//  @param	[in]	start	�J�n���_
//  @param	[in]	count	�`�悷�钸�_��
//  @param	[in]	size	�`�悷�鐔
//-----------------------------------------------------------------------------
void Vbo::InstancedRender(const Ibo& ibo, u32 start, u32 count, u32 size)
{
	if(_primitiveType == 0)
	{
		return;
	}
	if(ibo._indexCount == 0 || ibo._indexType == 0)
	{
		return;
	}

	//	https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glDrawElementsInstancedBaseVertexBaseInstance.xml
	//	https://www.opengl.org/wiki/GLAPI/glDrawElementsInstancedBaseVertexBaseInstance

	glDrawElementsInstancedBaseVertexBaseInstance(
		_primitiveType,
		count,
		ibo._indexType,
		nullptr,
		start + count,
		start,
		size
		);
}

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//-----------------------------------------------------------------------------
Ibo::Desc::Desc()
: _indexStride	(0)
, _indexCount	(0)
, _pIndexData	(nullptr)
{
}

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//  @param	[in]	stride	�f�[�^����̃T�C�Y
//  @param	[in]	count	�z��T�C�Y
//  @param	[in]	pData	�f�[�^�|�C���^
//-----------------------------------------------------------------------------
Ibo::Desc::Desc(u32 stride, u32 count, const void* pData)
: _indexStride	(stride)
, _indexCount	(count)
, _pIndexData	(pData)
{
}

//-----------------------------------------------------------------------------
//  @brief  �C���f�b�N�X�ݒ�
//  @param	[in]	stride	�f�[�^����̃T�C�Y
//  @param	[in]	count	�z��T�C�Y
//  @param	[in]	pData	�f�[�^�|�C���^
//-----------------------------------------------------------------------------
void Ibo::Desc::SetIndexData(u32 stride, u32 count, const void* pData)
{
	_indexStride = stride;
	_indexCount = count;
	_pIndexData = pData;
}

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//-----------------------------------------------------------------------------
Ibo::Ibo()
: _iboId(0)
, _indexType(0)
, _indexCount(0)
{
}

//-----------------------------------------------------------------------------
//  @brief  �f�X�g���N�^
//-----------------------------------------------------------------------------
Ibo::~Ibo()
{
	glDeleteBuffers(1, &_iboId);
}

//-----------------------------------------------------------------------------
//  @brief  �f�[�^�쐬
//  @param	[in]	desc			�o�b�t�@�f�[�^
//  @param	[in]	indexType		�C���f�b�N�X�̌^
//-----------------------------------------------------------------------------
bool Ibo::Create(const Desc& desc, INDEX_TYPE indexType)
{
	if (!desc._pIndexData)
	{
		return false;
	}

	// Ibo�̍쐬
	{
		glGenBuffers(1, &_iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboId);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			desc._indexStride * desc._indexCount,
			desc._pIndexData,
			GL_STATIC_DRAW
			);
	}

	switch (indexType)
	{
	case INDEX_TYPE::TYPE_16:
		_indexType = GL_UNSIGNED_SHORT;
		break;
	case INDEX_TYPE::TYPE_32:
		_indexType = GL_UNSIGNED_INT;
		break;
	default:
		/* ���w�� */
		_indexType = 0;
		return false;
	}

	//	�C���f�b�N�X���擾
	_indexCount = desc._indexCount;
	return true;
}


//-----------------------------------------------------------------------------
//  @brief  �o�b�t�@�L����
//-----------------------------------------------------------------------------
void Ibo::Bind()const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iboId);
}

//-----------------------------------------------------------------------------
//  @brief  �o�b�t�@������
//-----------------------------------------------------------------------------
void Ibo::UnBind()const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//-----------------------------------------------------------------------------
Vao::Desc::Desc()
: _vertexStride	(0)
, _vertexCount	(0)
, _pVertexData	(nullptr)
, _indexStride	(0)
, _indexCount	(0)
, _pIndexData	(nullptr)
{
	_elementList.resize(0);
}

//-----------------------------------------------------------------------------
//  @brief  ���_�ݒ�
//  @param	[in]	stride	�f�[�^����̃T�C�Y
//  @param	[in]	count	�z��T�C�Y
//  @param	[in]	pData	�f�[�^�|�C���^
//-----------------------------------------------------------------------------
void Vao::Desc::SetVertexData(u32 stride, u32 count, const void* pData)
{
	_vertexStride = stride;
	_vertexCount  = count;
	_pVertexData  = pData;
}

//-----------------------------------------------------------------------------
//  @brief  �C���f�b�N�X�ݒ�
//  @param	[in]	stride	�f�[�^����̃T�C�Y
//  @param	[in]	count	�z��T�C�Y
//  @param	[in]	pData	�f�[�^�|�C���^
//-----------------------------------------------------------------------------
void Vao::Desc::SetIndexData(u32 stride, u32 count, const void* pData)
{
	_indexStride = stride;
	_indexCount  = count;
	_pIndexData  = pData;
}

//-----------------------------------------------------------------------------
//  @brief  �o�b�t�@���ǉ�
//  @param	[in]	pList			���X�g�f�[�^
//  @param	[in]	elementCount	�z��T�C�Y
//-----------------------------------------------------------------------------
void Vao::Desc::SetElementList(const Element* pList, u32 elementCount)
{
	if( pList == nullptr || elementCount == 0 ) return;

	_elementList.clear();
	_elementList.resize(0);
	for(u32 i=0; i<elementCount; ++i)
	{
		_elementList.push_back(pList[i]);
	}
}

//-----------------------------------------------------------------------------
//  @brief  �o�b�t�@���ǉ�
//  @param	[in]	list	���X�g�f�[�^
//-----------------------------------------------------------------------------
void Vao::Desc::SetElementList(const std::vector<Element>& list)
{
	const size_t count = list.size();
	if( count == 0 ) return;

	_elementList.clear();
	_elementList.resize(0);
	for (size_t i = 0; i < count; ++i)
	{
		_elementList.push_back(list[i]);
	}
}

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//-----------------------------------------------------------------------------
Vao::Vao()
: _vao(0)
, _pVbo(nullptr)
, _pIbo(nullptr)
{
	AddTag("Vao");
}

//-----------------------------------------------------------------------------
//! @brief  �R���X�g���N�^
//!	@param	[in]	desc			�o�b�t�@�f�[�^
//!	@param	[in]	primitiveType	�`����@
//!	@param	[in]	indexType		�C���f�b�N�X�̌^
//-----------------------------------------------------------------------------
BufferObject::Vao::Vao(const Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType)
{
	Init(desc, primitiveType, indexType);
}
//-----------------------------------------------------------------------------
//  @brief  �f�X�g���N�^
//-----------------------------------------------------------------------------
Vao::~Vao()
{
	SafeDelete(_pVbo);
	SafeDelete(_pIbo);
	glDeleteVertexArrays(1,&_vao);
}


//-----------------------------------------------------------------------------
//  @brief  �f�[�^�쐬
//  @param	[in]	desc			�o�b�t�@�f�[�^
//  @param	[in]	primitiveType	�`����@
//  @param	[in]	indexType		�C���f�b�N�X�̌^
//-----------------------------------------------------------------------------
bool Vao::Init(const Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType)
{
	if (!desc._pVertexData)
	{
		return false;
	}
	if (!desc._pIndexData)
	{
		return false;
	}
	// Vao�̍쐬
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	bool result = true;
	Vbo* pVbo = new Vbo();
	if (pVbo)
	{
		if (!pVbo->Create((Vbo::Desc)desc, primitiveType))
		{
			result = false;
		}
	}
	else{ result = false; }

	Ibo* pIbo = new Ibo();
	if (pIbo)
	{
		if (!pIbo->Create((Ibo::Desc)desc, indexType))
		{
			result = false;
		}
	}
	else
	{
		result = false;
	}
	if (result)
	{
		_pVbo = pVbo;

		_pVbo->Bind();
		_pVbo->CallInputLayout();
		_pVbo->UnBind();

		_pIbo = pIbo;
	}
	else
	{
		SafeDelete(pVbo);
		SafeDelete(pIbo);
	}
	glBindVertexArray(0);

	return result;
}

//-----------------------------------------------------------------------------
//  @brief  �f�[�^�쐬
//  @param	[in]	desc			�o�b�t�@�f�[�^
//  @param	[in]	primitiveType	�`����@
//  @param	[in]	indexType		�C���f�b�N�X�̌^
//-----------------------------------------------------------------------------
Vao* Vao::Create(const Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType)
{
	Vao* result = nullptr;
	result = new Vao;

	if (!result->Init(desc, primitiveType, indexType))
	{
		SafeRelease(result);
	}

	return result;
}

//-----------------------------------------------------------------------------
//  @brief  �`��
//-----------------------------------------------------------------------------
void Vao::Render()
{
	if (!_pVbo || !_pIbo)
	{
		return;
	}

	BindMaterial();

	u32 primitiveType = _pVbo->_primitiveType;
	u32 indexCount = _pIbo->_indexCount;
	u32 indexType = _pIbo->_indexType;

	if (primitiveType == 0 || indexType == 0 || indexCount == 0)
	{
		return;
	}


	// �`��
	glBindVertexArray(_vao);
	glDrawElements(primitiveType, indexCount, indexType, nullptr);
	isErrorGL();

	glBindVertexArray(0);

//	_pVbo->Render(*_pIbo);
}

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//-----------------------------------------------------------------------------
VaoObject::VaoObject()
:_pVao(nullptr)
{
	AddTag("VaoObject");
}

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//  @param	[in]	desc			�o�b�t�@�f�[�^
//  @param	[in]	primitiveType	�`����@
//  @param	[in]	indexType		�C���f�b�N�X�̌^
//-----------------------------------------------------------------------------
VaoObject::VaoObject(const Vao::Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType, const MaterialData& material)
:_pVao(nullptr)
{
	_pVao = new Vao();
	if(!_pVao->Create(desc, primitiveType, indexType))
	{
		SafeRelease(_pVao);
	}
	else
	{
		_pVao->AddMateiral(material);
	}
	AddTag("VaoObject");
}

//-----------------------------------------------------------------------------
//  @brief  �f�X�g���N�^
//-----------------------------------------------------------------------------
VaoObject::~VaoObject()
{
	Clear();
}

//-----------------------------------------------------------------------------
//  @brief  ���
//-----------------------------------------------------------------------------
void VaoObject::Clear()
{
	SafeRelease(_pVao);
}

//-----------------------------------------------------------------------------
//  @brief  �f�[�^�쐬
//  @param	[in]	desc			�o�b�t�@�f�[�^
//  @param	[in]	primitiveType	�`����@
//  @param	[in]	indexType		�C���f�b�N�X�̌^
//-----------------------------------------------------------------------------
bool VaoObject::Init(const Vao::Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType, const MaterialData& materiial)
{
	_pVao = Vao::Create(desc, primitiveType, indexType);
	if (!_pVao)
	{
		return false;
	}
	AddMesh(_pVao);
	_pVao->AddMateiral(materiial);
	return true;
}

//-----------------------------------------------------------------------------
//  @brief  �f�[�^�쐬
//  @param	[in]	desc			�o�b�t�@�f�[�^
//  @param	[in]	primitiveType	�`����@
//  @param	[in]	indexType		�C���f�b�N�X�̌^
//-----------------------------------------------------------------------------
VaoObject* VaoObject::Create(const Vao::Desc& desc, PRIMITIVE_TYPE primitiveType, INDEX_TYPE indexType, const MaterialData& materiial)
{
	VaoObject* result = nullptr;
	result = new VaoObject;
	if (!result->Init(desc, primitiveType, indexType,materiial))
	{
		SafeRelease(result);
	}
	return result;
}

//-----------------------------------------------------------------------------
//  @brief  �`��
//-----------------------------------------------------------------------------
void VaoObject::Render()
{
}

//-----------------------------------------------------------------------------
//  @brief  �f�[�^�L��
//-----------------------------------------------------------------------------
bool BufferObject::VaoObject::IsEmpty()
{
	return _pVao == nullptr;
}

//-----------------------------------------------------------------------------
//  @brief  Vao�擾
//  @return	Vao�̃|�C���^
//-----------------------------------------------------------------------------
Vao* BufferObject::VaoObject::GetVao()
{
	return _pVao;
}

static const f32 gBoxSize = 1.0f;

//-----------------------------------------------------------------------------
//! @brief  ���_���
//-----------------------------------------------------------------------------
static const Vertex gBoxVertices[] =
{
	// �O
	{ Vector3(-gBoxSize, +gBoxSize, +gBoxSize), Vector3(0, 0, +1), Color(255), Vector2(0, 1) },
	{ Vector3(+gBoxSize, +gBoxSize, +gBoxSize), Vector3(0, 0, +1), Color(255), Vector2(1, 1) },
	{ Vector3(-gBoxSize, -gBoxSize, +gBoxSize), Vector3(0, 0, +1), Color(255), Vector2(0, 0) },
	{ Vector3(+gBoxSize, -gBoxSize, +gBoxSize), Vector3(0, 0, +1), Color(255), Vector2(1, 0) },

	// ��
	{ Vector3(+gBoxSize, +gBoxSize, -gBoxSize), Vector3(0, 0, -1), Color(255), Vector2(1, 1) },
	{ Vector3(-gBoxSize, +gBoxSize, -gBoxSize), Vector3(0, 0, -1), Color(255), Vector2(0, 1) },
	{ Vector3(+gBoxSize, -gBoxSize, -gBoxSize), Vector3(0, 0, -1), Color(255), Vector2(1, 0) },
	{ Vector3(-gBoxSize, -gBoxSize, -gBoxSize), Vector3(0, 0, -1), Color(255), Vector2(0, 0) },

	// ��
	{ Vector3(-gBoxSize, +gBoxSize, -gBoxSize), Vector3(-1, 0, 0), Color(255), Vector2(1, 0) },
	{ Vector3(-gBoxSize, +gBoxSize, +gBoxSize), Vector3(-1, 0, 0), Color(255), Vector2(1, 1) },
	{ Vector3(-gBoxSize, -gBoxSize, -gBoxSize), Vector3(-1, 0, 0), Color(255), Vector2(0, 0) },
	{ Vector3(-gBoxSize, -gBoxSize, +gBoxSize), Vector3(-1, 0, 0), Color(255), Vector2(0, 1) },

	// �E
	{ Vector3(+gBoxSize, +gBoxSize, +gBoxSize), Vector3(+1, 0, 0), Color(255), Vector2(1, 1) },
	{ Vector3(+gBoxSize, +gBoxSize, -gBoxSize), Vector3(+1, 0, 0), Color(255), Vector2(1, 0) },
	{ Vector3(+gBoxSize, -gBoxSize, +gBoxSize), Vector3(+1, 0, 0), Color(255), Vector2(0, 1) },
	{ Vector3(+gBoxSize, -gBoxSize, -gBoxSize), Vector3(+1, 0, 0), Color(255), Vector2(0, 0) },

	// ��
	{ Vector3(-gBoxSize, +gBoxSize, -gBoxSize), Vector3(0, +1, 0), Color(255), Vector2(0, 0) },
	{ Vector3(+gBoxSize, +gBoxSize, -gBoxSize), Vector3(0, +1, 0), Color(255), Vector2(1, 0) },
	{ Vector3(-gBoxSize, +gBoxSize, +gBoxSize), Vector3(0, +1, 0), Color(255), Vector2(0, 1) },
	{ Vector3(+gBoxSize, +gBoxSize, +gBoxSize), Vector3(0, +1, 0), Color(255), Vector2(1, 1) },

	// ��
	{ Vector3(-gBoxSize, -gBoxSize, +gBoxSize), Vector3(0, -1, 0), Color(255), Vector2(0, 1) },
	{ Vector3(+gBoxSize, -gBoxSize, +gBoxSize), Vector3(0, -1, 0), Color(255), Vector2(1, 1) },
	{ Vector3(-gBoxSize, -gBoxSize, -gBoxSize), Vector3(0, -1, 0), Color(255), Vector2(0, 0) },
	{ Vector3(+gBoxSize, -gBoxSize, -gBoxSize), Vector3(0, -1, 0), Color(255), Vector2(1, 0) },
};

//-----------------------------------------------------------------------------
//! @brief  ���_�C���f�b�N�X
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
//! @brief  �o�b�t�@���
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
//  @brief  �R���X�g���N�^
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
//  @brief  �f�X�g���N�^
//-----------------------------------------------------------------------------
BoxMesh::~BoxMesh()
{
}

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
//  @param	[in]	pParent	�e�I�u�W�F�N�g
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
//  @brief  �`��
//-----------------------------------------------------------------------------
void BoxMesh::Render()
{
	if (!_pStaticBoxMesh){ assert(false); }

	BindMaterial(0);
	_pStaticBoxMesh->Render();

}


