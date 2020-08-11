//=============================================================================
//!
//!	@file	SceneBase.cpp
//!	@brief	シーンベース
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
SceneBase::SceneBase()
: _pSceneCamera(nullptr)
, _isObjectUpdate(true)
{
}

//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
SceneBase::~SceneBase()
{
	auto itr = _pObjectList.begin();
	while(itr != _pObjectList.end())
	{
		GameObjectBase* p = (*itr);
		auto erase = itr;
		++itr;
		_pObjectList.erase(erase);

		SafeRelease(p);
	}
}

//-----------------------------------------------------------------------------
//!	オブジェクト追加
//!	@param	[in]	pObject	オブジェクトのポインタ
//-----------------------------------------------------------------------------
void SceneBase::AddGameObject(GameObjectBase* pObject)
{
	if(!pObject){ return; }
	pObject->AddRef();
	_pObjectList.push_back(pObject);
}

//-----------------------------------------------------------------------------
//!	オブジェクト削除
//!	@param	[in]	pObject	オブジェクトのポインタ
//!	@return	解放後の参照数
//!	@retval	0	削除済み
//-----------------------------------------------------------------------------
s32 SceneBase::ReleaseGameObject(GameObjectBase* pObject)
{
	if(!pObject){ return 0; }
	s32 count = 0;
	auto itr = _pObjectList.begin();
	while(itr != _pObjectList.end())
	{
		if((*itr) == pObject)
		{
			count = SafeRelease(*itr);	//	リスト内のオブジェクトをNULLにする
			break;
		}
		++itr;
	}
	return count;
}

//-----------------------------------------------------------------------------
//!	オブジェクトの更新
//-----------------------------------------------------------------------------
void SceneBase::ObjectUpdate()
{
	if (!_isObjectUpdate)return;

	std::wstring str;
	CREATE_WSTRING("ObjectSize : " << _pObjectList.size(), str);
	Manager::Debug()->SetDebugString(10, str);

	for (auto& pObject : _pObjectList)
	{
		if(!pObject || !pObject->IsEnable() || pObject->GetParentObject())continue;

		pObject->Update();
		if(!pObject)continue;	//	消えてたら次のオブジェクトへ
		pObject->ProcessUpdate();
		if (!pObject)continue;	//	消えてたら次のオブジェクトへ
		pObject->ChildrenUpdate();
	}
	//	消えたオブジェクトをリストから消す
	auto itr = _pObjectList.begin();
	while(itr != _pObjectList.end())
	{
		if(!(*itr))
		{
			itr = _pObjectList.erase(itr);
			continue;
		}
		++itr;
	}

}

//-----------------------------------------------------------------------------
//!	オブジェクトの描画
//-----------------------------------------------------------------------------
void SceneBase::ObjectRender()
{
	for (auto& pObject : _pObjectList)
	{
		if(!pObject || !pObject->IsEnable() || pObject->GetParentObject())continue;
		pObject->BindWorldMatrix();
		pObject->Render();
		pObject->MeshRender();
		pObject->ChildrenRender();
	}
	Manager::Model()->Render();
}



