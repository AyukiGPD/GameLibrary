//=============================================================================
//!
//!	@file	Shader.cpp
//!	@brief	シェーダー管理クラス
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
ShaderObject::ShaderObject()
:_type(SHADER_TYPE::NONE)
,_shaderId(0)
{
}

//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
ShaderObject::~ShaderObject()
{
	Clear();
}

//-----------------------------------------------------------------------------
//!	解放
//-----------------------------------------------------------------------------
void ShaderObject::Clear()
{
	if(_shaderId != 0)glDeleteShader(_shaderId);
}


//-----------------------------------------------------------------------------
//! シェーダー読み込み関数
//!	@param	[in]	filePath		ファイルディレクトリ文字列
//!	@param	[in]	type			シェーダータイプ
//!	@retval	true:成功	false:失敗
//-----------------------------------------------------------------------------
bool ShaderObject::Load(const std::string& filePath, SHADER_TYPE type)
{
	if(type == SHADER_TYPE::NONE){ return false; }
	DEBUG_LOG(filePath);
	//------------------------------------------------------------------
	//	(1)シェーダーファイルを開いてソースを読み込む
	//------------------------------------------------------------------
	//const char* fullPath = filePath.c_str(); 
	//const char* pExt = strstr(fullPath, ".");
	//FILE* fp;
	//errno_t err = fopen_s(&fp,filePath.c_str(),"rb");
	//if(err != 0)
	//{
	//	ERROR_LOG("File Not Open:" << filePath);
	//	return false;
	//}

	////	ファイルサイズを取得
	//fseek(fp,0,SEEK_END);	//	一番最後までシークして
	//s32 size = ftell(fp);	//	サイズを取得
	//char* buf = new char[size];
	//fseek(fp,0,SEEK_SET);	//	先頭に戻す
	//fread(buf,1,size,fp);	//	サイズ分読み込む
	//fclose(fp);				//	ファイルを閉じる

	std::ifstream                  ifs(filePath);
	if(!ifs.is_open())
	{
		ifs.close();
		return false;
	}
	std::istreambuf_iterator<char> ifsBegin(ifs);
	std::istreambuf_iterator<char> ifsEnd;
	std::string                    fileData(ifsBegin, ifsEnd);
	ifs.close();


	GLuint shaderType;
	switch(type)
	{
	case SHADER_TYPE::VS:
		shaderType = GL_VERTEX_SHADER;
		break;
	case SHADER_TYPE::FS:
		shaderType = GL_FRAGMENT_SHADER;
		break;
	case SHADER_TYPE::CS:
		shaderType = GL_COMPUTE_SHADER;
		break;
	default:
		ERROR_LOG(L"Type Error")
		return false;
	}

	//	オブジェクト取得
	//	サイズ分のメモリ確保
	_shaderId = glCreateShader(shaderType);
	//	プログラムセット
	const char* source = fileData.c_str();
	glShaderSource(_shaderId,1,&source, nullptr);

	//	メモリ解放
	//SafeDeleteArray(buf);

	//------------------------------------------------------------------
	//	(2)ソースをコンパイル
	//------------------------------------------------------------------
	//	読み込んだソースをコンパイル
	glCompileShader(_shaderId);
	//	コンパイルの結果を取得
	s32 compile;
	glGetShaderiv(_shaderId,GL_COMPILE_STATUS,&compile);
	//	失敗した場合
	if(compile == GL_FALSE)
	{
		s32 size;
		glGetShaderiv( _shaderId, GL_INFO_LOG_LENGTH, &size );
		//	エラーメッセージ表示
		if( size > 0 )
		{
			s32 len;						//	ログの長さ
			char* log = new char[size];		//	ログの内容を格納する配列
			glGetShaderInfoLog( _shaderId, size, &len, log );	//	ログ取得
			ERROR_LOG("CompileError" << filePath << ":" << log);
			SafeDeleteArray(log);
		}
		return false;
	}

	_type = type;
	return true;
}

