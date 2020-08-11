//=============================================================================
//!
//!	@file	DataLoader.h
//!	@brief	データ読み込み管理クラス	未実装
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once
class DataLoader : public ManagerBase
{
public:
	DataLoader();
	virtual ~DataLoader();

	//!	ファイル読み込み
	//!	@param	[in]	fileName	ファイル名
	//!	return	読み込んだファイル	nullptrなら失敗
	void* LoadFile(const std::string& fileName);
};

MANAGER_INTERFACE(DataLoader, Loader);
