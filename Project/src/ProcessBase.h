//=============================================================================
//!
//!	@file	ProcessBase.h
//!	@brief	�v���Z�X���N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//!	@class GameObjectBase BaseObject.h "BaseObject.h"
class GameObjectBase;

//-------------------------------------------------------------------
//! @brief  �v���Z�X���N���X
//-------------------------------------------------------------------
class ProcessBase : public BasicBase
{
public:
	//! @brief  �R���X�g���N�^
	ProcessBase();

	//! @brief  �R���X�g���N�^
	//!	@param	[in]	pParent	�e�I�u�W�F�N�g
	ProcessBase(GameObjectBase* pParent);

	//! @brief  �f�X�g���N�^
	virtual ~ProcessBase();

	//! @brief  �e�I�u�W�F�N�g�ݒ�
	//!	@param	[in]	pParent	�e�I�u�W�F�N�g
	void SetParent(GameObjectBase* pParent);

	//! @brief  �e�I�u�W�F�N�g�ݒ�
	//!	@param	[in]	pParent	�e�I�u�W�F�N�g
	GameObjectBase* GetParent() { return _pParent; }

	//! @brief  �X�V
	virtual void Update() = 0;

protected:
	GameObjectBase* _pParent;	//!< �e�I�u�W�F�N�g
};

