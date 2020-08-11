//=============================================================================
//!
//!	@file	Model.cpp
//!	@brief	���f���N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"



//=============================================================================
//!	���f���I�u�W�F�N�g
//=============================================================================
class Model::Object
{
public:
	//! �R���X�g���N�^
	Object(ModelData* pModel)
	: _pModel(pModel)
	, _pVbo(nullptr)
	, _pIbo(nullptr)
	, _visible(0)
	{
	}
	//!	�f�X�g���N�^
	virtual ~Object()
	{
		Clear();
	}

	//! �`��
	void Render();

	//!	���b�V����ݒ�
	//!	@param	
	void SetModelMesh(ModelData* pModelMesh)
	{
		_pModel = pModelMesh;
	}


	//! �f�[�^�\�z
	bool Build();

	//!	���
	void Clear();

	//!	�`����@�ݒ�
	//!	@param	[in]	type	�`��^�C�v
	//!	@retval	true	����
	//!	@retval	false	���s
	bool SetRenderMode(BufferObject::PRIMITIVE_TYPE type);

	//!	�e�N�X�`�����ƂɃ��b�V�������邽�߂̃f�[�^
	struct Cluster
	{
		s32			_indexStart;
		s32			_indexCount;
		Texture*	_pTexture;
	};

	//!	�ʒǉ�
	//!	@param	[in]	face	�ʃf�[�^
	void AddFace(const Model::Face& face);

private:
	friend ModelData;

	s32 _visible;								//!< �\���t���O

	std::vector<Vector3>		_vertices;		//!< ���_�z��
	std::vector<Vector3>		_normals;		//!< �@���z��
	std::vector<Model::Face>	_faces;			//!< �|���S���z��

	ModelData*					_pModel;		//!< �����郂�f���ւ̃|�C���^

	BufferObject::Vbo*			_pVbo;			//!< Vbo
	BufferObject::Ibo*			_pIbo;			//!< Ibo

	std::vector<Cluster>		_clusters;		//!< �����f�[�^

};

//=============================================================================
//!	�|���S���f�[�^
//=============================================================================
class Model::Face
{
	static const s32 VERTEX_COUNT_MAX = 4;
public:
	//!	�R���X�g���N�^
	Face()
	:_vertexCount(3)
	,_material(-1)
	{
		for (auto& pNormal : _pNormals)
		{
			pNormal = nullptr;
		}
		//_pNormals.assign(nullptr);
	}

	s32 GetMaterialNumber() const
	{
		return _material;
	}

	bool operator < (const Model::Face& other)
	{
		// �}�e���A���ԍ��ő召�֌W�����߂�
		return (_material < other.GetMaterialNumber());
	}

private:
	friend ModelData;
	friend Model::Object;

	s32									 _vertexCount;		//!< ���_��  3:�O�p�` 4:�l�p�`
	std::array<	   s32,VERTEX_COUNT_MAX> _vertexIndex;		//!< ���_�ԍ�
	std::array<Vector2,VERTEX_COUNT_MAX> _uv         ;		//!< �e�N�X�`��UV���W
	s32									 _material;			//!< �}�e���A���ԍ�


	
	/*!
		@todo

		���L���_�̖@���쐬���@

		�@���𒸓_���쐬
		���g�̓|�C���^
		���̂�Object�̒��_�ƈꏏ�Ɏ����Ă���
		
		�|���S���������ɖ@���̌v�Z������
		���̃C���f�b�N�X�ԍ��̒��_�ɖ@���̃f�[�^������
		�����ē��ꂽ�f�[�^�̃|�C���^���擾����

		�ʃx�N�g���̖@�����K�v�ȂƂ���
		Object�̖@����push_back�ŕ�������
		���̕��������f�[�^�̃|�C���^�ɏ㏑��

		���̏�����Object���ŋL�q����
	*/
	Vector3 _normal;	//!< �ʖ@��

	std::array<Vector3*, VERTEX_COUNT_MAX> _pNormals;	//!< ���_�@��

};

//=============================================================================
//!	���f���̃f�[�^��ێ�����I�u�W�F�N�g
//=============================================================================
//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
ModelData::ModelData()
{
}

//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
ModelData::~ModelData()
{
	for (auto& pObject : _pObjects)
	{
		SafeDelete(pObject);
	}

	_pObjects.clear();
}

