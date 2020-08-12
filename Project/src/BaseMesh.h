//=============================================================================
//!
//!	@file	BaseMesh.h
//!	@brief	メッシュ基底クラス
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//-------------------------------------------------------------------
//! @brief  マテリアルカラーモード
//-------------------------------------------------------------------
enum class MATERIAL_COLOR
{
	VERTEX = 0,
	MATERIAL = 1,
	TEXTURE = 2
};

//-------------------------------------------------------------------
//! @brief  マテリアルデータ
//-------------------------------------------------------------------
struct MaterialData
{
	//! @brief  コンストラクタ
	MaterialData()
	: _colorMode(MATERIAL_COLOR::MATERIAL)
	, _color	(128)
	, _roughness(0.0f)
	, _metalness(0.0f)
	, _emissiveColor(Vector3::ZERO)
	, _pTexture(nullptr)
	{
	}

	//! @brief  コンストラクタ
	//!	@param	[in]	colorMode	カラーモード
	//!	@param	[in]	color		マテリアルカラー
	//!	@param	[in]	roughness	ラフ
	//!	@param	[in]	metalness	メタル
	//!	@param	[in]	pTexture	マテリアルテクスチャ	default:nullptr
	MaterialData(MATERIAL_COLOR colorMode, const Color& color, f32 roughness, f32 metalness,const Vector3& emissiveColor = Vector3::ZERO, Texture* pTexture = nullptr)
	: _colorMode	(colorMode)
	, _color		(color)
	, _roughness	(roughness)
	, _metalness	(metalness)
	, _emissiveColor(emissiveColor)
	, _pTexture		(pTexture)
	{
	}

	//! @brief  デストラクタ
	virtual ~MaterialData()
	{
	}

	//! @brief  デフォルトマテリアル情報
	static const MaterialData DEFAULT;

	MATERIAL_COLOR	_colorMode;		//!< カラーモード
	Color			_color;			//!< マテリアルカラー
	f32				_roughness;		//!< ラフ
	f32				_metalness;		//!< メタル
	Vector3			_emissiveColor;	//!< エミッシブカラー
	Texture*		_pTexture;		//!< マテリアルテクスチャ
};

//!	@class GameObjectBase BaseObject.h "BaseObject.h"
class GameObjectBase;

//-------------------------------------------------------------------
//! @brief  メッシュ基底クラス
//-------------------------------------------------------------------
class MeshBase : public BasicBase
{
public:
	//! @brief  コンストラクタ
	MeshBase();

	//! @brief  コンストラクタ
	//!	@param	[in]	pParent	親オブジェクト
	MeshBase(GameObjectBase* pParent);

	//! @brief  デストラクタ
	virtual ~MeshBase();

	//! @brief  親オブジェクト設定
	//!	@param	[in]	pParent	親オブジェクト
	void SetParent(GameObjectBase* pParent);

	//! @brief  描画
	virtual void Render() = 0;

	//! @brief  マテリアル取得
	//!	@param	[in]	index	マテリアル番号
	MaterialData* GetMaterial(u32 index);

	//! @brief  マテリアルの数を取得
	//!	@return	マテリアルの数
	size_t GetMaterialSize() const;

	//! @brief  マテリアル追加
	//!	@param	[in]	material	マテリアル
	size_t AddMateiral(const MaterialData& material);

	//! @brief  マテリアル設定(書き換え)
	//!	@param	[in]	index		マテリアル番号
	//!	@param	[in]	material	マテリアル
	void SetMaterial(u32 index, const MaterialData& material);

	//! @brief  マテリアルの適応
	//!	@param	[in]	index	マテリアル番号 default:0
	void BindMaterial(u32 index = 0);

	//! @brief  メッシュデータ設定
	//!	@param	[in]	pMeshData	メッシュデータ
	void SetMeshData(MeshData* pMeshData);

	//! @brief  メッシュデータ設定
	//!	@param	[in]	pMesh	メッシュ
	void SetMeshData(MeshBase* pMesh);

protected:
	std::vector<MaterialData> _materials;		//!< マテリアル
	RefPointer<MeshData> _pMeshData;			//!< メッシュデータ
	GameObjectBase* _pParent;					//!< 親オブジェクト
};
