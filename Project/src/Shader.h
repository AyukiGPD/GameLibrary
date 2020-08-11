//=============================================================================
//!
//!	@file	Shader.h
//!	@brief	�V�F�[�_�[�Ǘ��N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//!	�V�F�[�_�[���
enum class SHADER_TYPE
{
	VS,		//!< ���_�V�F�[�_�[
	FS,		//!< �t���O�����g�V�F�[�_�[
	CS,		//!< �R���s���[�g�V�F�[�_�[
	NONE,
};

//!	�V�F�[�_�[�v���O�������
enum class PROGRAM_TYPE
{
	VS_FS,	//!< ���_,�t���O�����g

	CS,		//!< �R���s���[�g
	NONE,
};

//-------------------------------------------------------------------
//!	�V�F�[�_�[�I�u�W�F�N�g
//-------------------------------------------------------------------
class ShaderObject
{
public:
	//!	�R���X�g���N�^
	ShaderObject();
	//!	�f�X�g���N�^
	virtual ~ShaderObject();

	//!	���
	void Clear();

	//! �V�F�[�_�[�ǂݍ��݊֐�
	//!	@param	[in]	filePath		�t�@�C���f�B���N�g��������
	//!	@param	[in]	type			�V�F�[�_�[�^�C�v
	//!	@retval	true:����	false:���s
	bool		Load(const std::string& filePath, SHADER_TYPE type);

	//!	�V�F�[�_�[�̒��g
	bool		IsEmpty() const		{ return _shaderId == 0; }
	//!	�t�@�C����
	std::string	GetFilePath() const	{ return _filePath; }
	//!	�V�F�[�_�[�^�C�v
	SHADER_TYPE	GetType() const		{ return _type; }
	//!	�V�F�[�_�[ID
	GLuint		GetShaderId() const	{ return _shaderId; }
private:
	std::string	_filePath;		//!< �t�@�C���p�X
	SHADER_TYPE	_type;			//!< �V�F�[�_�[�^�C�v
	GLuint		_shaderId;		//!< �V�F�[�_�[ID

};

//-------------------------------------------------------------------
//!	�V�F�[�_�[�v���O����
//-------------------------------------------------------------------
class ShaderProgram
{
public:
	//!	�R���X�g���N�^
	ShaderProgram();
	//!	�f�X�g���N�^
	virtual ~ShaderProgram();

	//!	���
	void Clear();

	//!	�V�F�[�_�[�̃R���p�C��
	//!	@retval	true:����	false:���s
	bool Compile(ShaderObject* pObject, PROGRAM_TYPE type);
	bool Compile(ShaderObject* pObject1, ShaderObject* pObject2, PROGRAM_TYPE type);
	//!	�V�F�[�_�[���ݒ�
	void SetName(const std::string& name) { _name = name; }

	//!	�V�F�[�_�[�v���O�����擾
	u32 GetProgram() const { return _program; }
	//!	�V�F�[�_�[���擾
	const std::string& GetName() const { return _name; }
	PROGRAM_TYPE GetProgramType()const { return _programType; }
private:
	std::string		_name;			//!< �V�F�[�_�[��
	s32				_program;		//!< �V�F�[�_�[�v���O����
	PROGRAM_TYPE	_programType;
};

//-------------------------------------------------------------------
//!	�V�F�[�_�[����p�I�u�W�F�N�g
//-------------------------------------------------------------------
class ShaderHundler
{
	friend class ShaderManager;
private:
	//!	�R���X�g���N�^
	ShaderHundler(ShaderProgram* pProgram, ShaderObject* pShaderVS, ShaderObject* pShaderFS, ShaderObject* pShaderCS)
	: _pProgram (pProgram)
	, _pShaderVS(pShaderVS)
	, _pShaderFS(pShaderFS)
	, _pShaderCS(pShaderCS)
	{
	}

public:
	//!	�R���X�g���N�^
	ShaderHundler()
	: _pProgram (nullptr)
	, _pShaderVS(nullptr)
	, _pShaderFS(nullptr)
	, _pShaderCS(nullptr)
	{
	}
	//!	�f�X�g���N�^
	virtual ~ShaderHundler(){};

	//!	�ێ��`�F�b�N
	//!	@retval	true	�ێ����Ă��Ȃ�
	bool IsEmpty()const
	{
		return _pProgram == nullptr;
	}

	//!	�V�F�[�_�[�v���O����ID�擾
	//!	@return	�v���O����ID
	u32 GetProgram()const
	{
		if (!_pProgram){ return 0; }
		return _pProgram->GetProgram();
	}

