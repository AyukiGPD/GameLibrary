//=============================================================================
//!
//!	@file	GameObjectBase.cpp
//!	@brief	オブジェクト基底クラス
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
GameObjectBase::GameObjectBase()
: _pParentObject	(nullptr)
, _localPosition	(Vector3::ZERO)
, _localRotate		(Quaternion::IDENTITY)
, _localScale		(Vector3::ONE)
, _localMatrix		(Matrix::IDENTITY)
, _enable			(true)
{
	Init();
}
//-----------------------------------------------------------------------------
//!	コンストラクタ
//!	@param	[in]	position	座標
//!	@param	[in]	rotate		回転
//!	@param	[in]	Scale		サイズ	default:Vector3::ONE
//-----------------------------------------------------------------------------
GameObjectBase::GameObjectBase(const Vector3& position, const Quaternion& rotate, const Vector3& scale, bool enable)
: _pParentObject	(nullptr)
, _localPosition	(position)
, _localRotate		(rotate)
, _localScale		(scale)
, _localMatrix		(Matrix::IDENTITY)
, _enable			(enable)
{
	MatrixUpdate();
	Init();
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
GameObjectBase::~GameObjectBase()
{
	Clear();

	for(auto* pProcess : _pProcesss)
	{
		SafeRelease(pProcess);
	}
	for(auto* pMesh : _pMeshs)
	{
		SafeRelease(pMesh);
	}
	ReleaseChildObjects();
}

//-----------------------------------------------------------------------------
//!	親設定
//!@param	[in]	pParentObject	親のポインタ
//-----------------------------------------------------------------------------
void GameObjectBase::SetParentObject(GameObjectBase* pParentObject)
{
	if (!pParentObject)
	{
		DEBUG_LOG("New Parent Null");
		return;
	}
	ReleaseParent();
	_pParentObject = pParentObject;
	SetLocalMatrix(GetWorldMatrix() * Matrix::Invert(_pParentObject->GetWorldMatrix()));
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	親取得
//!	@return	親オブジェクトポインタ
//!	@retval	nullptr	親なし
//-----------------------------------------------------------------------------
GameObjectBase* GameObjectBase::GetParentObject()const
{
	return _pParentObject;
}


//-----------------------------------------------------------------------------
//!	子追加
//!@param	[in]	pChildObject	子のポインタ
//-----------------------------------------------------------------------------
void GameObjectBase::AddChildObject(GameObjectBase* pChildObject)
{
	if(pChildObject)
	{
		//	追加時のマトリックス変更にバグ
		pChildObject->AddRef();
		_pChildrenObjects.push_back(pChildObject);
		pChildObject->SetParentObject(this);
	}
}

//-----------------------------------------------------------------------------
//!	子削除
//!@param	[in]	pChildObject	子のポインタ
//!	@return	解放後の参照数 0なら削除済み
//-----------------------------------------------------------------------------
s32 GameObjectBase::ReleaseChildObject(GameObjectBase* pChildObject)
{
	if(!pChildObject)	return 0;

	s32 count = 0;
	auto itr = _pChildrenObjects.begin();
	while(itr != _pChildrenObjects.end())
	{
		GameObjectBase* p = (*itr);
		if(p == pChildObject)
		{
			count = SafeRelease(*itr);	//	リスト内のオブジェクトをNULLにする
			break;
		}
		++itr;
	}
	return count;
}

//-----------------------------------------------------------------------------
//!	親からの解放
//-----------------------------------------------------------------------------
void GameObjectBase::ReleaseParent()
{
	if (!_pParentObject) return;
	Matrix matWorld = GetWorldMatrix();
	_pParentObject->ReleaseChildObject(this);
	_pParentObject = nullptr;
	SetWorldMatrix(matWorld);
}

//-----------------------------------------------------------------------------
//!	子オブジェクトタグ検索
//!	@param	[in]	tag	オブジェクトのタグ
//!	@return	オブジェクトのポインタ
//!	@retval	nullptr	データなし
//-----------------------------------------------------------------------------
GameObjectBase* GameObjectBase::GetChildObject(const std::string& tag)const
{
	for(auto* pChild : _pChildrenObjects)
	{
		if(pChild->SearchTag(tag))
		{
			return pChild;
		}
	}
	return nullptr;
}

//-----------------------------------------------------------------------------
//!	子オブジェクトの解放
//-----------------------------------------------------------------------------
void GameObjectBase::ReleaseChildObjects()
{
	for (auto* pChild : _pChildrenObjects)
	{
		DestroyObject(pChild, false);
	}
}

//-----------------------------------------------------------------------------
//!	ワールド行列設定
//!	@param	[in]	matrix		行列
//-----------------------------------------------------------------------------
void GameObjectBase::SetWorldMatrix(const Matrix& matrix)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix newMatrix = matrix * Matrix::Invert(parentMatrix);

	_localMatrix	= newMatrix;
	_localPosition	= newMatrix.GetPosition();
	_localRotate	= newMatrix.GetRotateQuaternion();
	_localScale		= newMatrix.GetScale();
}

//-----------------------------------------------------------------------------
//!	ワールド回転
//!	@param	[in]	rotate		回転
//-----------------------------------------------------------------------------
void GameObjectBase::SetWorldPosition(const Vector3& position)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixPosition = Matrix::Translate(position) * Matrix::Invert(parentMatrix);
	_localPosition = matrixPosition.GetPosition();
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	ワールド回転
//!	@param	[in]	rotate		回転
//-----------------------------------------------------------------------------
void GameObjectBase::SetWorldRotate(const Quaternion& rotate)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixRotate = Matrix::Rotate(rotate) * Matrix::Invert(parentMatrix);
	_localRotate = matrixRotate.GetRotateQuaternion();
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	ワールドサイズ
//!	@param	[in]	scale		サイズ
//-----------------------------------------------------------------------------
void GameObjectBase::SetWorldScale(const Vector3& scale)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixScale = Matrix::Scale(scale) * Matrix::Invert(parentMatrix);
	_localScale = matrixScale.GetScale();
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	ワールド回転
//!	@param	[in]	rotate		回転
//-----------------------------------------------------------------------------
void GameObjectBase::AddWorldPosition(const Vector3& position)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixPosition = Matrix::Translate(position) * Matrix::Invert(parentMatrix);
	_localPosition = _localPosition + matrixPosition.GetPosition();
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	ワールド回転
//!	@param	[in]	rotate		回転
//-----------------------------------------------------------------------------
void GameObjectBase::AddWorldRotate(const Quaternion& rotate)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixRotate = Matrix::Rotate(rotate) * Matrix::Invert(parentMatrix);
	_localRotate = _localRotate * matrixRotate.GetRotateQuaternion();
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	ワールドサイズ
//!	@param	[in]	scale		サイズ
//-----------------------------------------------------------------------------
void GameObjectBase::AddWorldScale(const Vector3& scale)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixScale = Matrix::Scale(scale) * Matrix::Invert(parentMatrix);
	_localScale = _localScale + matrixScale.GetPosition();
	MatrixUpdate();
}


