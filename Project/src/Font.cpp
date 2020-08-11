//=============================================================================
//!
//!	@file	Font.cpp
//!	@brief	�t�H���g
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//	FontData
//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
FontData::FontData()
:_pFont(nullptr)
{
}
//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//!	@param	[in]	fileName	�t�@�C��
//!	@param	[in]	size		�t�H���g�T�C�Y	default 16
//-----------------------------------------------------------------------------
FontData::FontData(const std::string& fileName, u32 size)
:_pFont(nullptr)
{
	Load(fileName,size);
}
FontData::~FontData()
{
	Clear();
}
//-----------------------------------------------------------------------------
//	�ǂݍ��݊֐�
//!	@param	[in]	fileName	�t�@�C��
//!	@param	[in]	size		�t�H���g�T�C�Y	default 16
//!	@retval	�ǂݍ��ݐ������s
//-----------------------------------------------------------------------------
bool FontData::Load(const std::string& fileName, u32 size)
{
	_pFont = new FTPixmapFont(fileName.c_str());

	//	�ǂݍ��݃`�F�b�N
	if(_pFont == nullptr||_pFont->Error())
	{
		std::cout << "Failed to open font " << fileName << std::endl;
		SafeDelete(_pFont);
		return false;
	}
	//	�t�H���g�T�C�Y�ݒ�
	_pFont->FaceSize(size);
	//_lastWriteTexture._uv.y += size;
	_fileName = fileName;
	return true;
}
//-----------------------------------------------------------------------------
//!	�t�H���g�T�C�Y�ݒ�
//!	@param	[in]	size	�����T�C�Y
//-----------------------------------------------------------------------------
void FontData::SetSize(u32 size)
{
	_pFont->FaceSize(size);
}


//-----------------------------------------------------------------------------
//	���
//-----------------------------------------------------------------------------
void FontData::Clear()
{
	SafeDelete(_pFont);
}

//-----------------------------------------------------------------------------
//!	�`��֐�
//!	@param	[in]	str		�`�敶����
//!	@param	[in]	x		���W
//!	@param	[in]	y		���W
//-----------------------------------------------------------------------------
void FontData::Render(const std::wstring& str, f32 x, f32 y)
{
	if(_pFont == nullptr)
	{
		return;
	}
	if(str.empty())
	{
		return;
	}
	//	���W�w��
	glRasterPos2f(x, y);
	//	������`��
	_pFont->Render(str.c_str());
}

//-----------------------------------------------------------------------------
//!	�`��֐�
//!	@param	[in]	str		�`�敶����
//!	@param	[in]	pos		���W
//-----------------------------------------------------------------------------
void FontData::Render(const std::wstring& str, const Vector2& position)
{
	Render(str, position.x, position.y);
}
//-----------------------------------------------------------------------------
//!	�`��֐�
//!	@param	[in]	strs	�`�敶����̔z��
//!	@param	[in]	x		���W
//!	@param	[in]	y		���W
//!	@param	[in]	padding	���s�̋�
//-----------------------------------------------------------------------------
void FontData::Render(const std::vector<std::wstring>& strs, f32 x, f32 y, f32 padding)
{
	if(_pFont == nullptr)
	{
		return;
	}

	if(strs.empty())
	{
		return;
	}

	for(u32 i = 0; i < strs.size(); ++i)
	{
		Render(strs[i], x, y);
		//	�����T�C�Y�����ɂ��炵�ĉ��s
		y += _pFont->LineHeight()+padding;
	}
}

//-----------------------------------------------------------------------------
//!	�`��֐�
//!	@param	[in]	strs	�`�敶����̔z��
//!	@param	[in]	pos		���W
//!	@param	[in]	padding	���s�̋�
//-----------------------------------------------------------------------------
void FontData::Render(const std::vector<std::wstring>& strs, const Vector2& position, f32 padding)
{
	Render(strs, position.x, position.y, padding);
}


//void FontData::CreateChar(wchar_t charData)
//{
//}
//
//void FontData::CreateTexture()
//{
//}


//!	�`��֐�
//!	@param	[in]	str		�`�敶����
//!	@param	[in]	x		���W
//!	@param	[in]	y		���W
void FontHundler::Render(const std::wstring& str, f32 x, f32 y)
{
	if (!_pFontData)return;
	_pFontData->Render(str, x, y);
}

//!	�`��֐�
//!	@param	[in]	str		�`�敶����
//!	@param	[in]	pos		���W
void FontHundler::Render(const std::wstring& str, const Vector2& position)
{
	if (!_pFontData)return;
	_pFontData->Render(str, position);
}

//!	�`��֐�
//!	@param	[in]	strs	�`�敶����̔z��
//!	@param	[in]	x		���W
//!	@param	[in]	y		���W
//!	@param	[in]	padding	���s�̋�
void FontHundler::Render(const std::vector<std::wstring>& strs, f32 x, f32 y, f32 padding)
{
	if (!_pFontData)return;
	_pFontData->Render(strs, x,y,padding);
}

//!	�`��֐�
//!	@param	[in]	strs	�`�敶����̔z��
//!	@param	[in]	pos		���W
//!	@param	[in]	padding	���s�̋�
void FontHundler::Render(const std::vector<std::wstring>& strs, const Vector2& position, f32 padding)
{
	if (!_pFontData)return;
	_pFontData->Render(strs, position, padding);
}
//	FontManager
//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
FontManager::FontManager()
{
}

//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
FontManager::~FontManager()
{
}

//-----------------------------------------------------------------------------
//!	���
//-----------------------------------------------------------------------------
void FontManager::Clear(FontHundler fontHundler)
{
	//	�ԍ����͈͊O�Ȃ牽�������Ԃ�
	if (!fontHundler)return;
	SafeDelete(_pFontDatas[fontHundler.GetFileName()]);
}
//-----------------------------------------------------------------------------
//!	���ׂĉ��
//-----------------------------------------------------------------------------
void FontManager::ClearAll()
{
	for (auto& pFontData : _pFontDatas)
	{
		SafeDelete(pFontData.second);
	}
	_pFontDatas.clear();
}

//-----------------------------------------------------------------------------
//!	�ǂݍ��݊֐�
//!	@param	[in]	fileName	�t�@�C��
//!	@param	[in]	size		�t�H���g�T�C�Y	default 64
//!	@reurn	�t�H���g�ԍ�	���s����-1
//-----------------------------------------------------------------------------
FontHundler FontManager::Load(const std::string& fileName, u32 size)
{
	if (fileName.empty())
	{
		return FontHundler(nullptr);
	}
	//	�������̂�T��
	if (_pFontDatas[fileName])
	{
		return FontHundler(_pFontDatas[fileName]);
	}

	FontData* pFontData = new FontData(fileName, size);
	if (pFontData->IsEmpty())
	{
		SafeDelete(pFontData);
		return FontHundler(nullptr);	//	���Ԃ�
	}
	_pFontDatas[fileName] = pFontData;
	return FontHundler(_pFontDatas[fileName]);
}
