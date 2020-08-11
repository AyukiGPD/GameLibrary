//=============================================================================
//!
//!	@file	SystemResource.h
//!	@brief	リソースビルドクラス
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//	未実装

#if 0

class SystemResource
{
public:
	//!	コンストラクタ
	SystemResource();
	//!	デストラクタ
	virtual ~SystemResource();

	//!	データビルド
	void DataBuild();
private:
	//	ビルド用プライベート関数

	//!	画像
	void ImageBuild();
	//!	音楽
	void SoundBuild();
	//!	モデル
	void ModelBuild();
	//!	フォント
	void FontBuild();
	//!	シェーダー
	void ShaderBuild();
	//!	その他
	void OtherBuild();

	//!	テキスト
	void TextBuild();
};

#endif