//-----------------------------------------------------------------------------
//!	�`��
//-----------------------------------------------------------------------------
void ModelData::Render()
{
	for( auto* pObject : _pObjects)
	{
		pObject->Render();
	}
}

//-----------------------------------------------------------------------------
//!	�ǂݍ���
//!	@param	[in]	fileName	�t�@�C����
//!	@param	[in]	scale		�X�P�[��
//!	@retval	true	����
//!	@retval	false	���s
//-----------------------------------------------------------------------------
bool ModelData::Load(const std::string& fileName, f32 scale)
{
	bool result = false;
	size_t index = fileName.rfind(".", fileName.size() - 1);
	if (index != std::string::npos)
	{
		std::string ext = fileName.substr(index);	//	�g���q
		if (ext == ".mqo" || ext == ".MQO")
		{
			result = LoadMQO(fileName, scale);
		}
		//else if (ext == ".fbx" || ext == ".FBX")
		//{
		//	result = LoadFBX(fileName, scale);
		//}
		//else if (ext == ".x" || ext == ".X")
		//{
		//	result = _LoadXFile(fileName, scale);
		//}
		//else if (ext == ".obj" || ext == ".OBJ")
		//{
		//	result = LoadOBJ(fileName, scale);
		//}
		else
		{
			ERROR_LOG("File Format Error");
		}
	}
	return result;
}

//	mqo�t�@�C���t�H�[�}�b�g����T�C�g
//http://metaseq.net/jp/format.html

