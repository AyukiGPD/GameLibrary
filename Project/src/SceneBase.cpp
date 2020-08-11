//=============================================================================
//!
//!	@file	SceneBase.cpp
//!	@brief	�V�[���x�[�X
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
SceneBase::SceneBase()
: _pSceneCamera(nullptr)
, _isObjectUpdate(true)
{
}

//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
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
//!	�I�u�W�F�N�g�ǉ�
//!	@param	[in]	pObject	�I�u�W�F�N�g�̃|�C���^
//-----------------------------------------------------------------------------
void SceneBase::AddGameObject(GameObjectBase* pObject)
{
	if(!pObject){ return; }
	pObject->AddRef();
	_pObjectList.push_back(pObject);
}

//-----------------------------------------------------------------------------
//!	�I�u�W�F�N�g�폜
//!	@param	[in]	pObject	�I�u�W�F�N�g�̃|�C���^
//!	@return	�����̎Q�Ɛ�
//!	@retval	0	�폜�ς�
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
			count = SafeRelease(*itr);	//	���X�g���̃I�u�W�F�N�g��NULL�ɂ���
			break;
		}
		++itr;
	}
	return count;
}

//-----------------------------------------------------------------------------
//!	�I�u�W�F�N�g�̍X�V
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
		if(!pObject)continue;	//	�����Ă��玟�̃I�u�W�F�N�g��
		pObject->ProcessUpdate();
		if (!pObject)continue;	//	�����Ă��玟�̃I�u�W�F�N�g��
		pObject->ChildrenUpdate();
	}
	//	�������I�u�W�F�N�g�����X�g�������
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
//!	�I�u�W�F�N�g�̕`��
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



