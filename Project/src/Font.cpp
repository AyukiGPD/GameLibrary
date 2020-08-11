//=============================================================================
//!
//!	@file	Font.cpp
//!	@brief	フォント
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//	FontData
//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
FontData::FontData()
:_pFont(nullptr)
{
}
//-----------------------------------------------------------------------------
//!	コンストラクタ
//!	@param	[in]	fileName	ファイル
//!	@param	[in]	size		フォントサイズ	default 16
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
//	読み込み関数
//!	@param	[in]	fileName	ファイル
//!	@param	[in]	size		フォントサイズ	default 16
//!	@retval	読み込み成功失敗
//-----------------------------------------------------------------------------
bool FontData::Load(const std::string& fileName, u32 size)
{
	_pFont = new FTPixmapFont(fileName.c_str());

	//	読み込みチェック
	if(_pFont == nullptr||_pFont->Error())
	{
		std::cout << "Failed to open font " << fileName << std::endl;
		SafeDelete(_pFont);
		return false;
	}
	//	フォントサイズ設定
	_pFont->FaceSize(size);
	//_lastWriteTexture._uv.y += size;
	_fileName = fileName;
	return true;
}
//-----------------------------------------------------------------------------
//!	フォントサイズ設定
//!	@param	[in]	size	文字サイズ
//-----------------------------------------------------------------------------
void FontData::SetSize(u32 size)
{
	_pFont->FaceSize(size);
}


//-----------------------------------------------------------------------------
//	解放
//-----------------------------------------------------------------------------
void FontData::Clear()
{
	SafeDelete(_pFont);
}

//-----------------------------------------------------------------------------
//!	描画関数
//!	@param	[in]	str		描画文字列
//!	@param	[in]	x		座標
//!	@param	[in]	y		座標
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
	//	座標指定
	glRasterPos2f(x, y);
	//	文字列描画
	_pFont->Render(str.c_str());
}

//-----------------------------------------------------------------------------
//!	描画関数
//!	@param	[in]	str		描画文字列
//!	@param	[in]	pos		座標
//-----------------------------------------------------------------------------
void FontData::Render(const std::wstring& str, const Vector2& position)
{
	Render(str, position.x, position.y);
}
//-----------------------------------------------------------------------------
//!	描画関数
//!	@param	[in]	strs	描画文字列の配列
//!	@param	[in]	x		座標
//!	@param	[in]	y		座標
//!	@param	[in]	padding	改行の空白
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
		//	文字サイズ分下にずらして改行
		y += _pFont->LineHeight()+padding;
	}
}

//-----------------------------------------------------------------------------
//!	描画関数
//!	@param	[in]	strs	描画文字列の配列
//!	@param	[in]	pos		座標
//!	@param	[in]	padding	改行の空白
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


//!	描画関数
//!	@param	[in]	str		描画文字列
//!	@param	[in]	x		座標
//!	@param	[in]	y		座標
void FontHundler::Render(const std::wstring& str, f32 x, f32 y)
{
	if (!_pFontData)return;
	_pFontData->Render(str, x, y);
}

//!	描画関数
//!	@param	[in]	str		描画文字列
//!	@param	[in]	pos		座標
void FontHundler::Render(const std::wstring& str, const Vector2& position)
{
	if (!_pFontData)return;
	_pFontData->Render(str, position);
}

//!	描画関数
//!	@param	[in]	strs	描画文字列の配列
//!	@param	[in]	x		座標
//!	@param	[in]	y		座標
//!	@param	[in]	padding	改行の空白
void FontHundler::Render(const std::vector<std::wstring>& strs, f32 x, f32 y, f32 padding)
{
	if (!_pFontData)return;
	_pFontData->Render(strs, x,y,padding);
}

//!	描画関数
//!	@param	[in]	strs	描画文字列の配列
//!	@param	[in]	pos		座標
//!	@param	[in]	padding	改行の空白
void FontHundler::Render(const std::vector<std::wstring>& strs, const Vector2& position, f32 padding)
{
	if (!_pFontData)return;
	_pFontData->Render(strs, position, padding);
}
//	FontManager
//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
FontManager::FontManager()
{
}

//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
FontManager::~FontManager()
{
}

//-----------------------------------------------------------------------------
//!	解放
//-----------------------------------------------------------------------------
void FontManager::Clear(FontHundler fontHundler)
{
	//	番号が範囲外なら何もせず返す
	if (!fontHundler)return;
	SafeDelete(_pFontDatas[fontHundler.GetFileName()]);
}
//-----------------------------------------------------------------------------
//!	すべて解放
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
//!	読み込み関数
//!	@param	[in]	fileName	ファイル
//!	@param	[in]	size		フォントサイズ	default 64
//!	@reurn	フォント番号	失敗時は-1
//-----------------------------------------------------------------------------
FontHundler FontManager::Load(const std::string& fileName, u32 size)
{
	if (fileName.empty())
	{
		return FontHundler(nullptr);
	}
	//	同じものを探す
	if (_pFontDatas[fileName])
	{
		return FontHundler(_pFontDatas[fileName]);
	}

	FontData* pFontData = new FontData(fileName, size);
	if (pFontData->IsEmpty())
	{
		SafeDelete(pFontData);
		return FontHundler(nullptr);	//	空を返す
	}
	_pFontDatas[fileName] = pFontData;
	return FontHundler(_pFontDatas[fileName]);
}
