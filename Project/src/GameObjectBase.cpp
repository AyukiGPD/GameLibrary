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
//!	�R���X�g���N�^
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
//!	�R���X�g���N�^
//!	@param	[in]	position	���W
//!	@param	[in]	rotate		��]
//!	@param	[in]	Scale		�T�C�Y	default:Vector3::ONE
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
//! �f�X�g���N�^
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
//!	�e�ݒ�
//!@param	[in]	pParentObject	�e�̃|�C���^
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
//!	�e�擾
//!	@return	�e�I�u�W�F�N�g�|�C���^
//!	@retval	nullptr	�e�Ȃ�
//-----------------------------------------------------------------------------
GameObjectBase* GameObjectBase::GetParentObject()const
{
	return _pParentObject;
}


//-----------------------------------------------------------------------------
//!	�q�ǉ�
//!@param	[in]	pChildObject	�q�̃|�C���^
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
//!	�e����̉��
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
//!	�q�I�u�W�F�N�g�^�O����
//!	@param	[in]	tag	�I�u�W�F�N�g�̃^�O
//!	@return	�I�u�W�F�N�g�̃|�C���^
//!	@retval	nullptr	�f�[�^�Ȃ�
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
//!	�q�I�u�W�F�N�g�̉��
//-----------------------------------------------------------------------------
void GameObjectBase::ReleaseChildObjects()
{
	for (auto* pChild : _pChildrenObjects)
	{
		DestroyObject(pChild, false);
	}
}