	//!	�V�F�[�_�[���r���h
	bool ReBuild();
	//!	�V�F�[�_�[�폜
	void Delete();
private:
	ShaderProgram* _pProgram;
	ShaderObject*  _pShaderVS;	//!< ���_�V�F�[�_�[
	ShaderObject*  _pShaderFS;	//!< �t���O�����g�V�F�[�_�[
	ShaderObject*  _pShaderCS;	//!< �R���s���[�g�V�F�[�_�[
//	ShaderObject*  _pShaderGS;	//!< �W�I���g���V�F�[�_�[
};

//-------------------------------------------------------------------
//!	�V�F�[�_�[�Ǘ��N���X
//-------------------------------------------------------------------
class ShaderManager : public ManagerBase
{
public:
	//!	�R���X�g���N�^
	ShaderManager();
	//!	�f�X�g���N�^
	virtual ~ShaderManager();
	//!	���
	void Exit()
	{
		Clear();
	}

	//!	�f�[�^�폜
	void Clear();

	void DeleteShaderObject(ShaderObject*& pObject);
	void DeleteShaderProgram(ShaderProgram*& pProgram);


	//!	���[�h
	//!	@return	�v���O�����C���f�b�N�X
//	s32 Load(const std::string& shaderName, PROGRAM_TYPE programType);
	ShaderHundler Load(const std::string& shaderName, PROGRAM_TYPE programType);

	//!	�ԍ��擾
	//!	@param	[in]	shaderName	�V�F�[�_�[��
	//!	@return	�v���O�����C���f�b�N�X
	s32 SearchShader(const std::string& shaderName);

	//	�V�F�[�_�[�`��
	//!	�J�n
	//!	@param	[in]	index	�v���O�����ԍ�
	void Begin(const ShaderHundler& shader);
	//!	�I��
	void End();

	//!	�ꎞ��~
	void Pause();
	//!	�ĊJ
	void Play();
	
	//!	�V�F�[�_�[��
	u32 GetShaderCount() const
	{
		return _pShaderList.size();
	}


	bool IsRunShader()
	{
		return (_nowRunningShader != 0);
	}

	//!	�T�u���[�`���擾
	//!	@param	[in]	mode		�V�F�[�_�[�̎��
	//!	@param	[in]	name		�֐���
	//!	@return	�ϐ��̃��P�[�V����	���s����-1
	s32 GetSubroutine(SHADER_TYPE type, const std::string& str);

	//!	�T�u���[�`���ݒ�
	//!	@param	[in]	mode		�V�F�[�_�[�̎��
	//!	@param	[in]	index		�ԍ�
	void SetSubroutine(SHADER_TYPE type, s32 index);

	//!	�T�u���[�`���ݒ�
	//!	@param	[in]	mode		�V�F�[�_�[�̎��
	//!	@param	[in]	str			�֐���
	void SetSubroutine(SHADER_TYPE type, const std::string str)
	{
		SetSubroutine(type, GetSubroutine(type, str));
	}


	//!	Uniform���P�[�V�����̎擾
	//!	@param	[in]	name		�ϐ���
	//!	@return	�ϐ��̃��P�[�V����	-1�Ȃ玸�s
	s32 GetUniformLocation(const std::string& name);

	//!	�ϐ��ݒ�	s32
	//!	@param	[in]	name		�ϐ���
	//!	@param	[in]	val			�f�[�^
	void SetUniform(const std::string& name, s32 val);
	//!	�ϐ��ݒ�	f32
	//!	@param	[in]	name		�ϐ���
	//!	@param	[in]	val			�f�[�^
	void SetUniform(const std::string& name, f32 val);
	//!	�ϐ��ݒ�	bool
	//!	@param	[in]	name		�ϐ���
	//!	@param	[in]	val			�f�[�^
	void SetUniform(const std::string& name, bool val);
	//!	�ϐ��ݒ�	Vector2
	//!	@param	[in]	name		�ϐ���
	//!	@param	[in]	val			�f�[�^
	void SetUniform(const std::string& name, const Vector2& val);
	//!	�ϐ��ݒ�	Vector3
	//!	@param	[in]	name		�ϐ���
	//!	@param	[in]	val			�f�[�^
	void SetUniform(const std::string& name, const Vector3& val);
	//!	�ϐ��ݒ�	Vector4
	//!	@param	[in]	name		�ϐ���
	//!	@param	[in]	val			�f�[�^
	void SetUniform(const std::string& name, const Vector4& val);
	//!	�ϐ��ݒ�	Color
	//!	@param	[in]	name		�ϐ���
	//!	@param	[in]	val			�f�[�^
	//!	@param	[in]	mode 		true RGBA false	RGB	default:true
	void SetUniform(const std::string& name, const Color& val, bool mode = true);
	//!	�ϐ��ݒ�	Matrix
	//!	@param	[in]	name		�ϐ���
	//!	@param	[in]	val			�f�[�^
	//!	@param	[in]	transpose	�]�u	default:false
	void SetUniform(const std::string& name, const Matrix& val, bool transpose = false);

