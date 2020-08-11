//=============================================================================
//!
//!	@file	Light.h
//!	@brief	���C�g�N���X
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

/*!
	@todo
	���C�g�͎�ނɂ���ăN���X�𕪂���
	�V�F�[�_�[�ɂ͑S���ł͂Ȃ���ނ��Ƃɓn��
*/


//!	���C�g���[�h
enum class LIGHT_MODE : s32
{
	INVALID		= -1,		//!< ����
	AMBIENT		=  0,		//!< ����
	DIRECTIONAL	=  1,		//!< ���s����
	POINT		=  2,		//!< �_����
	SPOT		=  3,		//!< �X�|�b�g���C�g
};

//-----------------------------------------------------------------------------
//!	���C�g�I�u�W�F�N�g
//-----------------------------------------------------------------------------
class LightObject : public GameObjectBase
{
public:
	struct Desc
	{
		//!	�R���X�g���N�^
		Desc()
		: _position	(0,0,0)
		, _color	(0)
		, _lightMode(LIGHT_MODE::INVALID)
		, _power	(0.f)
		{		}
		//!	�R���X�g���N�^
		Desc(Vector3 position, Color color, LIGHT_MODE lightMode, f32 power = 1.0f)
		: _position	(position)
		, _color	(color)
		, _lightMode(lightMode)
		, _power	(power)
		{		}

		Vector3		_position;		//!< ���W
		Color		_color;			//!< �F
		LIGHT_MODE	_lightMode;		//!< ���C�g���[�h
		f32			_power;			//!< ���C�g�̋���
	};

public:
	//!	�R���X�g���N�^
	LightObject();
	//!	�R���X�g���N�^
	LightObject(const Desc& desc);
	//!	�f�X�g���N�^
	virtual ~LightObject();

	void SetLight(const Desc& desc);
	//!	�ݒ�
	//!	@param	[in]	position	���W,�x�N�g��
	//!	@param	[in]	color		�F
	//!	@param	[in]	mode		���[�h
	//!	@param	[in]	power		����	default 1.0f
	void SetLight(const Vector3& position, const Color& color, LIGHT_MODE mode, f32 power = 1.0f);
	//!	�F�ݒ�
	//!	@param	[in]	color	�F
	void SetColor(const Color& color);
	//!	���C�g���[�h�ݒ�
	//!	@param	[in]	mode	���[�h
	void SetMode(LIGHT_MODE mode);
	//!	���C�g�̋����ݒ�
	//!	@param	[in]	power	����
	void SetPower(f32 power);


	//!	@return �F�擾
	const Color& GetColor()const { return _color; }
	//!	@return ���C�g���x
	f32 GetPower()const { return _power; }
	//! @return ���C�g���[�h
	LIGHT_MODE GetLightMode()const { return _lightMode; }


private:
	Color				_color;			//!< �F
	f32					_power;			//!< ���C�g�̋���
	LIGHT_MODE			_lightMode;		//!< ���C�g���[�h

};


//-----------------------------------------------------------------------------
//!	���C�g�Ǘ��N���X
//-----------------------------------------------------------------------------
class LightManager : public ManagerBase
{
public:
	//!	�R���X�g���N�^
	LightManager();
	//!	�f�X�g���N�^
	virtual ~LightManager();

	//!	���
	//!	@param	[in,out]	pLight	�������J�����̃|�C���^
	void Add(LightObject* pLight);
	//!	���
	//!	@param	[in,out]	pLight	�������J�����̃|�C���^
	void Release(LightObject* pLight);

	//!	���C�g���X�g�擾
	//!	@return	���C�g���X�g
	std::list<LightObject*>& GetLightList();


private:
	std::list<LightObject*> _pLightList;
};
MANAGER_INTERFACE(LightManager, Light);

