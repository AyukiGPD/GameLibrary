//=============================================================================
//!
//!	@file	SceneGame.h
//!	@brief	ゲームシーン
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//-------------------------------------------------------------------
//!	ゲームシーンクラス
//-------------------------------------------------------------------
class SceneGame :public SceneBase
{
public:
	//!	コンストラクタ
	SceneGame();
	//!	デストラクタ
	virtual ~SceneGame();

	//!	初期化
	void Init();
	//!	更新
	void Update();
	//!	描画
	void Render();
	//!	終了
	void Exit();

private:
	RefPointer<CameraObject> _pCamera;			//!< カメラオブジェクト
	RefPointer<LightObject> _pDirectionalLight;	//!< 平行光源オブジェクト

	std::array< RefPointer<SoundObject>,3 >	_pBgmSounds;	//!< BGMオブジェクト

	FontHundler _font;		//!< フォントデータ
	bool _isPause;	//!< 一時停止フラグ
};


