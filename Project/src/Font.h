//=============================================================================
//!
//!	@file	Font.h
//!	@brief	フォント
//!
//!	@author	YKCH
//!
//=============================================================================

#include <FTGL/ftgl.h>
#pragma comment (lib, "freetype.lib")
#pragma comment (lib, "ftgl.lib")

//struct CharData
//{
//	Vector2 _uv = Vector2::ZERO;
//	Texture* _pFontTexture = nullptr;
//};

/*!
	@todo
	フォントデータでテクスチャでバッファを作る
*/
//-------------------------------------------------------------------
//!	フォントデータ
//-------------------------------------------------------------------
class FontData
{
public:
	//!	コンストラクタ
	FontData();
	//!	コンストラクタ
	//!	@param	[in]	fileName	ファイル
	//!	@param	[in]	size		フォントサイズ
	FontData(const std::string& fileName, u32 size);
	//!	デストラクタ
	virtual ~FontData();

	//!	解放
	void Clear();

	//!	中身のチェック
	bool IsEmpty()
	{
		return !_pFont;
	}

	//!	フォントサイズ設定
	//!	@param	[in]	size	文字サイズ
	void SetSize(u32 size);

	//	読み込み関数
	//!	@param	[in]	fileName	ファイル
	//!	@param	[in]	size		フォントサイズ
	//!	@retval	true	読み込み成功
	//!	@retval	false	読み込み失敗
	bool Load(const std::string& fileName, u32 size);

	//!	描画関数
	//!	@param	[in]	str		描画文字列
	//!	@param	[in]	x		座標
	//!	@param	[in]	y		座標
	void Render(const std::wstring& str, f32 x, f32 y);
	//!	描画関数
	//!	@param	[in]	str		描画文字列
	//!	@param	[in]	pos		座標
	void Render(const std::wstring& str, const Vector2& pos);
	//!	描画関数
	//!	@param	[in]	strs	描画文字列の配列
	//!	@param	[in]	x		座標
	//!	@param	[in]	y		座標
	//!	@param	[in]	padding	改行の空白
	void Render(const std::vector<std::wstring>& strs, f32 x, f32 y, f32 padding = 0);
	//!	描画関数
	//!	@param	[in]	strs	描画文字列の配列
	//!	@param	[in]	pos		座標
	//!	@param	[in]	padding	改行の空白
	void Render(const std::vector<std::wstring>& strs, const Vector2& pos, f32 padding = 0);

	//!	ファイル名取得
	std::string GetFileName()const
	{
		return _fileName;
	}
	f32 GetLineHeight()const
	{
		return _pFont->LineHeight();
	}

private:
	//void CreateChar(wchar_t charData);
	//void CreateTexture();
private:
	std::string		_fileName;	//!< ファイル名
	FTPixmapFont*	_pFont;		//!< フォントデータ

	//struct
	//{
	//	u32 _textureIndex = 0;			//!< FrameBufferのインデックス
	//	Vector2 _uv = Vector2::ZERO;	//!< テクスチャのUV
	//}_lastWriteTexture;	//!< 最後に書き込んだテクスチャの場所のデータ

	//std::vector< UniquePtr<FrameBuffer> > _pFrameBuffers;		//!< フォントテクスチャバッファ
	//std::array< UniquePtr<CharData>, WCHAR_MAX> _pCharDatas;	//!< 文字データ
};


//-------------------------------------------------------------------
//!	フォント操作オブジェクト
//-------------------------------------------------------------------
class FontHundler
{
	friend class FontManager;
public:
	//!	コンストラクタ
	FontHundler()
	:_pFontData(nullptr)
	{
	}
	//!	コンストラクタ
	//!	@param	[in]	pFontData	フォント
	FontHundler(FontData* pFontData)
	:_pFontData(pFontData)
	{
	}
	//!	デストラクタ
	virtual ~FontHundler()
	{
	}

	//!	描画関数
	//!	@param	[in]	str		描画文字列
	//!	@param	[in]	x		座標
	//!	@param	[in]	y		座標
	void Render(const std::wstring& str, f32 x, f32 y);
	//!	描画関数
	//!	@param	[in]	str		描画文字列
	//!	@param	[in]	pos		座標
	void Render(const std::wstring& str, const Vector2& pos);
	//!	描画関数
	//!	@param	[in]	strs	描画文字列の配列
	//!	@param	[in]	x		座標
	//!	@param	[in]	y		座標
	//!	@param	[in]	padding	改行の空白
	void Render(const std::vector<std::wstring>& strs, f32 x, f32 y, f32 padding = 0);
	//!	描画関数
	//!	@param	[in]	strs	描画文字列の配列
	//!	@param	[in]	pos		座標
	//!	@param	[in]	padding	改行の空白
	void Render(const std::vector<std::wstring>& strs, const Vector2& pos, f32 padding = 0);

	f32 GetLineHeight()const
	{
		return _pFontData ? _pFontData->GetLineHeight() : 0.f;
	}

	std::string GetFileName() const
	{
		return _pFontData ? _pFontData->GetFileName() : "";
	}
	operator bool()
	{
		return _pFontData != nullptr;
	}
private:

	FontData* _pFontData;
};

//-------------------------------------------------------------------
//!	フォント管理クラス
//-------------------------------------------------------------------
class FontManager : public ManagerBase
{
public:
	//!	コンストラクタ
	FontManager();
	//!	デストラクタ
	virtual ~FontManager();

	//!	終了
	void Exit()
	{
		ClearAll();
	}

	//!	解放
	//!	@param	[in]	fontHundler	フォント
	void Clear(FontHundler fontHundler);

	//!	すべて解放
	void ClearAll();

	//!	読み込み関数
	//!	@param	[in]	fileName	ファイル
	//!	@param	[in]	size		フォントサイズ	default 16
	//!	@return	フォントハンドラー
	FontHundler Load(const std::string& fileName, u32 size = 64);

private:
	std::unordered_map<std::string,FontData*>	_pFontDatas;	//!< フォントデータ

};


MANAGER_INTERFACE(FontManager, Font);



