//=============================================================================
//!
//!	@file	Tag.h
//!	@brief	�^�O
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

class Tag
{
public:
	//!	�R���X�g���N�^
	Tag() :_tag(""){};
	//!	�R���X�g���N�^
	//!	@param	[in]	str	�^�O
	Tag(const std::string& str) :_tag(str){};

	//!	�^�O�ݒ�
	//!	@param	[in]	str	�^�O
	void SetTag(const std::string& str);
	//!	�^�O�擾
	//!	return	�^�O������
	std::string GetTag()const;

	//	�I�y���[�^�[
	bool operator==(const Tag& tag){ return _tag == tag._tag; };
	bool operator!=(const Tag& tag){ return _tag != tag._tag; };
	bool operator< (const Tag& tag){ return _tag <  tag._tag; };
	bool operator<=(const Tag& tag){ return _tag <= tag._tag; };
	bool operator> (const Tag& tag){ return _tag >  tag._tag; };
	bool operator>=(const Tag& tag){ return _tag >= tag._tag; };

	bool operator==(const std::string& str){ return _tag == str; };
	bool operator!=(const std::string& str){ return _tag != str; };
	bool operator< (const std::string& str){ return _tag <  str; };
	bool operator<=(const std::string& str){ return _tag <= str; };
	bool operator> (const std::string& str){ return _tag >  str; };
	bool operator>=(const std::string& str){ return _tag >= str; };

	bool IsEmpty()const
	{
		return _tag.empty();
	}
private:
	std::string _tag;
};

class Tags : public RefCounter
{
public:
	Tags(){}
	virtual ~Tags()
	{
	}

	void AddTag(const std::string& str)
	{
		if (str.empty()){ return; }
		_tags.push_back(Tag(str));
	}
	void AddTag(const Tag& tag)
	{
		if (tag.IsEmpty()){ return; }
		_tags.push_back(tag);
	}

	bool SearchTag(const std::string& str)
	{
		bool result = false;
		for (auto tag : _tags)
		{
			if (tag != str){ continue; }
			result = true;
			break;
		}
		return result;
	}
	bool SearchTag(const Tag& tag)
	{
		bool result = false;
		for (auto myTag : _tags)
		{
			if (myTag != tag){ continue; }
			result = true;
			break;
		}
		return result;
	}

	bool SearchTag(const std::vector<std::string>& strs)
	{
		for (auto str : strs)
		{
			if (!SearchTag(str))
			{
				return false;
			}
		}
		return true;
	}
	bool SearchTag(const std::list<std::string>& strs)
	{
		for (auto str : strs)
		{
			if (!SearchTag(str))
			{
				return false;
			}
		}
		return true;
	}
	bool SearchTag(const std::vector<Tag>& tags)
	{
		for (auto tag : tags)
		{
			if (!SearchTag(tag))
			{
				return false;
			}
		}
		return true;
	}
	bool SearchTag(const std::list<Tag>& tags)
	{
		for (auto tag : tags)
		{
			if (!SearchTag(tag))
			{
				return false;
			}
		}
		return true;
	}

	bool SearchTag(const Tags& tags)
	{
		return SearchTag(tags._tags);
	}

	std::list<Tag> GetTags()const{ return _tags; }
private:
	std::list<Tag> _tags;
};


class BasicBase : public Tags
{
public:
	BasicBase()
	{}
	virtual ~BasicBase(){}

	//	�I�[�o�[���C�h�p�֐�
	//	Render�͊�{�I�ɂ͕s�K�v�����������ʂŕK�v�ȏꍇ�̓I�[�o�[���C�h����
	//!	������
	virtual void Init(){}
	//!	�X�V
	virtual void Update(){}

	//!	�`��
	virtual void Render(){}
	//!	���
	virtual void Clear(){}

private:
};