//-----------------------------------------------------------------------------
//!	���[���h�s��ݒ�
//!	@param	[in]	matrix		�s��
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
//!	���[���h��]
//!	@param	[in]	rotate		��]
//-----------------------------------------------------------------------------
void GameObjectBase::SetWorldPosition(const Vector3& position)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixPosition = Matrix::Translate(position) * Matrix::Invert(parentMatrix);
	_localPosition = matrixPosition.GetPosition();
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	���[���h��]
//!	@param	[in]	rotate		��]
//-----------------------------------------------------------------------------
void GameObjectBase::SetWorldRotate(const Quaternion& rotate)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixRotate = Matrix::Rotate(rotate) * Matrix::Invert(parentMatrix);
	_localRotate = matrixRotate.GetRotateQuaternion();
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	���[���h�T�C�Y
//!	@param	[in]	scale		�T�C�Y
//-----------------------------------------------------------------------------
void GameObjectBase::SetWorldScale(const Vector3& scale)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixScale = Matrix::Scale(scale) * Matrix::Invert(parentMatrix);
	_localScale = matrixScale.GetScale();
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	���[���h��]
//!	@param	[in]	rotate		��]
//-----------------------------------------------------------------------------
void GameObjectBase::AddWorldPosition(const Vector3& position)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixPosition = Matrix::Translate(position) * Matrix::Invert(parentMatrix);
	_localPosition = _localPosition + matrixPosition.GetPosition();
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	���[���h��]
//!	@param	[in]	rotate		��]
//-----------------------------------------------------------------------------
void GameObjectBase::AddWorldRotate(const Quaternion& rotate)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixRotate = Matrix::Rotate(rotate) * Matrix::Invert(parentMatrix);
	_localRotate = _localRotate * matrixRotate.GetRotateQuaternion();
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	���[���h�T�C�Y
//!	@param	[in]	scale		�T�C�Y
//-----------------------------------------------------------------------------
void GameObjectBase::AddWorldScale(const Vector3& scale)
{
	Matrix parentMatrix = GetWorldParentMatrix();
	Matrix matrixScale = Matrix::Scale(scale) * Matrix::Invert(parentMatrix);
	_localScale = _localScale + matrixScale.GetPosition();
	MatrixUpdate();
}


//-----------------------------------------------------------------------------
//!	���[�J���s��
//!	@param	[in]	matrix		�s��
//-----------------------------------------------------------------------------
void GameObjectBase::SetLocalMatrix(const Matrix& matrix)
{
	_localMatrix	= matrix;
	_localPosition	= matrix.GetPosition();
	_localRotate	= matrix.GetRotateQuaternion();
	_localScale		= matrix.GetScale();
}

//-----------------------------------------------------------------------------
//!	���[�J�����W
//!	@param	[in]	position	���W
//-----------------------------------------------------------------------------
void GameObjectBase::SetLocalPosition(const Vector3& position)
{
	_localPosition = (position.Rotate(_localRotate.GetAngleRotate()));
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	���[�J����]
//!	@param	[in]	rotate		��]
//-----------------------------------------------------------------------------
void GameObjectBase::SetLocalRotate(const Quaternion& rotate)
{
	_localRotate = rotate;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	���[�J���T�C�Y
//!	@param	[in]	scale		�T�C�Y
//-----------------------------------------------------------------------------
void GameObjectBase::SetLocalScale(const Vector3& scale)
{
	_localScale = scale;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	���[�J�����W
//!	@param	[in]	position	���W
//-----------------------------------------------------------------------------
void GameObjectBase::AddLocalPosition(const Vector3& position)
{
	_localPosition = _localPosition + (position.Rotate(_localRotate.GetAngleRotate()));
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	���[�J����]
//!	@param	[in]	rotate		��]
//-----------------------------------------------------------------------------
void GameObjectBase::AddLocalRotate(const Quaternion& rotate)
{
	_localRotate = _localRotate * rotate;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	���[�J���T�C�Y
//!	@param	[in]	scale		�T�C�Y
//-----------------------------------------------------------------------------
void GameObjectBase::AddLocalScale(const Vector3& scale)
{
	_localScale = _localScale + scale;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	�e���[�J�����W
//!	@param	[in]	position	���W
//-----------------------------------------------------------------------------
void GameObjectBase::SetRelativePosition(const Vector3& position)
{
	_localPosition = position;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	�e���[�J����]
//!	@param	[in]	rotate		��]
//-----------------------------------------------------------------------------
void GameObjectBase::SetRelativeRotate(const Quaternion& rotate)
{
	_localRotate = rotate;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	�e���[�J�����W
//!	@param	[in]	position	���W
//-----------------------------------------------------------------------------
void GameObjectBase::AddRelativePosition(const Vector3& position)
{
	_localPosition = _localPosition + position;
	MatrixUpdate();
}

//-----------------------------------------------------------------------------
//!	�e���[�J����]
//!	@param	[in]	rotate		��]
//-----------------------------------------------------------------------------
void GameObjectBase::AddRelativeRotate(const Quaternion& rotate)
{
	_localRotate = _localRotate * rotate;
	MatrixUpdate();
}


//-----------------------------------------------------------------------------
//!	���[���h�s��쐬
//!	@param	[in]	myMatrix	�|�������s��
//!	@param	[in]	pParent		�e�I�u�W�F�N�g�̃|�C���^ default:nullptr
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
//!	�s��̃o�C���h
//-----------------------------------------------------------------------------
void GameObjectBase::BindWorldMatrix()
{
	Manager::Renderer()->SetWorldMatrix(GetWorldMatrix());
}

//-----------------------------------------------------------------------------
//!	Process�I�u�W�F�N�g�ǉ�
//!	@param	[in]	pProcess	Process�I�u�W�F�N�g�̃|�C���^
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
//!	Mesh�I�u�W�F�N�g�ǉ�
//!	@param	[in]	pProcess	Mesh�I�u�W�F�N�g�̃|�C���^
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
//!	Process�I�u�W�F�N�g�̃^�O����
//!	@param	[in]	tag	�^�O
//!	@return	��������
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
//!	Process�I�u�W�F�N�g�̃^�O����
//!	@param	[in]	tags	�^�O
//!	@return	��������
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
//!	Mesh�I�u�W�F�N�g�̃^�O����
//!	@param	[in]	tag	�^�O
//!	@return	��������
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
//!	Mesh�I�u�W�F�N�g�̃^�O����
//!	@param	[in]	tags	�^�O
//!	@return	��������
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
//!	�v���Z�X�X�V
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
//!	���b�V���`��
//-----------------------------------------------------------------------------
void GameObjectBase::MeshRender()
{
	for( auto& pMesh : _pMeshs )
	{
		pMesh->Render();
	}
}


//-----------------------------------------------------------------------------
//!	�v���Z�X���
//!	@param	[in]	pProcess	�v���Z�X�̃|�C���^
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
//!	���b�V�����
//!	@param	[in]	pMesh		���b�V���̃|�C���^
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


