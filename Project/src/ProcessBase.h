//=============================================================================
//!
//!	@file	ProcessBase.h
//!	@brief	プロセス基底クラス
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//!	@class GameObjectBase BaseObject.h "BaseObject.h"
class GameObjectBase;

//-------------------------------------------------------------------
//! @brief  プロセス基底クラス
//-------------------------------------------------------------------
class ProcessBase : public BasicBase
{
public:
	//! @brief  コンストラクタ
	ProcessBase();

	//! @brief  コンストラクタ
	//!	@param	[in]	pParent	親オブジェクト
	ProcessBase(GameObjectBase* pParent);

	//! @brief  デストラクタ
	virtual ~ProcessBase();

	//! @brief  親オブジェクト設定
	//!	@param	[in]	pParent	親オブジェクト
	void SetParent(GameObjectBase* pParent);

	//! @brief  親オブジェクト設定
	//!	@param	[in]	pParent	親オブジェクト
	GameObjectBase* GetParent() { return _pParent; }

	//! @brief  更新
	virtual void Update() = 0;

protected:
	GameObjectBase* _pParent;	//!< 親オブジェクト
};

