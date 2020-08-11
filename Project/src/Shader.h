//=============================================================================
//!
//!	@file	Shader.h
//!	@brief	シェーダー管理クラス
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//!	シェーダー種類
enum class SHADER_TYPE
{
	VS,		//!< 頂点シェーダー
	FS,		//!< フラグメントシェーダー
	CS,		//!< コンピュートシェーダー
	NONE,
};

//!	シェーダープログラム種類
enum class PROGRAM_TYPE
{
	VS_FS,	//!< 頂点,フラグメント

	CS,		//!< コンピュート
	NONE,
};

//-------------------------------------------------------------------
//!	シェーダーオブジェクト
//-------------------------------------------------------------------
class ShaderObject
{
public:
	//!	コンストラクタ
	ShaderObject();
	//!	デストラクタ
	virtual ~ShaderObject();

	//!	解放
	void Clear();

	//! シェーダー読み込み関数
	//!	@param	[in]	filePath		ファイルディレクトリ文字列
	//!	@param	[in]	type			シェーダータイプ
	//!	@retval	true:成功	false:失敗
	bool		Load(const std::string& filePath, SHADER_TYPE type);

	//!	シェーダーの中身
	bool		IsEmpty() const		{ return _shaderId == 0; }
	//!	ファイル名
	std::string	GetFilePath() const	{ return _filePath; }
	//!	シェーダータイプ
	SHADER_TYPE	GetType() const		{ return _type; }
	//!	シェーダーID
	GLuint		GetShaderId() const	{ return _shaderId; }
private:
	std::string	_filePath;		//!< ファイルパス
	SHADER_TYPE	_type;			//!< シェーダータイプ
	GLuint		_shaderId;		//!< シェーダーID

};

//-------------------------------------------------------------------
//!	シェーダープログラム
//-------------------------------------------------------------------
class ShaderProgram
{
public:
	//!	コンストラクタ
	ShaderProgram();
	//!	デストラクタ
	virtual ~ShaderProgram();

	//!	解放
	void Clear();

	//!	シェーダーのコンパイル
	//!	@retval	true:成功	false:失敗
	bool Compile(ShaderObject* pObject, PROGRAM_TYPE type);
	bool Compile(ShaderObject* pObject1, ShaderObject* pObject2, PROGRAM_TYPE type);
	//!	シェーダー名設定
	void SetName(const std::string& name) { _name = name; }

	//!	シェーダープログラム取得
	u32 GetProgram() const { return _program; }
	//!	シェーダー名取得
	const std::string& GetName() const { return _name; }
	PROGRAM_TYPE GetProgramType()const { return _programType; }
private:
	std::string		_name;			//!< シェーダー名
	s32				_program;		//!< シェーダープログラム
	PROGRAM_TYPE	_programType;
};

//-------------------------------------------------------------------
//!	シェーダー操作用オブジェクト
//-------------------------------------------------------------------
class ShaderHundler
{
	friend class ShaderManager;
private:
	//!	コンストラクタ
	ShaderHundler(ShaderProgram* pProgram, ShaderObject* pShaderVS, ShaderObject* pShaderFS, ShaderObject* pShaderCS)
	: _pProgram (pProgram)
	, _pShaderVS(pShaderVS)
	, _pShaderFS(pShaderFS)
	, _pShaderCS(pShaderCS)
	{
	}

public:
	//!	コンストラクタ
	ShaderHundler()
	: _pProgram (nullptr)
	, _pShaderVS(nullptr)
	, _pShaderFS(nullptr)
	, _pShaderCS(nullptr)
	{
	}
	//!	デストラクタ
	virtual ~ShaderHundler(){};

	//!	保持チェック
	//!	@retval	true	保持していない
	bool IsEmpty()const
	{
		return _pProgram == nullptr;
	}

	//!	シェーダープログラムID取得
	//!	@return	プログラムID
	u32 GetProgram()const
	{
		if (!_pProgram){ return 0; }
		return _pProgram->GetProgram();
	}