//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
ShaderProgram::ShaderProgram()
: _program(-1)
, _programType(PROGRAM_TYPE::NONE)
{
}
//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
ShaderProgram::~ShaderProgram()
{
	Clear();
}

//-----------------------------------------------------------------------------
//!	解放
//-----------------------------------------------------------------------------
void ShaderProgram::Clear()
{
	glDeleteProgram(_program);
}

//-----------------------------------------------------------------------------
//!	シェーダーのコンパイル
//!	@retval	true:成功	false:失敗
//-----------------------------------------------------------------------------
bool ShaderProgram::Compile(ShaderObject* pObject, PROGRAM_TYPE type)
{
	if (!pObject)
	{
		return false;
	}
	if (type != PROGRAM_TYPE::CS)
	{
		return false;
	}

	_programType = type;
	bool result = true;
	//	プログラムオブジェクトの生成
	_program = glCreateProgram();
	//	オブジェクトを関連付ける
	glAttachShader(_program, pObject->GetShaderId());

	//	シェーダープログラムをリンク
	glLinkProgram(_program);

	//	リンクの確認
	s32 link;
	glGetProgramiv(_program, GL_LINK_STATUS, &link);
	if (link == GL_FALSE)
	{
		//	失敗した場合
		s32 size = 0;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &size);
		if (size > 0)
		{
			//	エラーメッセージ
			s32 len;						//	ログの長さ
			char* log = new char[size];		//	ログの内容を格納する配列
			glGetProgramInfoLog(_program, size, &len, log);	//	ログ取得
			ERROR_LOG("LinkError"
				<< pObject->GetFilePath() << std::endl << log);
			SafeDeleteArray(log);
		}
		result = false;
		_programType = PROGRAM_TYPE::NONE;
	}
	return result;
}

bool ShaderProgram::Compile(ShaderObject* pObject1, ShaderObject* pObject2, PROGRAM_TYPE type)
{
	if (!pObject1|| !pObject2)
	{
		return false;
	}

	if (type != PROGRAM_TYPE::VS_FS)
	{
		return false;
	}

	_programType = type;
	bool result = true;
	//	プログラムオブジェクトの生成
	_program = glCreateProgram();
	//	オブジェクトを関連付ける
	glAttachShader(_program, pObject1->GetShaderId());
	glAttachShader(_program, pObject2->GetShaderId());

	//	シェーダープログラムをリンク
	glLinkProgram(_program);

	//	リンクの確認
	s32 link;
	glGetProgramiv(_program,GL_LINK_STATUS,&link);
	if(link == GL_FALSE)
	{
		//	失敗した場合
		s32 size = 0;
		glGetProgramiv( _program, GL_INFO_LOG_LENGTH, &size );
		if( size > 0 )
		{
			//	エラーメッセージ
			s32 len;						//	ログの長さ
			char* log = new char[size];		//	ログの内容を格納する配列
			glGetProgramInfoLog( _program, size, &len, log );	//	ログ取得
			ERROR_LOG("LinkError"
				<< pObject1->GetFilePath() << ":"
				<< pObject2->GetFilePath() << std::endl << log);
			SafeDeleteArray(log);
		}
		result = false;
		_programType = PROGRAM_TYPE::NONE;
	}
	return result;
}

bool ShaderHundler::ReBuild()
{
	if(!_pProgram){ return false; }
	std::string fileName = _pProgram->GetName();
	PROGRAM_TYPE type = _pProgram->GetProgramType();
	Delete();
	*this = Manager::Shader()->Load(fileName, type);
	return !IsEmpty();
}

void ShaderHundler::Delete()
{
	auto* pShader = Manager::Shader();
	pShader->DeleteShaderObject(_pShaderVS);
	pShader->DeleteShaderObject(_pShaderFS);
	pShader->DeleteShaderObject(_pShaderCS);
	pShader->DeleteShaderProgram(_pProgram);
}


