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
//  @brief  コンストラクタ
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
//  @brief  コンストラクタ
//  @param	[in]	position	座標
//  @param	[in]	rotate		回転
//  @param	[in]	Scale		サイズ	default:Vector3::ONE
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
//  @brief  デストラクタ
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
//  @brief  親設定
//  @param	[in]	pParentObject	親のポインタ
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
//  @brief  親取得
//  @return	親オブジェクトポインタ
//  @retval	nullptr	親なし
//-----------------------------------------------------------------------------
GameObjectBase* GameObjectBase::GetParentObject()const
{
	return _pParentObject;
}


//-----------------------------------------------------------------------------
//  @brief  子追加
//  @param	[in]	pChildObject	子のポインタ
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
//  @brief  親からの解放
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
//  @brief  子オブジェクトタグ検索
//  @param	[in]	tag	オブジェクトのタグ
//  @return	オブジェクトのポインタ
//  @retval	nullptr	データなし
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
//  @brief  子オブジェクトの解放
//-----------------------------------------------------------------------------
void GameObjectBase::ReleaseChildObjects()
{
	for (auto* pChild : _pChildrenObjects)
	{
		DestroyObject(pChild, false);
	}
}

//-----------------------------------------------------------------------------
//  @brief  ワールド行列設定
//  @param	[in]	matrix		行列
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
//  @brief  ワールド回転
//  @param	[in]	rotate		回転
//-----------------------------------------------------------------------------
void GameObjectBase::SetWorldPosition(const Vector3& position)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixPosition = Matrix::Translate(position) * Matrix::Invert(parentMatrix);
	_localPosition = matrixPosition.GetPosition();
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  ワールド回転
//  @param	[in]	rotate		回転
//-----------------------------------------------------------------------------
void GameObjectBase::SetWorldRotate(const Quaternion& rotate)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixRotate = Matrix::Rotate(rotate) * Matrix::Invert(parentMatrix);
	_localRotate = matrixRotate.GetRotateQuaternion();
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  ワールドサイズ
//  @param	[in]	scale		サイズ
//-----------------------------------------------------------------------------
void GameObjectBase::SetWorldScale(const Vector3& scale)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixScale = Matrix::Scale(scale) * Matrix::Invert(parentMatrix);
	_localScale = matrixScale.GetScale();
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  ワールド回転
//  @param	[in]	rotate		回転
//-----------------------------------------------------------------------------
void GameObjectBase::AddWorldPosition(const Vector3& position)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixPosition = Matrix::Translate(position) * Matrix::Invert(parentMatrix);
	_localPosition = _localPosition + matrixPosition.GetPosition();
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  ワールド回転
//  @param	[in]	rotate		回転
//-----------------------------------------------------------------------------
void GameObjectBase::AddWorldRotate(const Quaternion& rotate)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixRotate = Matrix::Rotate(rotate) * Matrix::Invert(parentMatrix);
	_localRotate = _localRotate * matrixRotate.GetRotateQuaternion();
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  ワールドサイズ
//  @param	[in]	scale		サイズ
//-----------------------------------------------------------------------------
void GameObjectBase::AddWorldScale(const Vector3& scale)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixScale = Matrix::Scale(scale) * Matrix::Invert(parentMatrix);
	_localScale = _localScale + matrixScale.GetPosition();
	MatrixUpdate();
}


//-----------------------------------------------------------------------------
//  @brief  ローカル行列
//  @param	[in]	matrix		行列
//-----------------------------------------------------------------------------
void GameObjectBase::SetLocalMatrix(const Matrix& matrix)
{
	_localMatrix	= matrix;
	_localPosition	= matrix.GetPosition();
	_localRotate	= matrix.GetRotateQuaternion();
	_localScale		= matrix.GetScale();
}