//-----------------------------------------------------------------------------
//! MQO�t�@�C���̓ǂݍ���
//!	@param	[in]	fileName	MQO�t�@�C����
//!	@param	[in]	scale		�g�嗦
//!	@retval	true	����
//!	@retval	false	���s
//-----------------------------------------------------------------------------
bool ModelData::LoadMQO(const std::string& fileName, f32 scale)
{
	char	drive[MAX_PATH];
	char	path[MAX_PATH];
	char	file[MAX_PATH];
	char	ext[MAX_PATH];		// ".mqo"

	_splitpath_s(fileName.c_str(), drive, path, file, ext);

	//------------------------------------------------------------------
	// �t�@�C�����J��
	//------------------------------------------------------------------
	FILE*	fp = nullptr;
	fopen_s(&fp, fileName.c_str(), "rb");
	if (fp == nullptr)
	{
		DEBUG_LOG(L"�t�@�C���̃I�[�v���Ɏ��s���܂���.");
		return false;
	}

	enum class STATE
	{
		ROOT,			// ��ԍŏ�ʂ̊K�w
		MATERIAL,		// �}�e���A����͒�
		OBJECT,			// Object�`�����N��͒�
		OBJECT_VERTEX,	// Object�`�����N���̒��_���W
		OBJECT_FACE,	// Object�`�����N���̃|���S�����
	};

	STATE	state = STATE::ROOT;
	char	str[1024];

	Model::Object*	pObject = nullptr;

	// �P�s�ǂݍ���
	while (fgets(&str[0], sizeof(str), fp) != 0)
	{
		switch (state)
		{
			//------------------------------------------------------------
		case STATE::ROOT:		// �ŏ�ʂ̊K�w
			//------------------------------------------------------------
			if (strncmp(str, "Material", 8) == 0)
			{
				state = STATE::MATERIAL;
				break;
			}


			if (strncmp(str, "Object", 6) == 0)
			{
				pObject = new Model::Object(this);
				_pObjects.push_back(pObject);		// �V�K�쐬�����I�u�W�F�N�g�����̒i�K�Ŕz��ɓo�^

				state = STATE::OBJECT;
				break;
			}
			break;
			//------------------------------------------------------------
		case STATE::MATERIAL:		// �}�e���A��
			//------------------------------------------------------------
		{
			//-----�}�e���A���I�[�`�F�b�N
			if (strncmp(str, "}", 1) == 0)
			{
				state = STATE::ROOT;	// �߂�
				break;
			}
			//---------�}�e���A���ǉ�
			MaterialData	material;// ���̍s���o�^����΂��̍s���Ȃ��Ȃ�

			char* p = nullptr;

			//---- �ގ��J���[
			p = strstr(str, " col(");
			if (p != nullptr)
			{
				Vector4 colorF;
				sscanf_s(p, " col(%f %f %f %f)",
					&colorF.x,		// R
					&colorF.y,		// G
					&colorF.z,		// B
					&colorF.w);		// A
				material._color.Set(colorF);
			}


			// �e�N�X�`���[�̓ǂݍ���
			p = strstr(str, "tex(");
			if (p != nullptr)
			{
				// 1)�u�h�v�L��������
				p = strchr(p, '"');
				// 2) 1�������i�߂�( �t�@�C�����̐擪 )
				p++;
				char* pFilePath = p;

				// 3) �����́@�h�@�L��������
				p = strchr(p, '"');
				// 4) �����I�[�R�[�h����������ŕ��f����

				*p = '\0';

				// �e�N�X�`����ǂݍ���
				AssetTexture* pTexture = new AssetTexture();
				if (pTexture)
				{
					char	fullPath[MAX_PATH];
					_makepath_s(fullPath, drive, path, nullptr, nullptr);
					strcat_s(fullPath, pFilePath);	// �h���C�u�p�X + �e�N�X�`���t�@�C����
					if (!pTexture->Load(fullPath))
					{
						SafeDelete(pTexture);
						return false;
					}
					material._pTexture = pTexture;
				}
				else
				{
					return false;
				}
			}

			_modelMaterials.push_back(material);
		}
		break;
		//------------------------------------------------------------
		case STATE::OBJECT:		// Object�`�����N��͒�
			//------------------------------------------------------------
		{
			if (strncmp(str, "}", 1) == 0)
			{
				state = STATE::ROOT;
				break;
			}

			if (strstr(str, "visible") != nullptr)
			{
				s32 visible = -1;
				sscanf_s(str, " visible %d", &visible);
				pObject->_visible = visible;
				break;
			}

			if (strstr(str, "vertex") != nullptr)
			{
				state = STATE::OBJECT_VERTEX;
				break;
			}

			if (strstr(str, "face ") != nullptr)
			{
				state = STATE::OBJECT_FACE;
				break;
			}
		}
		break;
		//-------------------------------------------------------------------------
		case STATE::OBJECT_VERTEX:	// Object�`�����N���̒��_���W
			//-------------------------------------------------------------------------
		{
			if (strstr(str, "}") != nullptr)
			{
				state = STATE::OBJECT;
				break;
			}

			//-----------------------------------------------------------
			// ���_�̓ǂݍ���
			//-----------------------------------------------------------
			f32	x, y, z;

			sscanf_s(str, " %f %f %f", &x, &y, &z);

			x *= scale;
			y *= scale;
			z *= scale;

			pObject->_vertices.push_back(Vector3(x, y, z));
			pObject->_normals.push_back(Vector3::ZERO);	//	���_�Ɠ������@���̘g�����
		}
		break;
		//-------------------------------------------------------------------------
		case STATE::OBJECT_FACE:		// Object�`�����N���̃|���S�����
			//-------------------------------------------------------------------------
		{
			//---- �I�[�`�F�b�N
			if (strstr(str, "}") != nullptr)
			{
				state = STATE::OBJECT;
				break;
			}

			//---- �|���S���ǂݍ���
			Model::Face		face;

			// �O�p�` or �l�p�` �����o
			sscanf_s(str, " %d", &face._vertexCount);

			char*	p;
			// �}�e���A���ԍ�
			p = strstr(str, "M(");
			if (p)
			{
				sscanf_s(p, "M(%d)", &face._material);
			}

			if (face._vertexCount == 3)
			{
				//---- �O�p�`�̏ꍇ

				// ���_�C���f�b�N�X


				p = strstr(str, "V(");
				sscanf_s(p, "V(%d %d %d)", &face._vertexIndex[2], &face._vertexIndex[1], &face._vertexIndex[0]);
				face._vertexIndex[3] = -1;		// ���g�p

				// UV���W
				p = strstr(str, "UV(");
				if (p)
				{
					sscanf_s(p, "UV(%f %f %f %f %f %f)",
						&face._uv[2].x, &face._uv[2].y,		// UV2
						&face._uv[1].x, &face._uv[1].y,		// UV1
						&face._uv[0].x, &face._uv[0].y);	// UV0
				}

			}
			else if (face._vertexCount == 4)
			{
				//---- �l�p�`�̏ꍇ

				// ���_�C���f�b�N�X
				p = strstr(str, "V(");
				sscanf_s(p, "V(%d %d %d %d)", &face._vertexIndex[3], &face._vertexIndex[2], &face._vertexIndex[1], &face._vertexIndex[0]);

				// UV���W
				p = strstr(str, "UV(");
				if (p)
				{
					sscanf_s(p, "UV(%f %f %f %f %f %f %f %f)",
						&face._uv[3].x, &face._uv[3].y,
						&face._uv[2].x, &face._uv[2].y,
						&face._uv[1].x, &face._uv[1].y,
						&face._uv[0].x, &face._uv[0].y);
				}

			}
			else
			{
			}

			//	�z��֓o�^
			pObject->AddFace(face);
		}
		break;

		}


	}


	//-------------------------------------------------------------
	// �t�@�C�������
	//-------------------------------------------------------------
	fclose(fp);

	//-------------------------------------------------------------
	// �}�e���A���P�ʂɃ|���S������ёւ���
	//-------------------------------------------------------------
	for (auto* pModelObject : _pObjects)
	{
		if (pModelObject->Build() == false)
		{
			DEBUG_LOG(L"BuildError");
		}
	}


	//	�ǂݍ��݂Ń}�e���A������������Ȃ������ꍇ�f�t�H���g�̃}�e���A����ǉ����Ă���
	if (_modelMaterials.size() == 0)
	{
		_modelMaterials.push_back(MaterialData::DEFAULT);
	}

	_fileName = fileName;

	//---- ����
	return true;
}