//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
ShaderManager::ShaderManager()
{
	_nowRunningShader = 0;
}
//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
ShaderManager::~ShaderManager()
{
}
//-----------------------------------------------------------------------------
//!	データ削除
//-----------------------------------------------------------------------------
void ShaderManager::Clear()
{
	SafeDeleteArray(_pShaderList);
	SafeDeleteArray(_pFsList);
	SafeDeleteArray(_pVsList);
	SafeDeleteArray(_pCsList);
}

void ShaderManager::DeleteShaderObject(ShaderObject*& pObject)
{
	if (!pObject)return;

	switch (pObject->GetType())
	{
	case SHADER_TYPE::VS:
		{
			auto itr = _pVsList.begin();
			for (; itr != _pVsList.end(); ++itr)
			{
				ShaderObject* p = (*itr);
				if (p != pObject){ continue; }
				_pVsList.erase(itr);
				break;
			}
		}
		break;
	case SHADER_TYPE::FS:
		{
			auto itr = _pFsList.begin();
			for (; itr != _pFsList.end(); ++itr)
			{
				ShaderObject* p = (*itr);
				if (p != pObject){ continue; }
				_pFsList.erase(itr);
				break;
			}
		}
		break;
	case SHADER_TYPE::CS:
		{
			auto itr = _pCsList.begin();
			for (; itr != _pCsList.end(); ++itr)
			{
				ShaderObject* p = (*itr);
				if (p != pObject){ continue; }
				_pCsList.erase(itr);
				break;
			}
		}
		break;
	default:
		return;
		break;
	}

	SafeDelete(pObject);

}

void ShaderManager::DeleteShaderProgram(ShaderProgram*& pProgram)
{
	if(!pProgram){ return; }
	auto itr = _pShaderList.begin();
	for (; itr != _pShaderList.end(); ++itr)
	{
		ShaderProgram* p = (*itr);
		if (p != pProgram){ continue; }
		_pShaderList.erase(itr);
		SafeDelete(pProgram);
		break;
	}

}

//-----------------------------------------------------------------------------
//!	ロード
//!	@return	プログラムインデックス
//-----------------------------------------------------------------------------
//s32 ShaderManager::Load(const std::string& shaderName, PROGRAM_TYPE programType)
ShaderHundler ShaderManager::Load(const std::string& shaderName, PROGRAM_TYPE programType)
{
	ShaderHundler result;
	switch (programType)
	{
	case PROGRAM_TYPE::VS_FS:
		result = LoadVSFS(shaderName);
		break;
	case PROGRAM_TYPE::CS:
		result = LoadCS(shaderName);
		break;
	default:
		ERROR_LOG("ProgramMode Unknown");
		break;
	}
	return result;
}

void ShaderManager::Begin(const ShaderHundler& shader)
{
	RenderManager* pRenderer = Manager::Renderer();
	GLuint program = 0;
	program = shader.GetProgram();

	if (program == 0)
	{
		DEBUG_LOG(L"Shader Not Run");
		pRenderer->DisableShader();
		return;
	}
	_nowRunningShader = program;
	glUseProgram(program);
	pRenderer->EnableShader();

}

//-----------------------------------------------------------------------------
//!	終了
//-----------------------------------------------------------------------------
void ShaderManager::End()
{
	_nowRunningShader = 0;
	glUseProgram(0);
	Manager::Renderer()->DisableShader();
}

//-----------------------------------------------------------------------------
//!	一時停止
//-----------------------------------------------------------------------------
void ShaderManager::Pause()
{
	glUseProgram(0);
	Manager::Renderer()->DisableShader();
}

//-----------------------------------------------------------------------------
//!	再開
//-----------------------------------------------------------------------------
void ShaderManager::Play()
{
	glUseProgram(_nowRunningShader);
	if (_nowRunningShader != 0)
	{
		Manager::Renderer()->EnableShader();
	}
}