//-----------------------------------------------------------------------------
//  @brief  ローカル座標
//  @param	[in]	position	座標
//-----------------------------------------------------------------------------
void GameObjectBase::SetLocalPosition(const Vector3& position)
{
	_localPosition = (position.Rotate(_localRotate.GetAngleRotate()));
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  ローカル回転
//  @param	[in]	rotate		回転
//-----------------------------------------------------------------------------
void GameObjectBase::SetLocalRotate(const Quaternion& rotate)
{
	_localRotate = rotate;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  ローカルサイズ
//  @param	[in]	scale		サイズ
//-----------------------------------------------------------------------------
void GameObjectBase::SetLocalScale(const Vector3& scale)
{
	_localScale = scale;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  ローカル座標
//  @param	[in]	position	座標
//-----------------------------------------------------------------------------
void GameObjectBase::AddLocalPosition(const Vector3& position)
{
	_localPosition = _localPosition + (position.Rotate(_localRotate.GetAngleRotate()));
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  ローカル回転
//  @param	[in]	rotate		回転
//-----------------------------------------------------------------------------
void GameObjectBase::AddLocalRotate(const Quaternion& rotate)
{
	_localRotate = _localRotate * rotate;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  ローカルサイズ
//  @param	[in]	scale		サイズ
//-----------------------------------------------------------------------------
void GameObjectBase::AddLocalScale(const Vector3& scale)
{
	_localScale = _localScale + scale;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  親ローカル座標
//  @param	[in]	position	座標
//-----------------------------------------------------------------------------
void GameObjectBase::SetRelativePosition(const Vector3& position)
{
	_localPosition = position;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  親ローカル回転
//  @param	[in]	rotate		回転
//-----------------------------------------------------------------------------
void GameObjectBase::SetRelativeRotate(const Quaternion& rotate)
{
	_localRotate = rotate;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  親ローカル座標
//  @param	[in]	position	座標
//-----------------------------------------------------------------------------
void GameObjectBase::AddRelativePosition(const Vector3& position)
{
	_localPosition = _localPosition + position;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  親ローカル回転
//  @param	[in]	rotate		回転
//-----------------------------------------------------------------------------
void GameObjectBase::AddRelativeRotate(const Quaternion& rotate)
{
	_localRotate = _localRotate * rotate;
	MatrixUpdate();
}


//-----------------------------------------------------------------------------
//  @brief  ワールド行列作成
//  @param	[in]	myMatrix	掛け足す行列
//  @param	[in]	pParent		親オブジェクトのポインタ default:nullptr
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
//  @brief  ローカル行列
//  @return 行列
//-----------------------------------------------------------------------------
Matrix GameObjectBase::GetLocalMatrix() const
{
	return _localMatrix;
}

//-----------------------------------------------------------------------------
//  @brief  ローカル座標
//  @return 座標
//-----------------------------------------------------------------------------
Vector3 GameObjectBase::GetLocalPosition() const
{
	return _localPosition;
}

//-----------------------------------------------------------------------------
//  @brief  ローカル回転
//  @return 回転
//-----------------------------------------------------------------------------
Quaternion GameObjectBase::GetLocalRotate() const
{
	return _localRotate;
}

//-----------------------------------------------------------------------------
//  @brief  ローカルサイズ
//  @return サイズ
//-----------------------------------------------------------------------------
Vector3 GameObjectBase::GetLocalScale() const
{
	return _localScale;
}

//-----------------------------------------------------------------------------
//  @brief  ワールド行列
//  @return	行列
//-----------------------------------------------------------------------------
Matrix GameObjectBase::GetWorldMatrix() const
{
	return _worldMatrix;
}

//-----------------------------------------------------------------------------
//  @brief  親のワールド行列
//  @return	行列
//-----------------------------------------------------------------------------
Matrix GameObjectBase::GetWorldParentMatrix() const
{
	if (_pParentObject)
	{
		return _pParentObject->_worldMatrix;
	}
	return Matrix::IDENTITY;
}

//-----------------------------------------------------------------------------
//  @brief  ワールド座標
//  @return	座標
//-----------------------------------------------------------------------------
Vector3 GameObjectBase::GetWorldPosition() const
{
	return GetWorldMatrix().GetPosition();
}

//-----------------------------------------------------------------------------
//  @brief  ワールド回転
//  @return	回転
//-----------------------------------------------------------------------------
Quaternion GameObjectBase::GetWorldRotate() const
{
	return GetWorldMatrix().GetRotateQuaternion();
}

//-----------------------------------------------------------------------------
//  @brief  ワールドサイズ
//  @return	サイズ
//-----------------------------------------------------------------------------
Vector3 GameObjectBase::GetWorldScale() const
{
	return GetWorldMatrix().GetScale();
}

//-----------------------------------------------------------------------------
//  @brief  オブジェクトワールドXベクトル
//  @return	オブジェクトワールドXベクトル
//-----------------------------------------------------------------------------
Vector3 GameObjectBase::GetWorldXVector() const
{
	return GetWorldMatrix().GetXVector();
}

//-----------------------------------------------------------------------------
//  @brief  オブジェクトワールドYベクトル
//  @return	オブジェクトワールドYベクトル
//-----------------------------------------------------------------------------
Vector3 GameObjectBase::GetWorldYVector() const
{
	return GetWorldMatrix().GetYVector();
}

//-----------------------------------------------------------------------------
//  @brief  オブジェクトワールドZベクトル
//  @return	オブジェクトワールドZベクトル
//-----------------------------------------------------------------------------
Vector3 GameObjectBase::GetWorldZVector() const
{
	return GetWorldMatrix().GetZVector();
}

//-----------------------------------------------------------------------------
//  @brief  行列のバインド
//-----------------------------------------------------------------------------
void GameObjectBase::BindWorldMatrix()
{
	Manager::Renderer()->SetWorldMatrix(GetWorldMatrix());
}

//-----------------------------------------------------------------------------
//  @brief  Processオブジェクト追加
//  @param	[in]	pProcess	Processオブジェクトのポインタ
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
//  @brief  Meshオブジェクト追加
//  @param	[in]	pProcess	Meshオブジェクトのポインタ
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
//  @brief  Processオブジェクトのタグ検索
//  @param	[in]	tag	タグ
//  @return	検索結果
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
//  @brief  Processオブジェクトのタグ検索
//  @param	[in]	tags	タグ
//  @return	検索結果
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
//  @brief  Meshオブジェクトのタグ検索
//  @param	[in]	tag	タグ
//  @return	検索結果
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
//  @brief  Meshオブジェクトのタグ検索
//  @param	[in]	tags	タグ
//  @return	検索結果
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
//  @brief  有効化フラグ設定
//  @param	[in]	enable	有効化
//-----------------------------------------------------------------------------
void GameObjectBase::Enable(bool enable)
{
	_enable = enable;
}

//-----------------------------------------------------------------------------
//  @brief  有効化フラグ取得
//  @retval	true	有効
//  @retval	false	無効
//-----------------------------------------------------------------------------
bool GameObjectBase::IsEnable() const
{
	return _enable;
}

//-----------------------------------------------------------------------------
//  @brief  行列の更新
//-----------------------------------------------------------------------------
void GameObjectBase::MatrixUpdate()
{
	_localMatrix = Matrix::Affine(_localPosition, _localRotate, _localScale);
	if (_pParentObject)
	{
		_worldMatrix = _localMatrix * _pParentObject->_worldMatrix;
	}
	else
	{
		_worldMatrix = _localMatrix;
	}
	for (auto* pChild : _pChildrenObjects)
	{
		pChild->MatrixUpdate();
	}
}

//-----------------------------------------------------------------------------
//  @brief  プロセス更新
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
//  @brief  メッシュ描画
//-----------------------------------------------------------------------------
void GameObjectBase::MeshRender()
{
	for( auto& pMesh : _pMeshs )
	{
		pMesh->Render();
	}
}


//-----------------------------------------------------------------------------
//  @brief  プロセス解放
//  @param	[in]	pProcess	プロセスのポインタ
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
//  @brief  メッシュ解放
//  @param	[in]	pMesh		メッシュのポインタ
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

//-----------------------------------------------------------------------------
//  @brief  子オブジェクトの更新
//-----------------------------------------------------------------------------
void GameObjectBase::ChildrenUpdate()
{
	for (auto& pObject : _pChildrenObjects)
	{
		if (pObject && pObject->IsEnable())
		{
			pObject->Update();
			if (!pObject)continue;	//	消えてたら次のオブジェクトへ
			pObject->ProcessUpdate();
			if (!pObject)continue;	//	消えてたら次のオブジェクトへ
			pObject->ChildrenUpdate();
		}
	}

	//	消えたオブジェクトをリストから消す
	auto itr = _pChildrenObjects.begin();
	while (itr != _pChildrenObjects.end())
	{
		if (!(*itr))
		{
			itr = _pChildrenObjects.erase(itr);
			continue;
		}
		++itr;
	}

}

//-----------------------------------------------------------------------------
//  @brief  子オブジェクトの描画
//-----------------------------------------------------------------------------
void GameObjectBase::ChildrenRender()
{
	auto itr = _pChildrenObjects.begin();
	while (itr != _pChildrenObjects.end())
	{
		GameObjectBase* p = (*itr);
		if (p && p->IsEnable())
		{
			p->BindWorldMatrix();
			p->Render();
			p->MeshRender();
			p->ChildrenRender();
		}
		++itr;
	}
}


