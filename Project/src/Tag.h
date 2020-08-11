//=============================================================================
//!
//!	@file	Tag.h
//!	@brief	タグ
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

class Tag
{
public:
	//!	コンストラクタ
	Tag() :_tag(""){};
	//!	コンストラクタ
	//!	@param	[in]	str	タグ
	Tag(const std::string& str) :_tag(str){};

	//!	タグ設定
	//!	@param	[in]	str	タグ
	void SetTag(const std::string& str);
	//!	タグ取得
	//!	return	タグ文字列
	std::string GetTag()const;

	//	オペレーター
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

	//	オーバーライド用関数
	//	Renderは基本的には不必要だが処理が別で必要な場合はオーバーライドする
	//!	初期化
	virtual void Init(){}
	//!	更新
	virtual void Update(){}

	//!	描画
	virtual void Render(){}
	//!	解放
	virtual void Clear(){}

private:
};