//-----------------------------------------------------------------------------
//!	ローカル行列
//!	@param	[in]	matrix		行列
//-----------------------------------------------------------------------------
void GameObjectBase::SetLocalMatrix(const Matrix& matrix)
{
	_localMatrix	= matrix;
	_localPosition	= matrix.GetPosition();
	_localRotate	= matrix.GetRotateQuaternion();
	_localScale		= matrix.GetScale();
}

//-----------------------------------------------------------------------------
//!	ローカル座標
//!	@param	[in]	position	座標
//-----------------------------------------------------------------------------
void GameObjectBase::SetLocalPosition(const Vector3& position)
{
	_localPosition = (position.Rotate(_localRotate.GetAngleRotate()));
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	ローカル回転
//!	@param	[in]	rotate		回転
//-----------------------------------------------------------------------------
void GameObjectBase::SetLocalRotate(const Quaternion& rotate)
{
	_localRotate = rotate;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	ローカルサイズ
//!	@param	[in]	scale		サイズ
//-----------------------------------------------------------------------------
void GameObjectBase::SetLocalScale(const Vector3& scale)
{
	_localScale = scale;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	ローカル座標
//!	@param	[in]	position	座標
//-----------------------------------------------------------------------------
void GameObjectBase::AddLocalPosition(const Vector3& position)
{
	_localPosition = _localPosition + (position.Rotate(_localRotate.GetAngleRotate()));
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	ローカル回転
//!	@param	[in]	rotate		回転
//-----------------------------------------------------------------------------
void GameObjectBase::AddLocalRotate(const Quaternion& rotate)
{
	_localRotate = _localRotate * rotate;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	ローカルサイズ
//!	@param	[in]	scale		サイズ
//-----------------------------------------------------------------------------
void GameObjectBase::AddLocalScale(const Vector3& scale)
{
	_localScale = _localScale + scale;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	親ローカル座標
//!	@param	[in]	position	座標
//-----------------------------------------------------------------------------
void GameObjectBase::SetRelativePosition(const Vector3& position)
{
	_localPosition = position;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	親ローカル回転
//!	@param	[in]	rotate		回転
//-----------------------------------------------------------------------------
void GameObjectBase::SetRelativeRotate(const Quaternion& rotate)
{
	_localRotate = rotate;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	親ローカル座標
//!	@param	[in]	position	座標
//-----------------------------------------------------------------------------
void GameObjectBase::AddRelativePosition(const Vector3& position)
{
	_localPosition = _localPosition + position;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	親ローカル回転
//!	@param	[in]	rotate		回転
//-----------------------------------------------------------------------------
void GameObjectBase::AddRelativeRotate(const Quaternion& rotate)
{
	_localRotate = _localRotate * rotate;
	MatrixUpdate();
}


//-----------------------------------------------------------------------------
//!	ワールド行列作成
//!	@param	[in]	myMatrix	掛け足す行列
//!	@param	[in]	pParent		親オブジェクトのポインタ default:nullptr
//-----------------------------------------------------------------------------
Matrix GameObjectBase::CreateWorldMatrix(const Matrix& myMatrix, GameObjectBase* pParent) const
{
	Matrix result;
	if (pParent)
	{
		Matrix parentMat = pParent->GetLocalMatrix();
		Matrix parentWorldMatrix = pParent->CreateWorldMatrix(parentMat, pParent->_pParentObject);
		result = myMatrix * parentWorldMatrix;

	}
	else
	{
		result = myMatrix;
	}
	return result;
}

//-----------------------------------------------------------------------------
//!	行列のバインド
//-----------------------------------------------------------------------------
void GameObjectBase::BindWorldMatrix()
{
	Manager::Renderer()->SetWorldMatrix(GetWorldMatrix());
}

//-----------------------------------------------------------------------------
//!	Processオブジェクト追加
//!	@param	[in]	pProcess	Processオブジェクトのポインタ
//-----------------------------------------------------------------------------
void GameObjectBase::AddProcess(ProcessBase* pProcess)
{
	if(pProcess)
	{
		pProcess->AddRef();
		_pProcesss.push_back(pProcess);
		pProcess->SetParent(this);
	}
}

//-----------------------------------------------------------------------------
//!	Meshオブジェクト追加
//!	@param	[in]	pProcess	Meshオブジェクトのポインタ
//-----------------------------------------------------------------------------
void GameObjectBase::AddMesh(MeshBase* pMesh)
{
	if(pMesh)
	{
		pMesh->AddRef();
		_pMeshs.push_back(pMesh);
		pMesh->SetParent(this);
	}
}

//-----------------------------------------------------------------------------
//!	Processオブジェクトのタグ検索
//!	@param	[in]	tag	タグ
//!	@return	検索結果
//-----------------------------------------------------------------------------
std::vector<ProcessBase*> GameObjectBase::GetProcesss(const std::string& tag)
{
	std::vector<ProcessBase*> result;
	for (auto* pProcess : _pProcesss)
	{
		if (pProcess->SearchTag(tag))
		{
			result.push_back(pProcess);
		}
	}
	return result;
}

//-----------------------------------------------------------------------------
//!	Processオブジェクトのタグ検索
//!	@param	[in]	tags	タグ
//!	@return	検索結果
//-----------------------------------------------------------------------------
std::vector<ProcessBase*> GameObjectBase::GetProcesss(const std::vector<std::string>& tags)
{
	std::vector<ProcessBase*> result;
	for (auto* pProcess : _pProcesss)
	{
		if (pProcess->SearchTag(tags))
		{
			result.push_back(pProcess);
		}
	}
	return result;
}

//-----------------------------------------------------------------------------
//!	Meshオブジェクトのタグ検索
//!	@param	[in]	tag	タグ
//!	@return	検索結果
//-----------------------------------------------------------------------------
std::vector<MeshBase*> GameObjectBase::GetMeshs(const std::string& tag)
{
	std::vector<MeshBase*> result;
	for(auto* pMesh : _pMeshs)
	{
		if(pMesh->SearchTag(tag))
		{
			result.push_back(pMesh);
		}
	}
	return result;
}

//-----------------------------------------------------------------------------
//!	Meshオブジェクトのタグ検索
//!	@param	[in]	tags	タグ
//!	@return	検索結果
//-----------------------------------------------------------------------------
std::vector<MeshBase*> GameObjectBase::GetMeshs(const std::vector<std::string>& tags)
{
	std::vector<MeshBase*> result;
	for (auto* pMesh : _pMeshs)
	{
		if (pMesh->SearchTag(tags))
		{
			result.push_back(pMesh);
		}
	}
	return result;
}

//-----------------------------------------------------------------------------
//!	プロセス更新
//-----------------------------------------------------------------------------
void GameObjectBase::ProcessUpdate()
{
	for( auto& pProcess : _pProcesss )
	{
		if( !pProcess )continue;
		pProcess->Update();
	}
}

//-----------------------------------------------------------------------------
//!	メッシュ描画
//-----------------------------------------------------------------------------
void GameObjectBase::MeshRender()
{
	for( auto& pMesh : _pMeshs )
	{
		pMesh->Render();
	}
}


//-----------------------------------------------------------------------------
//!	プロセス解放
//!	@param	[in]	pProcess	プロセスのポインタ
//-----------------------------------------------------------------------------
void GameObjectBase::ReleaseProcess(ProcessBase* pProcess)
{
	if(!pProcess){ return; }
	auto itr = _pProcesss.begin();
	while(itr != _pProcesss.end())
	{
		auto* p = (*itr);
		if(p == pProcess)
		{
			SafeRelease(p);
			_pProcesss.erase(itr);
			break;
		}
		++itr;
	}
}

//-----------------------------------------------------------------------------
//!	メッシュ解放
//!	@param	[in]	pMesh		メッシュのポインタ
//-----------------------------------------------------------------------------
void GameObjectBase::ReleaseMesh(MeshBase* pMesh)
{
	if(!pMesh){ return; }
	auto itr = _pMeshs.begin();
	while(itr != _pMeshs.end())
	{
		auto* p = (*itr);
		if(p == pMesh)
		{
			SafeRelease(p);
			_pMeshs.erase(itr);
		}
		++itr;
	}
}


