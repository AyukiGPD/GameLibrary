//=============================================================================
//!
//!	@file	DataLoader.h
//!	@brief	�f�[�^�ǂݍ��݊Ǘ��N���X	������
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once
class DataLoader : public ManagerBase
{
public:
	DataLoader();
	virtual ~DataLoader();

	//!	�t�@�C���ǂݍ���
	//!	@param	[in]	fileName	�t�@�C����
	//!	return	�ǂݍ��񂾃t�@�C��	nullptr�Ȃ玸�s
	void* LoadFile(const std::string& fileName);
};

MANAGER_INTERFACE(DataLoader, Loader);
