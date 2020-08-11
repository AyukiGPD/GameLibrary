//=============================================================================
//!
//!	@file	Shader.cpp
//!	@brief	�V�F�[�_�[�Ǘ��N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
ShaderObject::ShaderObject()
:_type(SHADER_TYPE::NONE)
,_shaderId(0)
{
}

//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
ShaderObject::~ShaderObject()
{
	Clear();
}

//-----------------------------------------------------------------------------
//!	���
//-----------------------------------------------------------------------------
void ShaderObject::Clear()
{
	if(_shaderId != 0)glDeleteShader(_shaderId);
}


//-----------------------------------------------------------------------------
//! �V�F�[�_�[�ǂݍ��݊֐�
//!	@param	[in]	filePath		�t�@�C���f�B���N�g��������
//!	@param	[in]	type			�V�F�[�_�[�^�C�v
//!	@retval	true:����	false:���s
//-----------------------------------------------------------------------------
bool ShaderObject::Load(const std::string& filePath, SHADER_TYPE type)
{
	if(type == SHADER_TYPE::NONE){ return false; }
	DEBUG_LOG(filePath);
	//------------------------------------------------------------------
	//	(1)�V�F�[�_�[�t�@�C�����J���ă\�[�X��ǂݍ���
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

	////	�t�@�C���T�C�Y���擾
	//fseek(fp,0,SEEK_END);	//	��ԍŌ�܂ŃV�[�N����
	//s32 size = ftell(fp);	//	�T�C�Y���擾
	//char* buf = new char[size];
	//fseek(fp,0,SEEK_SET);	//	�擪�ɖ߂�
	//fread(buf,1,size,fp);	//	�T�C�Y���ǂݍ���
	//fclose(fp);				//	�t�@�C�������

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

	//	�I�u�W�F�N�g�擾
	//	�T�C�Y���̃������m��
	_shaderId = glCreateShader(shaderType);
	//	�v���O�����Z�b�g
	const char* source = fileData.c_str();
	glShaderSource(_shaderId,1,&source, nullptr);

	//	���������
	//SafeDeleteArray(buf);

	//------------------------------------------------------------------
	//	(2)�\�[�X���R���p�C��
	//------------------------------------------------------------------
	//	�ǂݍ��񂾃\�[�X���R���p�C��
	glCompileShader(_shaderId);
	//	�R���p�C���̌��ʂ��擾
	s32 compile;
	glGetShaderiv(_shaderId,GL_COMPILE_STATUS,&compile);
	//	���s�����ꍇ
	if(compile == GL_FALSE)
	{
		s32 size;
		glGetShaderiv( _shaderId, GL_INFO_LOG_LENGTH, &size );
		//	�G���[���b�Z�[�W�\��
		if( size > 0 )
		{
			s32 len;						//	���O�̒���
			char* log = new char[size];		//	���O�̓��e���i�[����z��
			glGetShaderInfoLog( _shaderId, size, &len, log );	//	���O�擾
			ERROR_LOG("CompileError" << filePath << ":" << log);
			SafeDeleteArray(log);
		}
		return false;
	}

	_type = type;
	return true;
}

//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
ShaderProgram::ShaderProgram()
: _program(-1)
, _programType(PROGRAM_TYPE::NONE)
{
}
//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
ShaderProgram::~ShaderProgram()
{
	Clear();
}

//-----------------------------------------------------------------------------
//!	���
//-----------------------------------------------------------------------------
void ShaderProgram::Clear()
{
	glDeleteProgram(_program);
}

