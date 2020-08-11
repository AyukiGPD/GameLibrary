//=============================================================================
//!
//!	@file	Model.cpp
//!	@brief	モデルクラス
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"



//=============================================================================
//!	モデルオブジェクト
//=============================================================================
class Model::Object
{
public:
	//! コンストラクタ
	Object(ModelData* pModel)
	: _pModel(pModel)
	, _pVbo(nullptr)
	, _pIbo(nullptr)
	, _visible(0)
	{
	}
	//!	デストラクタ
	virtual ~Object()
	{
		Clear();
	}

	//! 描画
	void Render();

	//!	メッシュ先設定
	//!	@param	
	void SetModelMesh(ModelData* pModelMesh)
	{
		_pModel = pModelMesh;
	}


	//! データ構築
	bool Build();

	//!	解放
	void Clear();

	//!	描画方法設定
	//!	@param	[in]	type	描画タイプ
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool SetRenderMode(BufferObject::PRIMITIVE_TYPE type);

	//!	テクスチャごとにメッシュ割するためのデータ
	struct Cluster
	{
		s32			_indexStart;
		s32			_indexCount;
		Texture*	_pTexture;
	};

	//!	面追加
	//!	@param	[in]	face	面データ
	void AddFace(const Model::Face& face);

private:
	friend ModelData;

	s32 _visible;								//!< 表示フラグ

	std::vector<Vector3>		_vertices;		//!< 頂点配列
	std::vector<Vector3>		_normals;		//!< 法線配列
	std::vector<Model::Face>	_faces;			//!< ポリゴン配列

	ModelData*					_pModel;		//!< 属するモデルへのポインタ

	BufferObject::Vbo*			_pVbo;			//!< Vbo
	BufferObject::Ibo*			_pIbo;			//!< Ibo

	std::vector<Cluster>		_clusters;		//!< 分割データ

};

//=============================================================================
//!	ポリゴンデータ
//=============================================================================
class Model::Face
{
	static const s32 VERTEX_COUNT_MAX = 4;
public:
	//!	コンストラクタ
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
		// マテリアル番号で大小関係を決める
		return (_material < other.GetMaterialNumber());
	}

private:
	friend ModelData;
	friend Model::Object;

	s32									 _vertexCount;		//!< 頂点数  3:三角形 4:四角形
	std::array<	   s32,VERTEX_COUNT_MAX> _vertexIndex;		//!< 頂点番号
	std::array<Vector2,VERTEX_COUNT_MAX> _uv         ;		//!< テクスチャUV座標
	s32									 _material;			//!< マテリアル番号


	
	/*!
		@todo

		共有頂点の法線作成方法

		法線を頂点分作成
		中身はポインタ
		実体はObjectの頂点と一緒に持っている
		
		ポリゴン生成時に法線の計算をして
		そのインデックス番号の頂点に法線のデータを入れる
		そして入れたデータのポインタを取得する

		別ベクトルの法線が必要なときに
		Objectの法線をpush_backで複製して
		その複製したデータのポインタに上書き

		この処理はObject側で記述する
	*/
	Vector3 _normal;	//!< 面法線

	std::array<Vector3*, VERTEX_COUNT_MAX> _pNormals;	//!< 頂点法線

};

//=============================================================================
//!	モデルのデータを保持するオブジェクト
//=============================================================================
//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
ModelData::ModelData()
{
}

//-----------------------------------------------------------------------------
//!	デストラクタ
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
//!	描画
//-----------------------------------------------------------------------------
void ModelData::Render()
{
	for( auto* pObject : _pObjects)
	{
		pObject->Render();
	}
}

