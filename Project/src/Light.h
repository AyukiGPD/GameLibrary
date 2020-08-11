//=============================================================================
//!
//!	@file	Light.h
//!	@brief	ライトクラス
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

/*!
	@todo
	ライトは種類によってクラスを分ける
	シェーダーには全部ではなく種類ごとに渡す
*/


//!	ライトモード
enum class LIGHT_MODE : s32
{
	INVALID		= -1,		//!< 無効
	AMBIENT		=  0,		//!< 環境光
	DIRECTIONAL	=  1,		//!< 平行光源
	POINT		=  2,		//!< 点光源
	SPOT		=  3,		//!< スポットライト
};

//-----------------------------------------------------------------------------
//!	ライトオブジェクト
//-----------------------------------------------------------------------------
class LightObject : public GameObjectBase
{
public:
	struct Desc
	{
		//!	コンストラクタ
		Desc()
		: _position	(0,0,0)
		, _color	(0)
		, _lightMode(LIGHT_MODE::INVALID)
		, _power	(0.f)
		{		}
		//!	コンストラクタ
		Desc(Vector3 position, Color color, LIGHT_MODE lightMode, f32 power = 1.0f)
		: _position	(position)
		, _color	(color)
		, _lightMode(lightMode)
		, _power	(power)
		{		}

		Vector3		_position;		//!< 座標
		Color		_color;			//!< 色
		LIGHT_MODE	_lightMode;		//!< ライトモード
		f32			_power;			//!< ライトの強さ
	};

public:
	//!	コンストラクタ
	LightObject();
	//!	コンストラクタ
	LightObject(const Desc& desc);
	//!	デストラクタ
	virtual ~LightObject();

	void SetLight(const Desc& desc);
	//!	設定
	//!	@param	[in]	position	座標,ベクトル
	//!	@param	[in]	color		色
	//!	@param	[in]	mode		モード
	//!	@param	[in]	power		強さ	default 1.0f
	void SetLight(const Vector3& position, const Color& color, LIGHT_MODE mode, f32 power = 1.0f);
	//!	色設定
	//!	@param	[in]	color	色
	void SetColor(const Color& color);
	//!	ライトモード設定
	//!	@param	[in]	mode	モード
	void SetMode(LIGHT_MODE mode);
	//!	ライトの強さ設定
	//!	@param	[in]	power	強さ
	void SetPower(f32 power);


	//!	@return 色取得
	const Color& GetColor()const { return _color; }
	//!	@return ライト強度
	f32 GetPower()const { return _power; }
	//! @return ライトモード
	LIGHT_MODE GetLightMode()const { return _lightMode; }


private:
	Color				_color;			//!< 色
	f32					_power;			//!< ライトの強さ
	LIGHT_MODE			_lightMode;		//!< ライトモード

};


//-----------------------------------------------------------------------------
//!	ライト管理クラス
//-----------------------------------------------------------------------------
class LightManager : public ManagerBase
{
public:
	//!	コンストラクタ
	LightManager();
	//!	デストラクタ
	virtual ~LightManager();

	//!	解放
	//!	@param	[in,out]	pLight	解放するカメラのポインタ
	void Add(LightObject* pLight);
	//!	解放
	//!	@param	[in,out]	pLight	解放するカメラのポインタ
	void Release(LightObject* pLight);

	//!	ライトリスト取得
	//!	@return	ライトリスト
	std::list<LightObject*>& GetLightList();


private:
	std::list<LightObject*> _pLightList;
};
MANAGER_INTERFACE(LightManager, Light);