	//!	シェーダーリビルド
	bool ReBuild();
	//!	シェーダー削除
	void Delete();
private:
	ShaderProgram* _pProgram;
	ShaderObject*  _pShaderVS;	//!< 頂点シェーダー
	ShaderObject*  _pShaderFS;	//!< フラグメントシェーダー
	ShaderObject*  _pShaderCS;	//!< コンピュートシェーダー
//	ShaderObject*  _pShaderGS;	//!< ジオメトリシェーダー
};

//-------------------------------------------------------------------
//!	シェーダー管理クラス
//-------------------------------------------------------------------
class ShaderManager : public ManagerBase
{
public:
	//!	コンストラクタ
	ShaderManager();
	//!	デストラクタ
	virtual ~ShaderManager();
	//!	解放
	void Exit()
	{
		Clear();
	}

	//!	データ削除
	void Clear();

	void DeleteShaderObject(ShaderObject*& pObject);
	void DeleteShaderProgram(ShaderProgram*& pProgram);


	//!	ロード
	//!	@return	プログラムインデックス
//	s32 Load(const std::string& shaderName, PROGRAM_TYPE programType);
	ShaderHundler Load(const std::string& shaderName, PROGRAM_TYPE programType);

	//!	番号取得
	//!	@param	[in]	shaderName	シェーダー名
	//!	@return	プログラムインデックス
	s32 SearchShader(const std::string& shaderName);

	//	シェーダー描画
	//!	開始
	//!	@param	[in]	index	プログラム番号
	void Begin(const ShaderHundler& shader);
	//!	終了
	void End();

	//!	一時停止
	void Pause();
	//!	再開
	void Play();
	
	//!	シェーダー数
	u32 GetShaderCount() const
	{
		return _pShaderList.size();
	}


	bool IsRunShader()
	{
		return (_nowRunningShader != 0);
	}

	//!	サブルーチン取得
	//!	@param	[in]	mode		シェーダーの種類
	//!	@param	[in]	name		関数名
	//!	@return	変数のロケーション	失敗時は-1
	s32 GetSubroutine(SHADER_TYPE type, const std::string& str);

	//!	サブルーチン設定
	//!	@param	[in]	mode		シェーダーの種類
	//!	@param	[in]	index		番号
	void SetSubroutine(SHADER_TYPE type, s32 index);

	//!	サブルーチン設定
	//!	@param	[in]	mode		シェーダーの種類
	//!	@param	[in]	str			関数名
	void SetSubroutine(SHADER_TYPE type, const std::string str)
	{
		SetSubroutine(type, GetSubroutine(type, str));
	}


	//!	Uniformロケーションの取得
	//!	@param	[in]	name		変数名
	//!	@return	変数のロケーション	-1なら失敗
	s32 GetUniformLocation(const std::string& name);

	//!	変数設定	s32
	//!	@param	[in]	name		変数名
	//!	@param	[in]	val			データ
	void SetUniform(const std::string& name, s32 val);
	//!	変数設定	f32
	//!	@param	[in]	name		変数名
	//!	@param	[in]	val			データ
	void SetUniform(const std::string& name, f32 val);
	//!	変数設定	bool
	//!	@param	[in]	name		変数名
	//!	@param	[in]	val			データ
	void SetUniform(const std::string& name, bool val);
	//!	変数設定	Vector2
	//!	@param	[in]	name		変数名
	//!	@param	[in]	val			データ
	void SetUniform(const std::string& name, const Vector2& val);
	//!	変数設定	Vector3
	//!	@param	[in]	name		変数名
	//!	@param	[in]	val			データ
	void SetUniform(const std::string& name, const Vector3& val);
	//!	変数設定	Vector4
	//!	@param	[in]	name		変数名
	//!	@param	[in]	val			データ
	void SetUniform(const std::string& name, const Vector4& val);
	//!	変数設定	Color
	//!	@param	[in]	name		変数名
	//!	@param	[in]	val			データ
	//!	@param	[in]	mode 		true RGBA false	RGB	default:true
	void SetUniform(const std::string& name, const Color& val, bool mode = true);
	//!	変数設定	Matrix
	//!	@param	[in]	name		変数名
	//!	@param	[in]	val			データ
	//!	@param	[in]	transpose	転置	default:false
	void SetUniform(const std::string& name, const Matrix& val, bool transpose = false);

