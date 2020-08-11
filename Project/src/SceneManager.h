//=============================================================================
//!
//!	@file	SceneManager.h
//!	@brief	シーンマネージャー
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//-------------------------------------------------------------------
//!	シーン管理クラス
//-------------------------------------------------------------------
class SceneManager : public ManagerBase
{
public:
	//!	コンストラクタ
	SceneManager();
	//!	デストラクタ
	virtual ~SceneManager();

	//!	初期化
	void Init();
	//!	更新
	void Update();
	//!	描画
	void Render();
	//!	終了
	void Exit();

	//!	シーン切り替え関数
	//!	@tparam	T	次のシーンクラス
	template<typename T>
	void SceneJump()
	{
		SafeDelete(_pNextScene);		//	念のために中身をnullptr初期化をする
		_pNextScene = new T();			//	新しいシーンを作成
	}

	//!	現在のシーンを取得
	//!	@return	シーンオブジェクトのポインタ
	 SceneBase* GetNowScene()
	{
		return _pScene;
	}

private:
	SceneBase*	_pScene;		//!< メインで動くシーンクラス
	SceneBase*	_pNextScene;	//!< 次切り替わるシーン

};

MANAGER_INTERFACE(SceneManager, Scene);


