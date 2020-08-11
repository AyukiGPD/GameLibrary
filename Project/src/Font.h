//=============================================================================
//!
//!	@file	Font.h
//!	@brief	�t�H���g
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
	�t�H���g�f�[�^�Ńe�N�X�`���Ńo�b�t�@�����
*/
//-------------------------------------------------------------------
//!	�t�H���g�f�[�^
//-------------------------------------------------------------------
class FontData
{
public:
	//!	�R���X�g���N�^
	FontData();
	//!	�R���X�g���N�^
	//!	@param	[in]	fileName	�t�@�C��
	//!	@param	[in]	size		�t�H���g�T�C�Y
	FontData(const std::string& fileName, u32 size);
	//!	�f�X�g���N�^
	virtual ~FontData();

	//!	���
	void Clear();

	//!	���g�̃`�F�b�N
	bool IsEmpty()
	{
		return !_pFont;
	}

	//!	�t�H���g�T�C�Y�ݒ�
	//!	@param	[in]	size	�����T�C�Y
	void SetSize(u32 size);

	//	�ǂݍ��݊֐�
	//!	@param	[in]	fileName	�t�@�C��
	//!	@param	[in]	size		�t�H���g�T�C�Y
	//!	@retval	true	�ǂݍ��ݐ���
	//!	@retval	false	�ǂݍ��ݎ��s
	bool Load(const std::string& fileName, u32 size);

	//!	�`��֐�
	//!	@param	[in]	str		�`�敶����
	//!	@param	[in]	x		���W
	//!	@param	[in]	y		���W
	void Render(const std::wstring& str, f32 x, f32 y);
	//!	�`��֐�
	//!	@param	[in]	str		�`�敶����
	//!	@param	[in]	pos		���W
	void Render(const std::wstring& str, const Vector2& pos);
	//!	�`��֐�
	//!	@param	[in]	strs	�`�敶����̔z��
	//!	@param	[in]	x		���W
	//!	@param	[in]	y		���W
	//!	@param	[in]	padding	���s�̋�
	void Render(const std::vector<std::wstring>& strs, f32 x, f32 y, f32 padding = 0);
	//!	�`��֐�
	//!	@param	[in]	strs	�`�敶����̔z��
	//!	@param	[in]	pos		���W
	//!	@param	[in]	padding	���s�̋�
	void Render(const std::vector<std::wstring>& strs, const Vector2& pos, f32 padding = 0);

	//!	�t�@�C�����擾
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
	std::string		_fileName;	//!< �t�@�C����
	FTPixmapFont*	_pFont;		//!< �t�H���g�f�[�^

	//struct
	//{
	//	u32 _textureIndex = 0;			//!< FrameBuffer�̃C���f�b�N�X
	//	Vector2 _uv = Vector2::ZERO;	//!< �e�N�X�`����UV
	//}_lastWriteTexture;	//!< �Ō�ɏ������񂾃e�N�X�`���̏ꏊ�̃f�[�^

	//std::vector< UniquePtr<FrameBuffer> > _pFrameBuffers;		//!< �t�H���g�e�N�X�`���o�b�t�@
	//std::array< UniquePtr<CharData>, WCHAR_MAX> _pCharDatas;	//!< �����f�[�^
};


//-------------------------------------------------------------------
//!	�t�H���g����I�u�W�F�N�g
//-------------------------------------------------------------------
class FontHundler
{
	friend class FontManager;
public:
	//!	�R���X�g���N�^
	FontHundler()
	:_pFontData(nullptr)
	{
	}
	//!	�R���X�g���N�^
	//!	@param	[in]	pFontData	�t�H���g
	FontHundler(FontData* pFontData)
	:_pFontData(pFontData)
	{
	}
	//!	�f�X�g���N�^
	virtual ~FontHundler()
	{
	}

	//!	�`��֐�
	//!	@param	[in]	str		�`�敶����
	//!	@param	[in]	x		���W
	//!	@param	[in]	y		���W
	void Render(const std::wstring& str, f32 x, f32 y);
	//!	�`��֐�
	//!	@param	[in]	str		�`�敶����
	//!	@param	[in]	pos		���W
	void Render(const std::wstring& str, const Vector2& pos);
	//!	�`��֐�
	//!	@param	[in]	strs	�`�敶����̔z��
	//!	@param	[in]	x		���W
	//!	@param	[in]	y		���W
	//!	@param	[in]	padding	���s�̋�
	void Render(const std::vector<std::wstring>& strs, f32 x, f32 y, f32 padding = 0);
	//!	�`��֐�
	//!	@param	[in]	strs	�`�敶����̔z��
	//!	@param	[in]	pos		���W
	//!	@param	[in]	padding	���s�̋�
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
//!	�t�H���g�Ǘ��N���X
//-------------------------------------------------------------------
class FontManager : public ManagerBase
{
public:
	//!	�R���X�g���N�^
	FontManager();
	//!	�f�X�g���N�^
	virtual ~FontManager();

	//!	�I��
	void Exit()
	{
		ClearAll();
	}

	//!	���
	//!	@param	[in]	fontHundler	�t�H���g
	void Clear(FontHundler fontHundler);

	//!	���ׂĉ��
	void ClearAll();

	//!	�ǂݍ��݊֐�
	//!	@param	[in]	fileName	�t�@�C��
	//!	@param	[in]	size		�t�H���g�T�C�Y	default 16
	//!	@return	�t�H���g�n���h���[
	FontHundler Load(const std::string& fileName, u32 size = 64);

private:
	std::unordered_map<std::string,FontData*>	_pFontDatas;	//!< �t�H���g�f�[�^

};


MANAGER_INTERFACE(FontManager, Font);