//-----------------------------------------------------------------------------
//!	サブルーチン取得
//!	@param	[in]	mode		シェーダーの種類
//!	@param	[in]	name		関数名
//!	@return	変数のロケーション	失敗時は-1
//-----------------------------------------------------------------------------
s32 ShaderManager::GetSubroutine(SHADER_TYPE type, const std::string& str)
{
	if (_nowRunningShader == 0){ return -1; }
	u32 shaderType = 0;
	switch (type)
	{
	case SHADER_TYPE::VS:
		shaderType = GL_VERTEX_SHADER;
		break;
	case SHADER_TYPE::FS:
		shaderType = GL_FRAGMENT_SHADER;
		break;
	default:
		return -1;
	}
	s32 result = glGetSubroutineIndex( _nowRunningShader, shaderType, str.c_str());
	return result;
}

//-----------------------------------------------------------------------------
//!	サブルーチン設定
//!	@param	[in]	mode		シェーダーの種類
//!	@param	[in]	index		番号
//-----------------------------------------------------------------------------
void ShaderManager::SetSubroutine(SHADER_TYPE type, s32 index)
{
	if (index < 0){ return; }
	u32 shaderType = 0;
	switch (type)
	{
	case SHADER_TYPE::VS:
		shaderType = GL_VERTEX_SHADER;
		break;
	case SHADER_TYPE::FS:
		shaderType = GL_FRAGMENT_SHADER;
		break;
	default:
		return;
	}
	u32 i = (u32)index;
	glUniformSubroutinesuiv(shaderType, 1, &i);
}


//-----------------------------------------------------------------------------
//!	Uniformロケーションの取得
//!	@param	[in]	name		変数名
//!	@return	変数のロケーション	-1なら失敗
//-----------------------------------------------------------------------------
s32 ShaderManager::GetUniformLocation(const std::string& name)
{
	if (_nowRunningShader == 0){ return -1; }
	s32 location = glGetUniformLocation(_nowRunningShader, name.c_str());
	return location;
}

//-----------------------------------------------------------------------------
//!	変数設定	s32
//!	@param	[in]	name	変数名
//!	@param	[in]	val		データ
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(const std::string& name, s32 val)
{
	if (_nowRunningShader == 0){ return; }
	s32 location = glGetUniformLocation(_nowRunningShader, name.c_str());
	if (location < 0){ return; }
	glUniform1i(location, val);
}
//-----------------------------------------------------------------------------
//!	変数設定	f32
//!	@param	[in]	name	変数名
//!	@param	[in]	val		データ
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(const std::string& name, f32 val)
{
	if (_nowRunningShader == 0){ return; }
	s32 location = glGetUniformLocation(_nowRunningShader, name.c_str());
	if (location < 0){ return; }
	glUniform1f(location, val);
}
//-----------------------------------------------------------------------------
//!	変数設定	bool
//!	@param	[in]	location	ロケーション番号
//!	@param	[in]	val			データ
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(const std::string& name, bool val)
{
	if (_nowRunningShader == 0){ return; }
	s32 location = glGetUniformLocation(_nowRunningShader, name.c_str());
	if (location < 0){ return; }
	BOOL boolean = val ? TRUE : FALSE;
	glUniform1f(location, (f32)boolean);
}