//=============================================================================
//!	���f�����b�V���N���X
//=============================================================================
//-----------------------------------------------------------------------------
//	�R���X�g���N�^
//-----------------------------------------------------------------------------
ModelMesh::ModelMesh()
{
	AddTag("ModelInstance");
}
//-----------------------------------------------------------------------------
//	�f�X�g���N�^
//-----------------------------------------------------------------------------
ModelMesh::~ModelMesh()
{
}

//-----------------------------------------------------------------------------
//!	���f���ǂݍ���
//!	@param	[in]	fileName	�t�@�C����
//!	@param	[in]	scale		�g�嗦
//!	@retval	true	����
//!	@retval	false	���s
//-----------------------------------------------------------------------------
bool ModelMesh::Load(const std::string& fileName, f32 scale)
{
	bool result = false;
	_pMeshData = CreateRefPointer<ModelData>();
	ModelData* pModelData = dynamic_cast<ModelData*>(_pMeshData.get());
	if (!pModelData) return false;
	result = pModelData->Load(fileName, scale);

	_materials = pModelData->_modelMaterials;

	return result;
}

//-----------------------------------------------------------------------------
//!	�`��
//-----------------------------------------------------------------------------
void ModelMesh::Render()
{
	BindMaterial();
	if (!_pMeshData) return;
	_pMeshData->Render();
}