	//!	変数設定	s32
	//!	@param	[in]	location	ロケーション番号
	//!	@param	[in]	val			データ
	void SetUniform(s32 location, s32 val);
	//!	変数設定	f32
	//!	@param	[in]	location	ロケーション番号
	//!	@param	[in]	val			データ
	void SetUniform(s32 location, f32 val);
	//!	変数設定	bool
	//!	@param	[in]	location	ロケーション番号
	//!	@param	[in]	val			データ
	void SetUniform(s32 location, bool val);
	//!	変数設定	Vector2
	//!	@param	[in]	location	ロケーション番号
	//!	@param	[in]	val			データ
	void SetUniform(s32 location, const Vector2& val);
	//!	変数設定	Vector3
	//!	@param	[in]	location	ロケーション番号
	//!	@param	[in]	val			データ
	void SetUniform(s32 location, const Vector3& val);
	//!	変数設定	Vector4
	//!	@param	[in]	location	ロケーション番号
	//!	@param	[in]	val			データ
	void SetUniform(s32 location, const Vector4& val);
	//!	変数設定	Color
	//!	@param	[in]	location	ロケーション番号
	//!	@param	[in]	val			データ
	//!	@param	[in]	mode		true RGBA false	RGB	default:true
	void SetUniform(s32 location, const Color& val, bool mode = true);
	//!	変数設定	Matrix
	//!	@param	[in]	location	ロケーション番号
	//!	@param	[in]	val			データ
	//!	@param	[in]	transpose	転置	default:false
	void SetUniform(s32 location, const Matrix& val, bool transpose = false);

	//!	変数設定	Matrix
	//!	@param	[in]	location	ロケーション番号
	//!	@param	[in]	val			データ
	//!	@param	[in]	size		バッファ数
	//!	@param	[in]	transpose	転置	default:false
	void SetUniforms(s32 location, const Matrix* val, u32 size,bool transpose = false);

private:

	//!	頂点,フラグメントロード
	//!	@param	[in]	filePath	ファイルパス
	//!	@return	シェーダーID
	ShaderHundler LoadVSFS(const std::string& shaderName);

	//!	コンピュートロード
	//!	@param	[in]	filePath	ファイルパス
	//!	@return	シェーダーID
	ShaderHundler LoadCS(const std::string& shaderName);


	//	オブジェクトロード

	//!	頂点シェーダーロード
	//!	@param	[in]	filePath	ファイルパス
	//!	@return	シェーダーオブジェクト
	ShaderObject* LoadObjectVS(const std::string& filePath);
	//!	フラグメントシェーダーロード
	//!	@param	[in]	filePath	ファイルパス
	//!	@return	シェーダーオブジェクト
	ShaderObject* LoadObjectFS(const std::string& filePath);
	//!	コンピュートシェーダーロード
	//!	@param	[in]	filePath	ファイルパス
	//!	@return	シェーダーオブジェクト
	ShaderObject* LoadObjectCS(const std::string& filePath);

private:
	std::list<ShaderProgram*>	_pShaderList;	//!< シェーダープログラム
	std::list<ShaderObject*>	_pVsList;		//!< 頂点シェーダー
	std::list<ShaderObject*>	_pFsList;		//!< フラグメントシェーダー
	std::list<ShaderObject*>	_pCsList;		//!< フラグメントシェーダー

	u32 _nowRunningShader;
};


MANAGER_INTERFACE(ShaderManager, Shader);