	//!	�ϐ��ݒ�	s32
	//!	@param	[in]	location	���P�[�V�����ԍ�
	//!	@param	[in]	val			�f�[�^
	void SetUniform(s32 location, s32 val);
	//!	�ϐ��ݒ�	f32
	//!	@param	[in]	location	���P�[�V�����ԍ�
	//!	@param	[in]	val			�f�[�^
	void SetUniform(s32 location, f32 val);
	//!	�ϐ��ݒ�	bool
	//!	@param	[in]	location	���P�[�V�����ԍ�
	//!	@param	[in]	val			�f�[�^
	void SetUniform(s32 location, bool val);
	//!	�ϐ��ݒ�	Vector2
	//!	@param	[in]	location	���P�[�V�����ԍ�
	//!	@param	[in]	val			�f�[�^
	void SetUniform(s32 location, const Vector2& val);
	//!	�ϐ��ݒ�	Vector3
	//!	@param	[in]	location	���P�[�V�����ԍ�
	//!	@param	[in]	val			�f�[�^
	void SetUniform(s32 location, const Vector3& val);
	//!	�ϐ��ݒ�	Vector4
	//!	@param	[in]	location	���P�[�V�����ԍ�
	//!	@param	[in]	val			�f�[�^
	void SetUniform(s32 location, const Vector4& val);
	//!	�ϐ��ݒ�	Color
	//!	@param	[in]	location	���P�[�V�����ԍ�
	//!	@param	[in]	val			�f�[�^
	//!	@param	[in]	mode		true RGBA false	RGB	default:true
	void SetUniform(s32 location, const Color& val, bool mode = true);
	//!	�ϐ��ݒ�	Matrix
	//!	@param	[in]	location	���P�[�V�����ԍ�
	//!	@param	[in]	val			�f�[�^
	//!	@param	[in]	transpose	�]�u	default:false
	void SetUniform(s32 location, const Matrix& val, bool transpose = false);

	//!	�ϐ��ݒ�	Matrix
	//!	@param	[in]	location	���P�[�V�����ԍ�
	//!	@param	[in]	val			�f�[�^
	//!	@param	[in]	size		�o�b�t�@��
	//!	@param	[in]	transpose	�]�u	default:false
	void SetUniforms(s32 location, const Matrix* val, u32 size,bool transpose = false);

private:

	//!	���_,�t���O�����g���[�h
	//!	@param	[in]	filePath	�t�@�C���p�X
	//!	@return	�V�F�[�_�[ID
	ShaderHundler LoadVSFS(const std::string& shaderName);

	//!	�R���s���[�g���[�h
	//!	@param	[in]	filePath	�t�@�C���p�X
	//!	@return	�V�F�[�_�[ID
	ShaderHundler LoadCS(const std::string& shaderName);


	//	�I�u�W�F�N�g���[�h

	//!	���_�V�F�[�_�[���[�h
	//!	@param	[in]	filePath	�t�@�C���p�X
	//!	@return	�V�F�[�_�[�I�u�W�F�N�g
	ShaderObject* LoadObjectVS(const std::string& filePath);
	//!	�t���O�����g�V�F�[�_�[���[�h
	//!	@param	[in]	filePath	�t�@�C���p�X
	//!	@return	�V�F�[�_�[�I�u�W�F�N�g
	ShaderObject* LoadObjectFS(const std::string& filePath);
	//!	�R���s���[�g�V�F�[�_�[���[�h
	//!	@param	[in]	filePath	�t�@�C���p�X
	//!	@return	�V�F�[�_�[�I�u�W�F�N�g
	ShaderObject* LoadObjectCS(const std::string& filePath);

private:
	std::list<ShaderProgram*>	_pShaderList;	//!< �V�F�[�_�[�v���O����
	std::list<ShaderObject*>	_pVsList;		//!< ���_�V�F�[�_�[
	std::list<ShaderObject*>	_pFsList;		//!< �t���O�����g�V�F�[�_�[
	std::list<ShaderObject*>	_pCsList;		//!< �t���O�����g�V�F�[�_�[

	u32 _nowRunningShader;
};


MANAGER_INTERFACE(ShaderManager, Shader);
