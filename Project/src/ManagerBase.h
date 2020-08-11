//=============================================================================
//!
//!	@file	ManagerBase.h
//!	@brief	管理クラス基底
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

class ManagerBase
{
public:
	//!	コンストラクタ
	ManagerBase(){};
	//!	デストラクタ
	virtual ~ManagerBase(){};

	//!	初期化
	virtual void Init(){}
	//!	更新
	virtual void Update(){}
	//!	描画
	virtual void Render(){}
	//!	終了
	virtual void Exit(){}

private:

};


//!	シングルトン取得関数生成
#define MANAGER_INTERFACE(CLASS_, FUNCTION_NAME_)	\
namespace Manager									\
{													\
MY_FORCEINLINE CLASS_*		FUNCTION_NAME_()		\
{													\
	return Singleton<CLASS_>::GetInstance();		\
}													\
}													\

//!	マネージャー呼び出しマクロ
#define CALL_MANAGER(MANAGER_FUNC_,POINER_)		\
auto* POINER_ = MANAGER_FUNC_;					\
MY_ASSERT(POINER_ != nullptr,"Manager Empty")	\