//-----------------------------------------------------------------------------
//!	�V�F�[�_�[�̃R���p�C��
//!	@retval	true:����	false:���s
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
	//	�v���O�����I�u�W�F�N�g�̐���
	_program = glCreateProgram();
	//	�I�u�W�F�N�g���֘A�t����
	glAttachShader(_program, pObject->GetShaderId());

	//	�V�F�[�_�[�v���O�����������N
	glLinkProgram(_program);

	//	�����N�̊m�F
	s32 link;
	glGetProgramiv(_program, GL_LINK_STATUS, &link);
	if (link == GL_FALSE)
	{
		//	���s�����ꍇ
		s32 size = 0;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &size);
		if (size > 0)
		{
			//	�G���[���b�Z�[�W
			s32 len;						//	���O�̒���
			char* log = new char[size];		//	���O�̓��e���i�[����z��
			glGetProgramInfoLog(_program, size, &len, log);	//	���O�擾
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
	//	�v���O�����I�u�W�F�N�g�̐���
	_program = glCreateProgram();
	//	�I�u�W�F�N�g���֘A�t����
	glAttachShader(_program, pObject1->GetShaderId());
	glAttachShader(_program, pObject2->GetShaderId());

	//	�V�F�[�_�[�v���O�����������N
	glLinkProgram(_program);

	//	�����N�̊m�F
	s32 link;
	glGetProgramiv(_program,GL_LINK_STATUS,&link);
	if(link == GL_FALSE)
	{
		//	���s�����ꍇ
		s32 size = 0;
		glGetProgramiv( _program, GL_INFO_LOG_LENGTH, &size );
		if( size > 0 )
		{
			//	�G���[���b�Z�[�W
			s32 len;						//	���O�̒���
			char* log = new char[size];		//	���O�̓��e���i�[����z��
			glGetProgramInfoLog( _program, size, &len, log );	//	���O�擾
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
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
ShaderManager::ShaderManager()
{
	_nowRunningShader = 0;
}
//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
ShaderManager::~ShaderManager()
{
}
//-----------------------------------------------------------------------------
//!	�f�[�^�폜
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
//!	���[�h
//!	@return	�v���O�����C���f�b�N�X
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
//!	�I��
//-----------------------------------------------------------------------------
void ShaderManager::End()
{
	_nowRunningShader = 0;
	glUseProgram(0);
	Manager::Renderer()->DisableShader();
}

//-----------------------------------------------------------------------------
//!	�ꎞ��~
//-----------------------------------------------------------------------------
void ShaderManager::Pause()
{
	glUseProgram(0);
	Manager::Renderer()->DisableShader();
}

//-----------------------------------------------------------------------------
//!	�ĊJ
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
//!	�T�u���[�`���擾
//!	@param	[in]	mode		�V�F�[�_�[�̎��
//!	@param	[in]	name		�֐���
//!	@return	�ϐ��̃��P�[�V����	���s����-1
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
//!	�T�u���[�`���ݒ�
//!	@param	[in]	mode		�V�F�[�_�[�̎��
//!	@param	[in]	index		�ԍ�
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
//!	Uniform���P�[�V�����̎擾
//!	@param	[in]	name		�ϐ���
//!	@return	�ϐ��̃��P�[�V����	-1�Ȃ玸�s
//-----------------------------------------------------------------------------
s32 ShaderManager::GetUniformLocation(const std::string& name)
{
	if (_nowRunningShader == 0){ return -1; }
	s32 location = glGetUniformLocation(_nowRunningShader, name.c_str());
	return location;
}

//-----------------------------------------------------------------------------
//!	�ϐ��ݒ�	s32
//!	@param	[in]	name	�ϐ���
//!	@param	[in]	val		�f�[�^
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(const std::string& name, s32 val)
{
	if (_nowRunningShader == 0){ return; }
	s32 location = glGetUniformLocation(_nowRunningShader, name.c_str());
	if (location < 0){ return; }
	glUniform1i(location, val);
}
//-----------------------------------------------------------------------------
//!	�ϐ��ݒ�	f32
//!	@param	[in]	name	�ϐ���
//!	@param	[in]	val		�f�[�^
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(const std::string& name, f32 val)
{
	if (_nowRunningShader == 0){ return; }
	s32 location = glGetUniformLocation(_nowRunningShader, name.c_str());
	if (location < 0){ return; }
	glUniform1f(location, val);
}
//-----------------------------------------------------------------------------
//!	�ϐ��ݒ�	bool
//!	@param	[in]	location	���P�[�V�����ԍ�
//!	@param	[in]	val			�f�[�^
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
//!	�ϐ��ݒ�	Vector2
//!	@param	[in]	name	�ϐ���
//!	@param	[in]	val		�f�[�^
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(const std::string& name, const Vector2& val)
{
	if (_nowRunningShader == 0){ return; }
	s32 location = glGetUniformLocation(_nowRunningShader, name.c_str());
	if (location < 0){ return; }
	glUniform2fv(location, 1, (f32*)&val);
}
//-----------------------------------------------------------------------------
//!	�ϐ��ݒ�	Vector3
//!	@param	[in]	name	�ϐ���
//!	@param	[in]	val		�f�[�^
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(const std::string& name, const Vector3& val)
{
	if (_nowRunningShader == 0){ return; }
	s32 location = glGetUniformLocation(_nowRunningShader, name.c_str());
	if (location < 0){ return; }
	glUniform3fv(location, 1, (f32*)&val);
}
//-----------------------------------------------------------------------------
//!	�ϐ��ݒ�	Vector4
//!	@param	[in]	name	�ϐ���
//!	@param	[in]	val		�f�[�^
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(const std::string& name, const Vector4& val)
{
	if (_nowRunningShader == 0){ return; }
	s32 location = glGetUniformLocation(_nowRunningShader, name.c_str());
	if (location < 0){ return; }
	glUniform4fv(location, 1, (f32*)&val);
}
//-----------------------------------------------------------------------------
//!	�ϐ��ݒ�	Color
//!	@param	[in]	name	�ϐ���
//!	@param	[in]	val		�f�[�^
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
//!	�ϐ��ݒ�	Matrix
//!	@param	[in]	name	�ϐ���
//!	@param	[in]	val		�f�[�^
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
//!	�ϐ��ݒ�	s32
//!	@param	[in]	location	���P�[�V�����ԍ�
//!	@param	[in]	val			�f�[�^
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(s32 location, s32 val)
{
	if (_nowRunningShader == 0){ return; }
	if (location < 0){ return; }
	glUniform1i(location, val);
	isErrorGL();
}
//-----------------------------------------------------------------------------
//!	�ϐ��ݒ�	f32
//!	@param	[in]	location	���P�[�V�����ԍ�
//!	@param	[in]	val			�f�[�^
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(s32 location, f32 val)
{
	if (_nowRunningShader == 0){ return; }
	if (location < 0){ return; }
	glUniform1f(location, val);
	isErrorGL();
}
//-----------------------------------------------------------------------------
//!	�ϐ��ݒ�	bool
//!	@param	[in]	location	���P�[�V�����ԍ�
//!	@param	[in]	val			�f�[�^
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
//!	�ϐ��ݒ�	Vector2
//!	@param	[in]	location	���P�[�V�����ԍ�
//!	@param	[in]	val			�f�[�^
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(s32 location, const Vector2& val)
{
	if (_nowRunningShader == 0){ return; }
	if (location < 0){ return; }
	glUniform2fv(location, 1, (f32*)&val);
	isErrorGL();
}
//-----------------------------------------------------------------------------
//!	�ϐ��ݒ�	Vector3
//!	@param	[in]	location	���P�[�V�����ԍ�
//!	@param	[in]	val			�f�[�^
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(s32 location, const Vector3& val)
{
	if (_nowRunningShader == 0){ return; }
	if (location < 0){ return; }
	glUniform3fv(location, 1, (f32*)&val);
	isErrorGL();
}
//-----------------------------------------------------------------------------
//!	�ϐ��ݒ�	Vector4
//!	@param	[in]	location	���P�[�V�����ԍ�
//!	@param	[in]	val			�f�[�^
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(s32 location, const Vector4& val)
{
	if (_nowRunningShader == 0){ return; }
	if (location < 0){ return; }
	glUniform4fv(location, 1, (f32*)&val);
	isErrorGL();
}
//-----------------------------------------------------------------------------
//!	�ϐ��ݒ�	Color
//!	@param	[in]	location	���P�[�V�����ԍ�
//!	@param	[in]	val			�f�[�^
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
//!	�ϐ��ݒ�	Matrix
//!	@param	[in]	location	���P�[�V�����ԍ�
//!	@param	[in]	val			�f�[�^
//!	@param	[in]	transpose	�]�u	default:false
//-----------------------------------------------------------------------------
void ShaderManager::SetUniform(s32 location, const Matrix& val, bool transpose)
{
	if (_nowRunningShader == 0){ return; }
	if (location < 0){ return; }
	s8 t = transpose ? GL_TRUE : GL_FALSE;
	glUniformMatrix4fv(location, 1, t, (f32*)&val);
	isErrorGL();
}


//!	�ϐ��ݒ�	Matrix
//!	@param	[in]	location	���P�[�V�����ԍ�
//!	@param	[in]	val			�f�[�^
//!	@param	[in]	size		�o�b�t�@��
//!	@param	[in]	transpose	�]�u	default:false
void ShaderManager::SetUniforms(s32 location, const Matrix* val, u32 size, bool transpose)
{
	if(_nowRunningShader == 0){ return; }
	if(location < 0){ return; }
	s8 t = transpose ? GL_TRUE : GL_FALSE;
	glUniformMatrix4fv(location, size, t, (f32*)&val);
	isErrorGL();
}


//-----------------------------------------------------------------------------
//!	���_,�t���O�����g���[�h
//!	@param	[in]	filePath	�t�@�C���p�X
//!	@return	�V�F�[�_�[ID
//-----------------------------------------------------------------------------
ShaderHundler ShaderManager::LoadVSFS(const std::string& shaderName)
{
	// �e�V�F�[�_��ǂݍ���
	ShaderObject* pVS = LoadObjectVS("shader/" + shaderName + ".vert");
	ShaderObject* pFS = LoadObjectFS("shader/" + shaderName + ".frag");

	ShaderProgram* pProgram = nullptr;

	// ��ł����s���Ă���ꍇ�̓G���[
	if (!pVS || !pFS)
	{
		DeleteShaderObject(pVS);
		SafeDelete(pVS);
		DeleteShaderObject(pFS);
		SafeDelete(pFS);
		return ShaderHundler(nullptr, nullptr, nullptr, nullptr);
	}

	// �R���p�C��
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
//!	�R���s���[�g���[�h
//!	@param	[in]	filePath	�t�@�C���p�X
//!	@return	�V�F�[�_�[ID
//-----------------------------------------------------------------------------
ShaderHundler ShaderManager::LoadCS(const std::string& shaderName)
{
	// �e�V�F�[�_��ǂݍ���
	ShaderObject* pCS = LoadObjectCS("shader/" + shaderName + ".comp");
	ShaderProgram* pProgram = nullptr;

	// ��ł����s���Ă���ꍇ�̓G���[
	if (!pCS)
	{
		SafeDelete(pCS);
//		return -1;
		return ShaderHundler(nullptr, nullptr, nullptr, nullptr);
	}

	// �R���p�C��
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
//!	���_�V�F�[�_�[���[�h
//!	@param	[in]	filePath		�t�@�C���f�B���N�g��������
//!	@return	�V�F�[�_�[�I�u�W�F�N�g
//-----------------------------------------------------------------------------
ShaderObject* ShaderManager::LoadObjectVS(const std::string& filePath)
{
	ShaderObject* pShader = nullptr;

	// ���łɓǂݍ���ł���ꍇ�̓��X�g����擾����
	for (auto* pVS : _pVsList)
	{
		const std::string& fileName = pVS->GetFilePath();
		if (fileName == filePath)
		{
			pShader = pVS;
			break;
		}
	}

	// ���X�g���ɑ��݂��Ȃ��ꍇ�͐V�K�쐬����
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
//!	�t���O�����g�V�F�[�_�[���[�h
//!	@param	[in]	filePath		�t�@�C���f�B���N�g��������
//!	@return	�V�F�[�_�[�I�u�W�F�N�g
//-----------------------------------------------------------------------------
ShaderObject* ShaderManager::LoadObjectFS(const std::string& filePath)
{
	ShaderObject* pShader = nullptr;

	// ���łɓǂݍ���ł���ꍇ�̓��X�g����擾����
	for (auto* pFS : _pVsList)
	{
		const std::string& fileName = pFS->GetFilePath();
		if (fileName == filePath)
		{
			pShader = pFS;
			break;
		}
	}

	// ���X�g���ɑ��݂��Ȃ��ꍇ�͐V�K�쐬����
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
//!	�R���s���[�g�V�F�[�_�[���[�h
//!	@param	[in]	filePath	�t�@�C���p�X
//!	@return	�V�F�[�_�[�I�u�W�F�N�g
//-----------------------------------------------------------------------------
ShaderObject* ShaderManager::LoadObjectCS(const std::string& filePath)
{
	ShaderObject* pShader = nullptr;

	// ���łɓǂݍ���ł���ꍇ�̓��X�g����擾����
	for (auto* pCS : _pCsList)
	{
		const std::string& fileName = pCS->GetFilePath();
		if (fileName == filePath)
		{
			pShader = pCS;
			break;
		}
	}
	// ���X�g���ɑ��݂��Ȃ��ꍇ�͐V�K�쐬����
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

