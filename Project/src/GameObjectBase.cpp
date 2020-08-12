//=============================================================================
//!
//!	@file	GameObjectBase.cpp
//!	@brief	�I�u�W�F�N�g���N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
//  @brief  �R���X�g���N�^
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
//  @brief  �R���X�g���N�^
//  @param	[in]	position	���W
//  @param	[in]	rotate		��]
//  @param	[in]	Scale		�T�C�Y	default:Vector3::ONE
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
//  @brief  �f�X�g���N�^
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
//  @brief  �e�ݒ�
//  @param	[in]	pParentObject	�e�̃|�C���^
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
//  @brief  �e�擾
//  @return	�e�I�u�W�F�N�g�|�C���^
//  @retval	nullptr	�e�Ȃ�
//-----------------------------------------------------------------------------
GameObjectBase* GameObjectBase::GetParentObject()const
{
	return _pParentObject;
}


//-----------------------------------------------------------------------------
//  @brief  �q�ǉ�
//  @param	[in]	pChildObject	�q�̃|�C���^
//-----------------------------------------------------------------------------
void GameObjectBase::AddChildObject(GameObjectBase* pChildObject)
{
	if(pChildObject)
	{
		//	�ǉ����̃}�g���b�N�X�ύX�Ƀo�O
		pChildObject->AddRef();
		_pChildrenObjects.push_back(pChildObject);
		pChildObject->SetParentObject(this);
	}
}

//-----------------------------------------------------------------------------
//!	�q�폜
//!@param	[in]	pChildObject	�q�̃|�C���^
//!	@return	�����̎Q�Ɛ� 0�Ȃ�폜�ς�
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
			count = SafeRelease(*itr);	//	���X�g���̃I�u�W�F�N�g��NULL�ɂ���
			break;
		}
		++itr;
	}
	return count;
}

//-----------------------------------------------------------------------------
//  @brief  �e����̉��
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
//  @brief  �q�I�u�W�F�N�g�^�O����
//  @param	[in]	tag	�I�u�W�F�N�g�̃^�O
//  @return	�I�u�W�F�N�g�̃|�C���^
//  @retval	nullptr	�f�[�^�Ȃ�
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
//  @brief  �q�I�u�W�F�N�g�̉��
//-----------------------------------------------------------------------------
void GameObjectBase::ReleaseChildObjects()
{
	for (auto* pChild : _pChildrenObjects)
	{
		DestroyObject(pChild, false);
	}
}