//-----------------------------------------------------------------------------
//!	読み込み
//!	@param	[in]	fileName	ファイル名
//!	@param	[in]	scale		スケール
//!	@retval	true	成功
//!	@retval	false	失敗
//-----------------------------------------------------------------------------
bool ModelData::Load(const std::string& fileName, f32 scale)
{
	bool result = false;
	size_t index = fileName.rfind(".", fileName.size() - 1);
	if (index != std::string::npos)
	{
		std::string ext = fileName.substr(index);	//	拡張子
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

//	mqoファイルフォーマット解説サイト
//http://metaseq.net/jp/format.html

//-----------------------------------------------------------------------------
//! MQOファイルの読み込み
//!	@param	[in]	fileName	MQOファイル名
//!	@param	[in]	scale		拡大率
//!	@retval	true	成功
//!	@retval	false	失敗
//-----------------------------------------------------------------------------
bool ModelData::LoadMQO(const std::string& fileName, f32 scale)
{
	char	drive[MAX_PATH];
	char	path[MAX_PATH];
	char	file[MAX_PATH];
	char	ext[MAX_PATH];		// ".mqo"

	_splitpath_s(fileName.c_str(), drive, path, file, ext);

	//------------------------------------------------------------------
	// ファイルを開く
	//------------------------------------------------------------------
	FILE*	fp = nullptr;
	fopen_s(&fp, fileName.c_str(), "rb");
	if (fp == nullptr)
	{
		DEBUG_LOG(L"ファイルのオープンに失敗しました.");
		return false;
	}

	enum class STATE
	{
		ROOT,			// 一番最上位の階層
		MATERIAL,		// マテリアル解析中
		OBJECT,			// Objectチャンク解析中
		OBJECT_VERTEX,	// Objectチャンク内の頂点座標
		OBJECT_FACE,	// Objectチャンク内のポリゴン情報
	};

	STATE	state = STATE::ROOT;
	char	str[1024];

	Model::Object*	pObject = nullptr;

	// １行読み込み
	while (fgets(&str[0], sizeof(str), fp) != 0)
	{
		switch (state)
		{
			//------------------------------------------------------------
		case STATE::ROOT:		// 最上位の階層
			//------------------------------------------------------------
			if (strncmp(str, "Material", 8) == 0)
			{
				state = STATE::MATERIAL;
				break;
			}


			if (strncmp(str, "Object", 6) == 0)
			{
				pObject = new Model::Object(this);
				_pObjects.push_back(pObject);		// 新規作成したオブジェクトをこの段階で配列に登録

				state = STATE::OBJECT;
				break;
			}
			break;
			//------------------------------------------------------------
		case STATE::MATERIAL:		// マテリアル
			//------------------------------------------------------------
		{
			//-----マテリアル終端チェック
			if (strncmp(str, "}", 1) == 0)
			{
				state = STATE::ROOT;	// 戻る
				break;
			}
			//---------マテリアル追加
			MaterialData	material;// 次の行が登録すればこの行がなくなる

			char* p = nullptr;

			//---- 材質カラー
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


			// テクスチャーの読み込み
			p = strstr(str, "tex(");
			if (p != nullptr)
			{
				// 1)「”」記号を検索
				p = strchr(p, '"');
				// 2) 1文字分進める( ファイル名の先頭 )
				p++;
				char* pFilePath = p;

				// 3) 末尾の　”　記号を検索
				p = strchr(p, '"');
				// 4) 文字終端コードを書き込んで分断する

				*p = '\0';

				// テクスチャを読み込み
				AssetTexture* pTexture = new AssetTexture();
				if (pTexture)
				{
					char	fullPath[MAX_PATH];
					_makepath_s(fullPath, drive, path, nullptr, nullptr);
					strcat_s(fullPath, pFilePath);	// ドライブパス + テクスチャファイル名
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
		case STATE::OBJECT:		// Objectチャンク解析中
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
		case STATE::OBJECT_VERTEX:	// Objectチャンク内の頂点座標
			//-------------------------------------------------------------------------
		{
			if (strstr(str, "}") != nullptr)
			{
				state = STATE::OBJECT;
				break;
			}

			//-----------------------------------------------------------
			// 頂点の読み込み
			//-----------------------------------------------------------
			f32	x, y, z;

			sscanf_s(str, " %f %f %f", &x, &y, &z);

			x *= scale;
			y *= scale;
			z *= scale;

			pObject->_vertices.push_back(Vector3(x, y, z));
			pObject->_normals.push_back(Vector3::ZERO);	//	頂点と同じ数法線の枠も作る
		}
		break;
		//-------------------------------------------------------------------------
		case STATE::OBJECT_FACE:		// Objectチャンク内のポリゴン情報
			//-------------------------------------------------------------------------
		{
			//---- 終端チェック
			if (strstr(str, "}") != nullptr)
			{
				state = STATE::OBJECT;
				break;
			}

			//---- ポリゴン読み込み
			Model::Face		face;

			// 三角形 or 四角形 を検出
			sscanf_s(str, " %d", &face._vertexCount);

			char*	p;
			// マテリアル番号
			p = strstr(str, "M(");
			if (p)
			{
				sscanf_s(p, "M(%d)", &face._material);
			}

			if (face._vertexCount == 3)
			{
				//---- 三角形の場合

				// 頂点インデックス


				p = strstr(str, "V(");
				sscanf_s(p, "V(%d %d %d)", &face._vertexIndex[2], &face._vertexIndex[1], &face._vertexIndex[0]);
				face._vertexIndex[3] = -1;		// 未使用

				// UV座標
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
				//---- 四角形の場合

				// 頂点インデックス
				p = strstr(str, "V(");
				sscanf_s(p, "V(%d %d %d %d)", &face._vertexIndex[3], &face._vertexIndex[2], &face._vertexIndex[1], &face._vertexIndex[0]);

				// UV座標
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

			//	配列へ登録
			pObject->AddFace(face);
		}
		break;

		}


	}


	//-------------------------------------------------------------
	// ファイルを閉じる
	//-------------------------------------------------------------
	fclose(fp);

	//-------------------------------------------------------------
	// マテリアル単位にポリゴンを並び替える
	//-------------------------------------------------------------
	for (auto* pModelObject : _pObjects)
	{
		if (pModelObject->Build() == false)
		{
			DEBUG_LOG(L"BuildError");
		}
	}


	//	読み込みでマテリアルが生成されなかった場合デフォルトのマテリアルを追加しておく
	if (_modelMaterials.size() == 0)
	{
		_modelMaterials.push_back(MaterialData::DEFAULT);
	}

	_fileName = fileName;

	//---- 成功
	return true;
}


//=============================================================================
//!	モデルメッシュクラス
//=============================================================================
//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
ModelMesh::ModelMesh()
{
	AddTag("ModelInstance");
}
//-----------------------------------------------------------------------------
//	デストラクタ
//-----------------------------------------------------------------------------
ModelMesh::~ModelMesh()
{
}

//-----------------------------------------------------------------------------
//!	モデル読み込み
//!	@param	[in]	fileName	ファイル名
//!	@param	[in]	scale		拡大率
//!	@retval	true	成功
//!	@retval	false	失敗
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
//!	描画
//-----------------------------------------------------------------------------
void ModelMesh::Render()
{
	BindMaterial();
	if (!_pMeshData) return;
	_pMeshData->Render();
}

//-----------------------------------------------------------------------------
//!	描画方法設定
//!	@param	[in]	type	描画タイプ
//!	@retval	true	成功
//!	@retval	false	失敗
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
//!	メッシュデータ設定
//!	@param	[in]	pModelData	メッシュのポインタ
//-----------------------------------------------------------------------------
void ModelMesh::SetModelMeshData(ModelData* pModelData)
{
	SetMeshData(pModelData);
	_materials = pModelData->_modelMaterials;

}

//-----------------------------------------------------------------------------
//!	描画
//!	@param	[in]	textureMode	テクスチャの有効無効 default:true
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
			// テクスチャあり
			//pShader->Begin(1);

			Texture::BindTexture(pTexture, 0);
			pShader->SetSubroutine(SHADER_TYPE::FS, "Texture");		//	臨時処理
			pShader->SetUniform(15, 0);

		}
		else
		{
//			pShader->SetSubroutine(SHADER_TYPE::FS, "Material");		//	臨時処理

			// テクスチャなし
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
//! データ構築
//-----------------------------------------------------------------------------
bool Model::Object::Build()
{
	if (_visible <= 0){ return false; }
	//---- マテリアルの並び替え
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
		
		// 材質更新
		const s32 materialId = face._material;
		if (materialId < 0)
		{
			// テクスチャなし
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
			// テクスチャあり
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
		// 頂点登録

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


	//	バッファ情報
	BufferObject::Element element[] =
	{
		{ 0, 3, BufferObject::ELEMENT_TYPE::F32, false, offsetof(Vertex, _position) },
		{ 1, 3, BufferObject::ELEMENT_TYPE::F32, false, offsetof(Vertex, _normal)   },
		{ 2, 4, BufferObject::ELEMENT_TYPE::U8 , true , offsetof(Vertex, _color)    },
		{ 3, 2, BufferObject::ELEMENT_TYPE::F32, false, offsetof(Vertex, _uv)       },
	};
	const u32 elementCount = ARRAY_COUNT(element);

	//	Vbo生成
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

	//	Ibo生成
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
//!	解放
//-----------------------------------------------------------------------------
void Model::Object::Clear()
{
	_vertices.clear();
	SafeDelete(_pVbo);
	SafeDelete(_pIbo);
}



//-----------------------------------------------------------------------------
//!	描画方法設定
//!	@param	[in]	type	描画タイプ
//!	@retval	true	成功
//!	@retval	false	失敗
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
//!	モデルオブジェクト
//=============================================================================
//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
ModelObject::ModelObject()
{
}

//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
ModelObject::~ModelObject()
{
	AddTag("ModelObject");
}

//-----------------------------------------------------------------------------
//!	モデル読み込み
//!	@param	[in]	fileName	ファイル名
//!	@param	[in]	scale		拡大率
//!	@retval	true	成功
//!	@retval	false	失敗
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
//! 描画
//-----------------------------------------------------------------------------
void ModelObject::Render()
{
}

//-----------------------------------------------------------------------------
//!	モデル設定
//!	@param	[in]	pModel	メッシュ
//-----------------------------------------------------------------------------
void ModelObject::SetModel(ModelMesh* pModel)
{
	_pModelInstance->SetMeshData(pModel);
}

//-----------------------------------------------------------------------------
//!	モデル設定
//!	@param	[in]	model	モデルオブジェクト
//-----------------------------------------------------------------------------
void ModelObject::SetModel(const ModelObject& model)
{
	_pModelInstance->SetMeshData(model._pModelInstance.get());
}


//-----------------------------------------------------------------------------
//!	終了
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
//!	モデル検索
//!	@param	[in]	fileName	ファイル名
//!	@return	モデルデータのポインタ
//!	@retval	nullptr	取得失敗
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
//!	モデル追加
//!	@param	[in]	pModel	モデルデータ
//-----------------------------------------------------------------------------
void ModelManager::AddModel(ModelData* pModel)
{
	if(!pModel){ return; }
	pModel->AddRef();
	_pModels.push_back(pModel);
}

//-----------------------------------------------------------------------------
//!	モデル読み込み
//!	@param	[in]	fileName	ファイル名
//!	@param	[in]	scale		拡大率
//!	@return	モデルデータのポインタ
//!	@retval	nullptr	読み込み失敗
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