//-----------------------------------------------------------------------------
//!	変数設定	Vector2
//!	@param	[in]	name	変数名
//!	@param	[in]	val		データ
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(const std::string& name, const Vector2& val)
{
	if (_nowRunningShader == 0){ return; }
	s32 location = glGetUniformLocation(_nowRunningShader, name.c_str());
	if (location < 0){ return; }
	glUniform2fv(location, 1, (f32*)&val);
}
//-----------------------------------------------------------------------------
//!	変数設定	Vector3
//!	@param	[in]	name	変数名
//!	@param	[in]	val		データ
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(const std::string& name, const Vector3& val)
{
	if (_nowRunningShader == 0){ return; }
	s32 location = glGetUniformLocation(_nowRunningShader, name.c_str());
	if (location < 0){ return; }
	glUniform3fv(location, 1, (f32*)&val);
}
//-----------------------------------------------------------------------------
//!	変数設定	Vector4
//!	@param	[in]	name	変数名
//!	@param	[in]	val		データ
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(const std::string& name, const Vector4& val)
{
	if (_nowRunningShader == 0){ return; }
	s32 location = glGetUniformLocation(_nowRunningShader, name.c_str());
	if (location < 0){ return; }
	glUniform4fv(location, 1, (f32*)&val);
}
//-----------------------------------------------------------------------------
//!	変数設定	Color
//!	@param	[in]	name	変数名
//!	@param	[in]	val		データ
//!	@param	[in]	mode 	true RGBA false	RGB	default:true
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(const std::string& name, const Color& val, bool mode)
{
	if (_nowRunningShader == 0){ return; }
	s32 location = glGetUniformLocation(_nowRunningShader, name.c_str());
	if (location < 0){ return; }
	if (mode)
	{
		//	RGBA
		Vector4 colF = val.GetColorF();
		glUniform4fv(location, 1, (f32*)&colF);
	}
	else
	{
		//	RGB
		Vector3 colF = val.GetColorFRGB();
		glUniform3fv(location, 1, (f32*)&colF);
	}
}
//-----------------------------------------------------------------------------
//!	変数設定	Matrix
//!	@param	[in]	name	変数名
//!	@param	[in]	val		データ
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(const std::string& name, const Matrix& val, bool transpose)
{
	if (_nowRunningShader == 0){ return; }
	s32 location = glGetUniformLocation(_nowRunningShader, name.c_str());
	if (location < 0){ return; }
	s8 t = transpose ? GL_TRUE : GL_FALSE;
	glUniformMatrix4fv(location, 1, t, (f32*)&val);
}


//-----------------------------------------------------------------------------
//!	変数設定	s32
//!	@param	[in]	location	ロケーション番号
//!	@param	[in]	val			データ
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(s32 location, s32 val)
{
	if (_nowRunningShader == 0){ return; }
	if (location < 0){ return; }
	glUniform1i(location, val);
	isErrorGL();
}
//-----------------------------------------------------------------------------
//!	変数設定	f32
//!	@param	[in]	location	ロケーション番号
//!	@param	[in]	val			データ
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(s32 location, f32 val)
{
	if (_nowRunningShader == 0){ return; }
	if (location < 0){ return; }
	glUniform1f(location, val);
	isErrorGL();
}
//-----------------------------------------------------------------------------
//!	変数設定	bool
//!	@param	[in]	location	ロケーション番号
//!	@param	[in]	val			データ
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(s32 location, bool val)
{
	if (_nowRunningShader == 0){ return; }
	if (location < 0){ return; }
	s32 boolean = val ? 1 : 0;
	glUniform1i(location, boolean);
	isErrorGL();
}
//-----------------------------------------------------------------------------
//!	変数設定	Vector2
//!	@param	[in]	location	ロケーション番号
//!	@param	[in]	val			データ
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(s32 location, const Vector2& val)
{
	if (_nowRunningShader == 0){ return; }
	if (location < 0){ return; }
	glUniform2fv(location, 1, (f32*)&val);
	isErrorGL();
}
//-----------------------------------------------------------------------------
//!	変数設定	Vector3
//!	@param	[in]	location	ロケーション番号
//!	@param	[in]	val			データ
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(s32 location, const Vector3& val)
{
	if (_nowRunningShader == 0){ return; }
	if (location < 0){ return; }
	glUniform3fv(location, 1, (f32*)&val);
	isErrorGL();
}
//-----------------------------------------------------------------------------
//!	変数設定	Vector4
//!	@param	[in]	location	ロケーション番号
//!	@param	[in]	val			データ
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(s32 location, const Vector4& val)
{
	if (_nowRunningShader == 0){ return; }
	if (location < 0){ return; }
	glUniform4fv(location, 1, (f32*)&val);
	isErrorGL();
}
//-----------------------------------------------------------------------------
//!	変数設定	Color
//!	@param	[in]	location	ロケーション番号
//!	@param	[in]	val			データ
//!	@param	[in]	mode		true RGBA false	RGB	default:true
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(s32 location, const Color& val, bool mode)
{
	if (_nowRunningShader == 0){ return; }
	if (location < 0){ return; }
	if (mode)
	{
		//	RGBA
		Vector4 colF = val.GetColorF();
		glUniform4fv(location, 1, (f32*)&colF);
		isErrorGL();
	}
	else
	{
		//	RGB
		Vector3 colF = val.GetColorFRGB();
		glUniform3fv(location, 1, (f32*)&colF);
		isErrorGL();
	}
}
//-----------------------------------------------------------------------------
//!	変数設定	Matrix
//!	@param	[in]	location	ロケーション番号
//!	@param	[in]	val			データ
//!	@param	[in]	transpose	転置	default:false
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(s32 location, const Matrix& val, bool transpose)
{
	if (_nowRunningShader == 0){ return; }
	if (location < 0){ return; }
	s8 t = transpose ? GL_TRUE : GL_FALSE;
	glUniformMatrix4fv(location, 1, t, (f32*)&val);
	isErrorGL();
}


