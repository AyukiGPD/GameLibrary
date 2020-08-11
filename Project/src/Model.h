//=============================================================================
//!
//!	@file	Model.ｈ
//!	@brief	モデルクラス
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

namespace Model
{

//	実装はModel.cpp
class Object;			// モデルオブジェクト
class Face;				// ポリゴン
}

//-------------------------------------------------------------------
//!	モデルのデータを保持するオブジェクト
//!	現在対応しているのはmqoのみ
//-------------------------------------------------------------------
class ModelData : public MeshData
{
public:
	friend class ModelMesh;
	//!	コンストラクタ
	ModelData();
	//!	デストラクタ
	virtual ~ModelData();

	//!	描画
	void Render();

	//!	読み込み
	//!	@param	[in]	fileName	ファイル名
	//!	@param	[in]	scale		スケール
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool Load(const std::string& fileName, f32 scale);

	//!	マテリアル取得
	//!	@param	[in]	index	番号
	//!	@return	マテリアルへのポインタ
	//!	@retval	nullptr	取得失敗
	MaterialData* GetMaterial(u32 index)
	{
		if (index >= _modelMaterials.size())return nullptr;
		return &_modelMaterials[index];
	}

	//!	マテリアル全取得
	//!	@return	マテリアルのvector配列
	std::vector<MaterialData> GetMaterials()const
	{
		return _modelMaterials;
	}

	//!	ファイル名取得
	//!	@return	ファイル名
	std::string GetFileName()const
	{
		return _fileName;
	}

private:
	//! MQOファイルの読み込み
	//!	@param	[in]	fileName	MQOファイル名
	//!	@param	[in]	scale		拡大率
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool	LoadMQO(const std::string& fileName, f32 scale = 0.1f);

private:
	//! モデルのマテリアル このデータは持ってるだけで使うには取り出さなければいけない
	std::vector<MaterialData> _modelMaterials;

	std::vector<Model::Object*>		_pObjects;		//!< モデルオブジェクト配列
	std::string						_fileName;		//!< ファイル名
};
//-------------------------------------------------------------------
//!	モデルメッシュクラス
//-------------------------------------------------------------------
class ModelMesh : public MeshBase
{
	friend class ModelData;
	friend class ModelObject;
public:
	//! コンストラクタ
	ModelMesh();

	//! デストラクタ
	virtual ~ModelMesh();

	//!	モデル読み込み
	//!	@param	[in]	fileName	ファイル名
	//!	@param	[in]	scale		拡大率
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool Load(const std::string& fileName, f32 scale = 0.1f);

	//! 描画
	void Render();


	//!	描画方法設定
	//!	@param	[in]	type	描画タイプ
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool SetRenderMode(BufferObject::PRIMITIVE_TYPE type);


	//!	メッシュデータ設定
	//!	@param	[in]	pModelData	メッシュのポインタ
	void SetModelMeshData(ModelData* pModelData);

	//! オブジェクトを取得
	//! @param	[in]	index	オブジェクト番号(0~)
	//!	@return	オブジェクトのポインタ
	//!	@retval	nullptr	取得失敗
	Model::Object* GetObjectData(u32 index) const
	{
		ModelData* pModel = dynamic_cast<ModelData*>(_pMeshData.get());
		if(!pModel)return nullptr;
		if( index <                  0 ) return nullptr;
		if(index >= pModel->_pObjects.size()) return nullptr;

		return pModel->_pObjects[index];
	}

	//	オブジェクトの数を取得
	//!	@return	オブジェクトの数
	u32 GetObjectSize() const
	{
		ModelData* pModel = dynamic_cast<ModelData*>(_pMeshData.get());
		return pModel->_pObjects.size();
	}

	//!	モデル保持
	//!	@retval	false	保持していない
	bool IsEmpty()const
	{
		ModelData* pModel = dynamic_cast<ModelData*>(_pMeshData.get());
		return pModel->_pObjects.empty();
	}

	//!	ファイル名取得
	//!	@return	ファイル名
	std::string GetFileName()const
	{
		ModelData* pModel = dynamic_cast<ModelData*>(_pMeshData.get());
		return pModel->_fileName;
	}

private:

};

//-------------------------------------------------------------------
//!	モデルオブジェクト
//-------------------------------------------------------------------
class ModelObject : public GameObjectBase
{
public:
	//!	コンストラクタ
	ModelObject();
	//!	デストラクタ
	virtual ~ModelObject();

	//!	モデル読み込み
	//!	@param	[in]	fileName	ファイル名
	//!	@param	[in]	scale		拡大率
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool Load(const std::string& fileName, f32 scale = 0.1f);

	//! 描画
	void Render();

	//!	モデル設定
	//!	@param	[in]	pModel	メッシュ
	void SetModel(ModelMesh* pModel);

	//!	モデル設定
	//!	@param	[in]	model	モデルオブジェクト
	void SetModel(const ModelObject& model);

	//!	描画方法設定
	//!	@param	[in]	type	描画タイプ
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool SetRenderMode(BufferObject::PRIMITIVE_TYPE type)
	{
		return _pModelInstance->SetRenderMode(type);
	}

	//! マテリアルを取得
	//! @param	[in]	マテリアル番号(0~)
	//!	@return	マテリアルのポインタ	失敗時はnullptr
	MaterialData* GetMaterial(u32 index)
	{
		if(_pModelInstance)
		{
			return _pModelInstance->GetMaterial(index);
		}
		return nullptr;
	}

	//! オブジェクトを取得
	//! @param	[in]	index	オブジェクト番号(0~)
	//!	@return	オブジェクトのポインタ	失敗時はnullptr
	Model::Object* GetObjectData(u32 index) const
	{
		if(_pModelInstance)
		{
			_pModelInstance->GetObjectData(index);
		}
		return nullptr;
	}

	//!	マテリアルの数を取得
	//!	@return	マテリアルの数
	u32 GetMaterialSize() const
	{
		return _pModelInstance->GetMaterialSize();
	}
	//	オブジェクトの数を取得
	//!	@return	オブジェクトの数
	u32 GetObjectSize() const
	{
		return _pModelInstance->GetObjectSize();
	}

	//!	モデル保持
	//!	@retval	false	保持していない
	bool IsEmpty()const
	{
		if(_pModelInstance)
		{
			return _pModelInstance->IsEmpty();
		}
		return false;
	}

	//!	イコール演算子
	ModelObject& operator = (const ModelObject& p)
	{
		_pModelInstance.reset(p._pModelInstance.get());
		_pModelInstance->AddRef();
		return *this;
	}

private:
	RefPointer<ModelMesh> _pModelInstance;	//!< モデルメッシュデータ
};

//-------------------------------------------------------------------
//!	モデル管理クラス
//-------------------------------------------------------------------
class ModelManager : public ManagerBase
{
public:
	//!	終了
	void Exit();

	//!	モデル検索
	//!	@param	[in]	fileName	ファイル名
	//!	@return	モデルデータのポインタ
	//!	@retval	nullptr	取得失敗
	ModelData* Search(std::string fileName);

	//!	モデル追加
	//!	@param	[in]	pModel	モデルデータ
	void AddModel(ModelData* pModel);


	//!	モデル読み込み
	//!	@param	[in]	fileName	ファイル名
	//!	@param	[in]	scale		拡大率
	//!	@return	モデルデータのポインタ
	//!	@retval	nullptr	読み込み失敗
	ModelData*  LoadMesh(const std::string& fileName, f32 scale = 0.1f);

private:
	std::list<ModelData*> _pModels;		//!< モデルデータ
};


MANAGER_INTERFACE(ModelManager, Model);

