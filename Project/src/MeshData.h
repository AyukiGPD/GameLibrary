//=============================================================================
//!
//!	@file	MeshData.h
//!	@brief	メッシュ情報基底クラス
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//-------------------------------------------------------------------
//! @brief  メッシュデータオブジェクト
//-------------------------------------------------------------------
class MeshData : public BasicBase
{
public:
	//! @brief  コンストラクタ
	MeshData()
	{
	}

	//! @brief  デストラクタ
	virtual ~MeshData() {}

	//! @brief  描画
	virtual void Render() = 0;

protected:

};
