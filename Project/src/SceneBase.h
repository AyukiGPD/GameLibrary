//=============================================================================
//!
//!	@file	SceneBase.h
//!	@brief	シーンベース
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

// TODO キーやマウスなどのイベントクラスを入れる
//	シーン作成時はこのクラスを継承させる
//-------------------------------------------------------------------
//!	シーンベースクラス
//-------------------------------------------------------------------
class SceneBase
{
	friend class SceneManager;
public:
	//!	コンストラクタ
	SceneBase();
	//!	デストラクタ
	virtual ~SceneBase();
	//!	初期化
	virtual void Init(){}
	//!	更新
	virtual void Update(){}
	//!	描画
	virtual void Render(){}
	//!	終了
	virtual void Exit(){}

	//!	オブジェクト追加
	//!	@param	[in]	pObject	オブジェクトのポインタ
	void AddGameObject(GameObjectBase* pObject);

	//!	オブジェクト削除
	//!	@param	[in]	pObject	オブジェクトのポインタ
	//!	@return	解放後の参照数
	//!	@retval	0	削除済み
	s32 ReleaseGameObject(GameObjectBase* pObject);

	//!	カメラ設定
	//!	@param	[in]	pCamera	メインになるカメラ
	void SetSceneCamera(CameraObject* pCamera)
	{
		_pSceneCamera = pCamera;
	}

	//!	稼働中のカメラ取得
	//!	@return	メインのカメラ
	CameraObject* GetSceneCamera()
	{
		return _pSceneCamera;
	}

	//!	@tparam	T	検索するオブジェクト型
	//!	@return	オブジェクトの検索結果
	template <class T>
	std::vector<T*> GetObjects()
	{
		std::vector<T*> result;
		for(auto* pObject : _pObjectList)
		{
			T* p = dynamic_cast<T*>(pObject);
			if(!p)continue;
			result.push_back(p);
		}
		return result;
	}
protected:
	//!	オブジェクトの更新
	void ObjectUpdate();
	//!	オブジェクトの描画
	void ObjectRender();
protected:
	bool _isObjectUpdate;	//!< オブジェクトの自動更新のフラグ
private:
	std::list<GameObjectBase*> _pObjectList;	//!< シーン内で扱われるオブジェクト
	CameraObject* _pSceneCamera;				//!< シーン内で動かすカメラ
};

//-------------------------------------------------------------------
//!	ゲームオブジェクト作成
//!	@tparam	T		作成するゲームオブジェクト
//!	@tparam	Args	引数型
//!	@param	[in]	args	引数
//-------------------------------------------------------------------
template <typename T, typename... Args>
MY_INLINE
RefPointer<T>	CreateGameObject(Args&&... args)
{
	auto* pSceneManager = Manager::Scene();
	MY_ASSERT(pSceneManager != nullptr,"Not Created SceneManager");
	auto* pSceneObject = pSceneManager->GetNowScene();
	MY_ASSERT(pSceneObject != nullptr, "Not Created Scene");

	RefPointer<T> result = CreateRefPointer<T>(args...);
	pSceneObject->AddGameObject(result.get());
	return result;
}

//-------------------------------------------------------------------
//!	ゲームオブジェクト作成(初期化関数)
//!	@tparam	T		作成するゲームオブジェクト
//!	@tparam	Args	引数型
//!	@param	[in]	args	引数
//-------------------------------------------------------------------
template <typename T, typename... Args>
MY_INLINE
RefPointer<T>	CreateGameObjectInit(Args&&... args)
{
	auto* pSceneManager = Manager::Scene();
	MY_ASSERT(pSceneManager != nullptr, "Not Created SceneManager");
	auto* pSceneObject = pSceneManager->GetNowScene();
	MY_ASSERT(pSceneObject != nullptr, "Not Created Scene");

	RefPointer<T> result = CreateRefPointerInit<T>(args...);
	pSceneObject->AddGameObject(result.get());
	return result;
}

//-------------------------------------------------------------------
//!	ゲームオブジェクト削除 (RefPointer)
//!	@tparam	T			オブジェクトの型
//!	@param	[in,out]	pRefObject	オブジェクトのポインタ
//!	@param	[in]		forceDelete	強制削除 default:true
//!	@return	残りの参照数
//-------------------------------------------------------------------
template <typename T>
MY_INLINE
void DestroyObject(RefPointer<T>& pRefObject, bool forceDelete = true)
{
	if (!pRefObject)return;

	auto* pSceneObject = Manager::Scene()->GetNowScene();
	assert(pSceneObject != nullptr);

	T* pObject = pRefObject.get();

	GameObjectBase* pParent = pObject->GetParentObject();	//	親から解放

	s32 count = 100;

	if(pParent)
	{
		pParent->ReleaseChildObject(pObject);
	}

	count = pSceneObject->ReleaseGameObject(pObject);	//	シーンから削除

	if (count <= 0)return;

	count = pObject->Release();
	pRefObject.release();

	//	強制削除
	if(forceDelete)
	{
		if(count > 0)
		{
			SafeDelete(pObject);
			DEBUG_LOG("ForceDelete GameObject Count:" << count);
		}
	}
}

//-------------------------------------------------------------------
//!	ゲームオブジェクト削除 (const pointer)
//!	@tparam	T			オブジェクトの型
//!	@param	[in,out]	pRefObject	オブジェクトのポインタ
//!	@param	[in]		forceDelete	強制削除 default:true
//!	@return	残りの参照数
//-------------------------------------------------------------------
template <typename T>
MY_INLINE
void DestroyObject(T* pObject, bool forceDelete = true)
{
	if (!pObject)return;

	auto* pSceneObject = Manager::Scene()->GetNowScene();
	assert(pSceneObject != nullptr);


	GameObjectBase* pParent = pObject->GetParentObject();	//	親から解放

	if(pParent)
	{
		pParent->ReleaseChildObject(pObject);
	}

	s32 count = 100;
	count = pSceneObject->ReleaseGameObject(pObject);	//	シーンから削除

	if (count <= 0)return;

	count = pObject->Release();

	//	強制削除
	if(forceDelete)
	{
		if(count > 0)
		{
			SafeDelete(pObject);
			DEBUG_LOG("ForceDelete GameObject Count:" << count);
		}
	}
}