//!	変数設定	Matrix
//!	@param	[in]	location	ロケーション番号
//!	@param	[in]	val			データ
//!	@param	[in]	size		バッファ数
//!	@param	[in]	transpose	転置	default:false
void ShaderManager::SetUniforms(s32 location, const Matrix* val, u32 size, bool transpose)
{
	if(_nowRunningShader == 0){ return; }
	if(location < 0){ return; }
	s8 t = transpose ? GL_TRUE : GL_FALSE;
	glUniformMatrix4fv(location, size, t, (f32*)&val);
	isErrorGL();
}


//-----------------------------------------------------------------------------
//!	頂点,フラグメントロード
//!	@param	[in]	filePath	ファイルパス
//!	@return	シェーダーID
//-----------------------------------------------------------------------------
ShaderHundler ShaderManager::LoadVSFS(const std::string& shaderName)
{
	// 各シェーダを読み込む
	ShaderObject* pVS = LoadObjectVS("shader/" + shaderName + ".vert");
	ShaderObject* pFS = LoadObjectFS("shader/" + shaderName + ".frag");

	ShaderProgram* pProgram = nullptr;

	// 一つでも失敗している場合はエラー
	if (!pVS || !pFS)
	{
		DeleteShaderObject(pVS);
		SafeDelete(pVS);
		DeleteShaderObject(pFS);
		SafeDelete(pFS);
		return ShaderHundler(nullptr, nullptr, nullptr, nullptr);
	}

	// コンパイル
	for (auto* p : _pShaderList)
	{
		const std::string& name = p->GetName();
		if (shaderName == name)
		{
			pProgram = p;
			break;
		}
	}

	if (!pProgram)
	{
		pProgram = new ShaderProgram();
		const bool isCompile = pProgram->Compile(pVS, pFS, PROGRAM_TYPE::VS_FS);
		if (isCompile)
		{
			pProgram->SetName(shaderName);
			_pShaderList.push_back(pProgram);
		}
		else
		{
			DeleteShaderObject(pVS);
			SafeDelete(pVS);
			DeleteShaderObject(pFS);
			SafeDelete(pFS);
			SafeDelete(pProgram);
//			return -1;
			return ShaderHundler(nullptr, nullptr, nullptr, nullptr);
		}
	}



	return ShaderHundler(pProgram, pVS, pFS, nullptr);
}