//-----------------------------------------------------------------------------
//!	�`����@�ݒ�
//!	@param	[in]	type	�`��^�C�v
//!	@retval	true	����
//!	@retval	false	���s
//-----------------------------------------------------------------------------
bool ModelMesh::SetRenderMode(BufferObject::PRIMITIVE_TYPE type)
{
	ModelData* pModelData = dynamic_cast<ModelData*>(_pMeshData.get());
	if (!pModelData) return false;

	for (auto* pObject : pModelData->_pObjects)
	{
		if (pObject->SetRenderMode(type) == false)
		{
			return false;
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
//!	���b�V���f�[�^�ݒ�
//!	@param	[in]	pModelData	���b�V���̃|�C���^
//-----------------------------------------------------------------------------
void ModelMesh::SetModelMeshData(ModelData* pModelData)
{
	SetMeshData(pModelData);
	_materials = pModelData->_modelMaterials;

}

//-----------------------------------------------------------------------------
//!	�`��
//!	@param	[in]	textureMode	�e�N�X�`���̗L������ default:true
//-----------------------------------------------------------------------------
void Model::Object::Render()
{
	if (_pVbo == nullptr || _pIbo == nullptr) return;
	if (_visible <= 0){ return; }

	_pVbo->Bind();
	_pIbo->Bind();
	_pVbo->CallInputLayout();
	ShaderManager* pShader = Manager::Shader();
	for(u32 i = 0; i < _clusters.size(); ++i)
	{
		const Cluster& c = _clusters[i];
		Texture* pTexture = c._pTexture;
		if(pTexture)
		{
			// �e�N�X�`������
			//pShader->Begin(1);

			Texture::BindTexture(pTexture, 0);
			pShader->SetSubroutine(SHADER_TYPE::FS, "Texture");		//	�Վ�����
			pShader->SetUniform(15, 0);

		}
		else
		{
//			pShader->SetSubroutine(SHADER_TYPE::FS, "Material");		//	�Վ�����

			// �e�N�X�`���Ȃ�
			//pShader->Begin(0);
			Texture::UnBindTexture(0);
		}


		_pVbo->Render(*_pIbo, c._indexStart, c._indexCount);
	}
	Texture::UnBindTexture(0);
	_pVbo->UnBind();
	_pIbo->UnBind();
}


//-----------------------------------------------------------------------------
//! �f�[�^�\�z
//-----------------------------------------------------------------------------
bool Model::Object::Build()
{
	if (_visible <= 0){ return false; }
	//---- �}�e���A���̕��ёւ�
	std::sort( _faces.begin(), _faces.end() );

	std::vector<Vertex> vertices;
	std::vector<u32> indices;

	s32 currentMaterialId = -1;
	s32 indexCount = 0;
	s32* pIndexCount = nullptr;
	Color vertexColor(255);
	u32	v0, v1, v2;
	u32	i0, i1, i2;
	Vector3 normal[3];
	Vector3 position[3];
	Vertex v[3];

	for (u32 i = 0; i < _normals.size(); ++i)
	{
		_normals[i] = Vector3::Normalize(_normals[i]);
	}

	for (u32 i = 0; i<_faces.size(); ++i)
	{
		const Model::Face& face = _faces[i];
		
		// �ގ��X�V
		const s32 materialId = face._material;
		if (materialId < 0)
		{
			// �e�N�X�`���Ȃ�
			currentMaterialId = materialId;
			auto clusterIndex = _clusters.size();
			_clusters.push_back(Cluster());
			Cluster& cluster = _clusters[clusterIndex];
			cluster._pTexture = nullptr;
			cluster._indexStart = indexCount;
			cluster._indexCount = 0;
			pIndexCount = &cluster._indexCount;

			vertexColor = Color(255);
		}
		else if (currentMaterialId != materialId)
		{
			// �e�N�X�`������
			currentMaterialId = materialId;
			MaterialData* pMaterial = _pModel->GetMaterial(materialId);

			auto clusterIndex = _clusters.size();
			_clusters.push_back(Cluster());
			Cluster& cluster = _clusters[clusterIndex];
			cluster._pTexture = pMaterial->_pTexture;
			cluster._indexStart = indexCount;
			cluster._indexCount = 0;
			pIndexCount = &cluster._indexCount;

			vertexColor = pMaterial->_color;
		}

//		normal = face._normal;
		// ���_�o�^

#if 0
		s32	triangleCount = face._vertexCount - 2;
		for (s32 triangle = 0; triangle<triangleCount; triangle++)
		{
			v0 = 0;
			v1 = triangle + 1;
			v2 = triangle + 2;

			i0 = vertices.size();
			i1 = i0 + 1;
			i2 = i0 + 2;

			position[0] = _vertices[ face._vertexIndex[v0] ];
			position[1] = _vertices[ face._vertexIndex[v1] ];
			position[2] = _vertices[ face._vertexIndex[v2] ];

			normal[0] = _normals[face._vertexIndex[v0]];
			normal[1] = _normals[face._vertexIndex[v1]];
			normal[2] = _normals[face._vertexIndex[v2]];


			v[0]._position = position[0];
			v[0]._normal   = normal[0];
			v[0]._color = vertexColor;
			v[0]._uv       = face._uv[v0];

			v[1]._position = position[1];
			v[1]._normal   = normal[1];
			v[1]._color = vertexColor;
			v[1]._uv       = face._uv[v1];

			v[2]._position = position[2];
			v[2]._normal   = normal[2];
			v[2]._color = vertexColor;
			v[2]._uv       = face._uv[v2];

			vertices.push_back(v[0]);
			vertices.push_back(v[1]);
			vertices.push_back(v[2]);

			indices.push_back(i0);
			indices.push_back(i1);
			indices.push_back(i2);

			indexCount   += 3;
			*pIndexCount += 3;
		}
#else
		s32	triangleCount = face._vertexCount - 2;
		for (s32 triangle = 0; triangle<triangleCount; triangle++)
		{
			v0 = 0;
			v1 = triangle + 1;
			v2 = triangle + 2;

			i0 = (u32)vertices.size();
			i1 = i0 + 1;
			i2 = i0 + 2;

			position[0] = _vertices[face._vertexIndex[v0]];
			position[1] = _vertices[face._vertexIndex[v1]];
			position[2] = _vertices[face._vertexIndex[v2]];

			normal[0] = _normals[face._vertexIndex[v0]];
			normal[1] = _normals[face._vertexIndex[v1]];
			normal[2] = _normals[face._vertexIndex[v2]];


			v[0]._position = position[0];
			v[0]._normal = normal[0];
			v[0]._color = vertexColor;
			v[0]._uv = face._uv[v0];

			v[1]._position = position[1];
			v[1]._normal = normal[1];
			v[1]._color = vertexColor;
			v[1]._uv = face._uv[v1];

			v[2]._position = position[2];
			v[2]._normal = normal[2];
			v[2]._color = vertexColor;
			v[2]._uv = face._uv[v2];

			vertices.push_back(v[0]);
			vertices.push_back(v[1]);
			vertices.push_back(v[2]);

			indices.push_back(i0);
			indices.push_back(i1);
			indices.push_back(i2);

			indexCount += 3;
			*pIndexCount += 3;
		}
#endif
	}


	//	�o�b�t�@���
	BufferObject::Element element[] =
	{
		{ 0, 3, BufferObject::ELEMENT_TYPE::F32, false, offsetof(Vertex, _position) },
		{ 1, 3, BufferObject::ELEMENT_TYPE::F32, false, offsetof(Vertex, _normal)   },
		{ 2, 4, BufferObject::ELEMENT_TYPE::U8 , true , offsetof(Vertex, _color)    },
		{ 3, 2, BufferObject::ELEMENT_TYPE::F32, false, offsetof(Vertex, _uv)       },
	};
	const u32 elementCount = ARRAY_COUNT(element);

	//	Vbo����
	BufferObject::Vbo* pVbo = new BufferObject::Vbo;
	if(pVbo)
	{
		BufferObject::Vbo::Desc vboDesc;
		if (vertices.empty())
		{
			SafeDelete(pVbo);
			return false;
		}
		vboDesc.SetVertexData(sizeof(Vertex), (u32)vertices.size(), &vertices[0]);
		vboDesc.SetElementList(&element[0], elementCount);
		if(pVbo->Create(vboDesc, BufferObject::PRIMITIVE_TYPE::TRIANGLES))
		{
			_pVbo = pVbo;
		}
		else
		{
			SafeDelete(pVbo);
			return false;
		}
	}

	//	Ibo����
		BufferObject::Ibo* pIbo = new BufferObject::Ibo;
	if (pIbo)
	{
		if (indices.empty())
		{
			SafeDelete(pVbo);
			return false;
		}

		BufferObject::Ibo::Desc iboDesc;
		iboDesc.SetIndexData(sizeof(u32), (u32)indices.size(), &indices[0]);
		if(pIbo->Create(iboDesc, BufferObject::INDEX_TYPE::TYPE_32))
		{
			_pIbo = pIbo;
		}
		else
		{
			SafeDelete(pIbo);
			return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
//!	���
//-----------------------------------------------------------------------------
void Model::Object::Clear()
{
	_vertices.clear();
	SafeDelete(_pVbo);
	SafeDelete(_pIbo);
}



//-----------------------------------------------------------------------------
//!	�`����@�ݒ�
//!	@param	[in]	type	�`��^�C�v
//!	@retval	true	����
//!	@retval	false	���s
//-----------------------------------------------------------------------------
bool Model::Object::SetRenderMode(BufferObject::PRIMITIVE_TYPE type)
{
	if (!_pVbo){ return false; }

	bool result = _pVbo->SetPrimitiveType(type);
	return result;	
}


void Model::Object::AddFace(const Model::Face& face)
{

	u32	v0, v1, v2;

	Vector3 position[3];
	Model::Face f = face;
	s32	triangleCount = f._vertexCount - 2;
	for (s32 triangle = 0; triangle < triangleCount; triangle++)
	{
		v0 = 0;
		v1 = 1;
		v2 = 2;

		position[0] = _vertices[f._vertexIndex[v0]];
		position[1] = _vertices[f._vertexIndex[v1]];
		position[2] = _vertices[f._vertexIndex[v2]];

		Vector3 normal = CreateTriangleNormal(position[0], position[1], position[2]);
		f._normal = normal;

		_normals[f._vertexIndex[v0]] += normal;
		_normals[f._vertexIndex[v1]] += normal;
		_normals[f._vertexIndex[v2]] += normal;


	}

	_faces.push_back(f);
}


//=============================================================================
//!	���f���I�u�W�F�N�g
//=============================================================================
//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
ModelObject::ModelObject()
{
}

//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
ModelObject::~ModelObject()
{
	AddTag("ModelObject");
}

//-----------------------------------------------------------------------------
//!	���f���ǂݍ���
//!	@param	[in]	fileName	�t�@�C����
//!	@param	[in]	scale		�g�嗦
//!	@retval	true	����
//!	@retval	false	���s
//-----------------------------------------------------------------------------
bool ModelObject::Load(const std::string& fileName, f32 scale)
{
	if(_pModelInstance){ return false; }
	auto* pModel = Manager::Model();
	ModelData* pMesh = nullptr;
	pMesh = pModel->LoadMesh(fileName, scale);
	if(!pMesh)
	{
		return false;
	}
	_pModelInstance = CreateMesh<ModelMesh>();
	_pModelInstance->SetMeshData(pMesh);
	_pModelInstance->_materials = pMesh->GetMaterials();
	return true;
}

//-----------------------------------------------------------------------------
//! �`��
//-----------------------------------------------------------------------------
void ModelObject::Render()
{
}

//-----------------------------------------------------------------------------
//!	���f���ݒ�
//!	@param	[in]	pModel	���b�V��
//-----------------------------------------------------------------------------
void ModelObject::SetModel(ModelMesh* pModel)
{
	_pModelInstance->SetMeshData(pModel);
}

//-----------------------------------------------------------------------------
//!	���f���ݒ�
//!	@param	[in]	model	���f���I�u�W�F�N�g
//-----------------------------------------------------------------------------
void ModelObject::SetModel(const ModelObject& model)
{
	_pModelInstance->SetMeshData(model._pModelInstance.get());
}


//-----------------------------------------------------------------------------
//!	�I��
//-----------------------------------------------------------------------------
void ModelManager::Exit()
{
	for (auto* pModel : _pModels)
	{
		SafeRelease(pModel);
	}
	_pModels.clear();
}

//-----------------------------------------------------------------------------
//!	���f������
//!	@param	[in]	fileName	�t�@�C����
//!	@return	���f���f�[�^�̃|�C���^
//!	@retval	nullptr	�擾���s
//-----------------------------------------------------------------------------
ModelData* ModelManager::Search(std::string fileName)
{
	if(fileName.empty()){ return nullptr; }
	for(auto* pModel : _pModels)
	{
		if(pModel->GetFileName() == fileName)
		{
			return pModel;
		}
	}
	return nullptr;
}

//-----------------------------------------------------------------------------
//!	���f���ǉ�
//!	@param	[in]	pModel	���f���f�[�^
//-----------------------------------------------------------------------------
void ModelManager::AddModel(ModelData* pModel)
{
	if(!pModel){ return; }
	pModel->AddRef();
	_pModels.push_back(pModel);
}

//-----------------------------------------------------------------------------
//!	���f���ǂݍ���
//!	@param	[in]	fileName	�t�@�C����
//!	@param	[in]	scale		�g�嗦
//!	@return	���f���f�[�^�̃|�C���^
//!	@retval	nullptr	�ǂݍ��ݎ��s
//-----------------------------------------------------------------------------
ModelData* ModelManager::LoadMesh(const std::string& fileName, f32 scale)
{
	ModelData* result = nullptr;
	result = Search(fileName);
	if(!result)
	{
		auto pMesh = CreateRefPointer<ModelData>();
		if(pMesh->Load(fileName, scale))
		{
			AddModel(pMesh.get());
			result = pMesh.get();
		}
	}

	return result;
}
