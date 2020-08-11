//=============================================================================
//!
//!	@file	WorkSpace.h
//!	@brief	ワークスペース
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

// TODO 別ファイルに移動
// TODO 本実装
//!	マネージャークラス系の生成用クラス
class ManagerBundle
{
public:
	//!	コンストラクタ
	ManagerBundle();
	//!	デストラクタ
	virtual ~ManagerBundle();

	//!	マネージャー追加
	//!	@tparam	T	マネージャー型
	template<typename T>
	static void AddManager()
	{
		_pManagers.push_back(Singleton<T>::GetInstance());
	}

	//void Update();
	//void Render();

	//!	終了
	void Exit();

private:
	static std::list<ManagerBase*> _pManagers;
};

//!	マネージャー追加
//!	@tparam	T	マネージャー型
template<typename T>
void AddManager()
{
	ManagerBundle::AddManager<T>();
}

//!	メインクラス
class SystemWork
{
public:
	//!	コンストラクタ
	SystemWork(){}
	//!	デストラクタ
	virtual ~SystemWork(){}

	//!	初期化
	bool Init();
	//!	更新
	void Update();
	//!	描画
	void Render();
	//!	終了
	void Exit();

private:
	ManagerBundle* _pManager;
};