//-----------------------------------------------------------------------------
//!	コンピュートロード
//!	@param	[in]	filePath	ファイルパス
//!	@return	シェーダーID
//-----------------------------------------------------------------------------
ShaderHundler ShaderManager::LoadCS(const std::string& shaderName)
{
	// 各シェーダを読み込む
	ShaderObject* pCS = LoadObjectCS("shader/" + shaderName + ".comp");
	ShaderProgram* pProgram = nullptr;

	// 一つでも失敗している場合はエラー
	if (!pCS)
	{
		SafeDelete(pCS);
//		return -1;
		return ShaderHundler(nullptr, nullptr, nullptr, nullptr);
	}

	// コンパイル
	for (auto* p : _pShaderList)
	{
		const std::string& name = p->GetName();
		if (shaderName == name)
		{
			pProgram = p;
			break;
		}
	}

	if (!pProgram)
	{
		pProgram = new ShaderProgram();
		const bool isCompile = pProgram->Compile(pCS, PROGRAM_TYPE::CS);
		if (isCompile)
		{
			pProgram->SetName(shaderName);
			_pShaderList.push_back(pProgram);
		}
		else
		{
			SafeDelete(pCS);
			SafeDelete(pProgram);
//			return -1;
			return ShaderHundler(nullptr, nullptr, nullptr, nullptr);
		}
	}

	return ShaderHundler(pProgram, nullptr, nullptr, pCS);
}

//-----------------------------------------------------------------------------
//!	頂点シェーダーロード
//!	@param	[in]	filePath		ファイルディレクトリ文字列
//!	@return	シェーダーオブジェクト
//-----------------------------------------------------------------------------
ShaderObject* ShaderManager::LoadObjectVS(const std::string& filePath)
{
	ShaderObject* pShader = nullptr;

	// すでに読み込んでいる場合はリストから取得する
	for (auto* pVS : _pVsList)
	{
		const std::string& fileName = pVS->GetFilePath();
		if (fileName == filePath)
		{
			pShader = pVS;
			break;
		}
	}

	// リスト内に存在しない場合は新規作成する
	if( pShader == nullptr )
	{
		pShader = new ShaderObject();
		if( pShader )
		{
			const bool isLoad = pShader->Load(filePath, SHADER_TYPE::VS);
			if( isLoad )
			{
				_pVsList.push_back(pShader);
			}
			else
			{
				SafeDelete(pShader);
			}
		}
	}
	return pShader;
}

//-----------------------------------------------------------------------------
//!	フラグメントシェーダーロード
//!	@param	[in]	filePath		ファイルディレクトリ文字列
//!	@return	シェーダーオブジェクト
//-----------------------------------------------------------------------------
ShaderObject* ShaderManager::LoadObjectFS(const std::string& filePath)
{
	ShaderObject* pShader = nullptr;

	// すでに読み込んでいる場合はリストから取得する
	for (auto* pFS : _pVsList)
	{
		const std::string& fileName = pFS->GetFilePath();
		if (fileName == filePath)
		{
			pShader = pFS;
			break;
		}
	}

	// リスト内に存在しない場合は新規作成する
	if( pShader == nullptr )
	{
		pShader = new ShaderObject();
		if( pShader )
		{
			const bool isLoad = pShader->Load(filePath, SHADER_TYPE::FS);
			if( isLoad )
			{
				_pFsList.push_back(pShader);
			}
			else
			{
				SafeDelete(pShader);
			}
		}
	}
	return pShader;
}

//-----------------------------------------------------------------------------
//!	コンピュートシェーダーロード
//!	@param	[in]	filePath	ファイルパス
//!	@return	シェーダーオブジェクト
//-----------------------------------------------------------------------------
ShaderObject* ShaderManager::LoadObjectCS(const std::string& filePath)
{
	ShaderObject* pShader = nullptr;

	// すでに読み込んでいる場合はリストから取得する
	for (auto* pCS : _pCsList)
	{
		const std::string& fileName = pCS->GetFilePath();
		if (fileName == filePath)
		{
			pShader = pCS;
			break;
		}
	}
	// リスト内に存在しない場合は新規作成する
	if (pShader == nullptr)
	{
		pShader = new ShaderObject();
		if (pShader)
		{
			const bool isLoad = pShader->Load(filePath, SHADER_TYPE::CS);
			if (isLoad)
			{
				_pCsList.push_back(pShader);
			}
			else
			{
				SafeDelete(pShader);
			}
		}
	}
	return pShader;
}