//-----------------------------------------------------------------------------
//  @brief  ���[���h�s��ݒ�
//  @param	[in]	matrix		�s��
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
//  @brief  ���[���h��]
//  @param	[in]	rotate		��]
//-----------------------------------------------------------------------------
void GameObjectBase::SetWorldPosition(const Vector3& position)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixPosition = Matrix::Translate(position) * Matrix::Invert(parentMatrix);
	_localPosition = matrixPosition.GetPosition();
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  ���[���h��]
//  @param	[in]	rotate		��]
//-----------------------------------------------------------------------------
void GameObjectBase::SetWorldRotate(const Quaternion& rotate)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixRotate = Matrix::Rotate(rotate) * Matrix::Invert(parentMatrix);
	_localRotate = matrixRotate.GetRotateQuaternion();
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  ���[���h�T�C�Y
//  @param	[in]	scale		�T�C�Y
//-----------------------------------------------------------------------------
void GameObjectBase::SetWorldScale(const Vector3& scale)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixScale = Matrix::Scale(scale) * Matrix::Invert(parentMatrix);
	_localScale = matrixScale.GetScale();
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  ���[���h��]
//  @param	[in]	rotate		��]
//-----------------------------------------------------------------------------
void GameObjectBase::AddWorldPosition(const Vector3& position)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixPosition = Matrix::Translate(position) * Matrix::Invert(parentMatrix);
	_localPosition = _localPosition + matrixPosition.GetPosition();
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  ���[���h��]
//  @param	[in]	rotate		��]
//-----------------------------------------------------------------------------
void GameObjectBase::AddWorldRotate(const Quaternion& rotate)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixRotate = Matrix::Rotate(rotate) * Matrix::Invert(parentMatrix);
	_localRotate = _localRotate * matrixRotate.GetRotateQuaternion();
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  ���[���h�T�C�Y
//  @param	[in]	scale		�T�C�Y
//-----------------------------------------------------------------------------
void GameObjectBase::AddWorldScale(const Vector3& scale)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixScale = Matrix::Scale(scale) * Matrix::Invert(parentMatrix);
	_localScale = _localScale + matrixScale.GetPosition();
	MatrixUpdate();
}


//-----------------------------------------------------------------------------
//  @brief  ���[�J���s��
//  @param	[in]	matrix		�s��
//-----------------------------------------------------------------------------
void GameObjectBase::SetLocalMatrix(const Matrix& matrix)
{
	_localMatrix	= matrix;
	_localPosition	= matrix.GetPosition();
	_localRotate	= matrix.GetRotateQuaternion();
	_localScale		= matrix.GetScale();
}

//-----------------------------------------------------------------------------
//  @brief  ���[�J�����W
//  @param	[in]	position	���W
//-----------------------------------------------------------------------------
void GameObjectBase::SetLocalPosition(const Vector3& position)
{
	_localPosition = (position.Rotate(_localRotate.GetAngleRotate()));
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  ���[�J����]
//  @param	[in]	rotate		��]
//-----------------------------------------------------------------------------
void GameObjectBase::SetLocalRotate(const Quaternion& rotate)
{
	_localRotate = rotate;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  ���[�J���T�C�Y
//  @param	[in]	scale		�T�C�Y
//-----------------------------------------------------------------------------
void GameObjectBase::SetLocalScale(const Vector3& scale)
{
	_localScale = scale;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  ���[�J�����W
//  @param	[in]	position	���W
//-----------------------------------------------------------------------------
void GameObjectBase::AddLocalPosition(const Vector3& position)
{
	_localPosition = _localPosition + (position.Rotate(_localRotate.GetAngleRotate()));
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  ���[�J����]
//  @param	[in]	rotate		��]
//-----------------------------------------------------------------------------
void GameObjectBase::AddLocalRotate(const Quaternion& rotate)
{
	_localRotate = _localRotate * rotate;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  ���[�J���T�C�Y
//  @param	[in]	scale		�T�C�Y
//-----------------------------------------------------------------------------
void GameObjectBase::AddLocalScale(const Vector3& scale)
{
	_localScale = _localScale + scale;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  �e���[�J�����W
//  @param	[in]	position	���W
//-----------------------------------------------------------------------------
void GameObjectBase::SetRelativePosition(const Vector3& position)
{
	_localPosition = position;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  �e���[�J����]
//  @param	[in]	rotate		��]
//-----------------------------------------------------------------------------
void GameObjectBase::SetRelativeRotate(const Quaternion& rotate)
{
	_localRotate = rotate;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  �e���[�J�����W
//  @param	[in]	position	���W
//-----------------------------------------------------------------------------
void GameObjectBase::AddRelativePosition(const Vector3& position)
{
	_localPosition = _localPosition + position;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//  @brief  �e���[�J����]
//  @param	[in]	rotate		��]
//-----------------------------------------------------------------------------
void GameObjectBase::AddRelativeRotate(const Quaternion& rotate)
{
	_localRotate = _localRotate * rotate;
	MatrixUpdate();
}


//-----------------------------------------------------------------------------
//  @brief  ���[���h�s��쐬
//  @param	[in]	myMatrix	�|�������s��
//  @param	[in]	pParent		�e�I�u�W�F�N�g�̃|�C���^ default:nullptr
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
//  @brief  ���[�J���s��
//  @return �s��
//-----------------------------------------------------------------------------
Matrix GameObjectBase::GetLocalMatrix() const
{
	return _localMatrix;
}

//-----------------------------------------------------------------------------
//  @brief  ���[�J�����W
//  @return ���W
//-----------------------------------------------------------------------------
Vector3 GameObjectBase::GetLocalPosition() const
{
	return _localPosition;
}

//-----------------------------------------------------------------------------
//  @brief  ���[�J����]
//  @return ��]
//-----------------------------------------------------------------------------
Quaternion GameObjectBase::GetLocalRotate() const
{
	return _localRotate;
}

//-----------------------------------------------------------------------------
//  @brief  ���[�J���T�C�Y
//  @return �T�C�Y
//-----------------------------------------------------------------------------
Vector3 GameObjectBase::GetLocalScale() const
{
	return _localScale;
}

//-----------------------------------------------------------------------------
//  @brief  ���[���h�s��
//  @return	�s��
//-----------------------------------------------------------------------------
Matrix GameObjectBase::GetWorldMatrix() const
{
	return _worldMatrix;
}

//-----------------------------------------------------------------------------
//  @brief  �e�̃��[���h�s��
//  @return	�s��
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
//  @brief  ���[���h���W
//  @return	���W
//-----------------------------------------------------------------------------
Vector3 GameObjectBase::GetWorldPosition() const
{
	return GetWorldMatrix().GetPosition();
}

//-----------------------------------------------------------------------------
//  @brief  ���[���h��]
//  @return	��]
//-----------------------------------------------------------------------------
Quaternion GameObjectBase::GetWorldRotate() const
{
	return GetWorldMatrix().GetRotateQuaternion();
}

//-----------------------------------------------------------------------------
//  @brief  ���[���h�T�C�Y
//  @return	�T�C�Y
//-----------------------------------------------------------------------------
Vector3 GameObjectBase::GetWorldScale() const
{
	return GetWorldMatrix().GetScale();
}

//-----------------------------------------------------------------------------
//  @brief  �I�u�W�F�N�g���[���hX�x�N�g��
//  @return	�I�u�W�F�N�g���[���hX�x�N�g��
//-----------------------------------------------------------------------------
Vector3 GameObjectBase::GetWorldXVector() const
{
	return GetWorldMatrix().GetXVector();
}

//-----------------------------------------------------------------------------
//  @brief  �I�u�W�F�N�g���[���hY�x�N�g��
//  @return	�I�u�W�F�N�g���[���hY�x�N�g��
//-----------------------------------------------------------------------------
Vector3 GameObjectBase::GetWorldYVector() const
{
	return GetWorldMatrix().GetYVector();
}

//-----------------------------------------------------------------------------
//  @brief  �I�u�W�F�N�g���[���hZ�x�N�g��
//  @return	�I�u�W�F�N�g���[���hZ�x�N�g��
//-----------------------------------------------------------------------------
Vector3 GameObjectBase::GetWorldZVector() const
{
	return GetWorldMatrix().GetZVector();
}

//-----------------------------------------------------------------------------
//  @brief  �s��̃o�C���h
//-----------------------------------------------------------------------------
void GameObjectBase::BindWorldMatrix()
{
	Manager::Renderer()->SetWorldMatrix(GetWorldMatrix());
}

//-----------------------------------------------------------------------------
//  @brief  Process�I�u�W�F�N�g�ǉ�
//  @param	[in]	pProcess	Process�I�u�W�F�N�g�̃|�C���^
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
//  @brief  Mesh�I�u�W�F�N�g�ǉ�
//  @param	[in]	pProcess	Mesh�I�u�W�F�N�g�̃|�C���^
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
//  @brief  Process�I�u�W�F�N�g�̃^�O����
//  @param	[in]	tag	�^�O
//  @return	��������
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
//  @brief  Process�I�u�W�F�N�g�̃^�O����
//  @param	[in]	tags	�^�O
//  @return	��������
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
//  @brief  Mesh�I�u�W�F�N�g�̃^�O����
//  @param	[in]	tag	�^�O
//  @return	��������
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
//  @brief  Mesh�I�u�W�F�N�g�̃^�O����
//  @param	[in]	tags	�^�O
//  @return	��������
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
//  @brief  �L�����t���O�ݒ�
//  @param	[in]	enable	�L����
//-----------------------------------------------------------------------------
void GameObjectBase::Enable(bool enable)
{
	_enable = enable;
}

//-----------------------------------------------------------------------------
//  @brief  �L�����t���O�擾
//  @retval	true	�L��
//  @retval	false	����
//-----------------------------------------------------------------------------
bool GameObjectBase::IsEnable() const
{
	return _enable;
}

//-----------------------------------------------------------------------------
//  @brief  �s��̍X�V
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
//  @brief  �v���Z�X�X�V
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
//  @brief  ���b�V���`��
//-----------------------------------------------------------------------------
void GameObjectBase::MeshRender()
{
	for( auto& pMesh : _pMeshs )
	{
		pMesh->Render();
	}
}


//-----------------------------------------------------------------------------
//  @brief  �v���Z�X���
//  @param	[in]	pProcess	�v���Z�X�̃|�C���^
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
//  @brief  ���b�V�����
//  @param	[in]	pMesh		���b�V���̃|�C���^
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
//  @brief  �q�I�u�W�F�N�g�̍X�V
//-----------------------------------------------------------------------------
void GameObjectBase::ChildrenUpdate()
{
	for (auto& pObject : _pChildrenObjects)
	{
		if (pObject && pObject->IsEnable())
		{
			pObject->Update();
			if (!pObject)continue;	//	�����Ă��玟�̃I�u�W�F�N�g��
			pObject->ProcessUpdate();
			if (!pObject)continue;	//	�����Ă��玟�̃I�u�W�F�N�g��
			pObject->ChildrenUpdate();
		}
	}

	//	�������I�u�W�F�N�g�����X�g�������
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
//  @brief  �q�I�u�W�F�N�g�̕`��
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


