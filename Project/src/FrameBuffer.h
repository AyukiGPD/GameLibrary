//=============================================================================
//!
//!	@file	Texture.h
//!	@brief	画像描画クラス
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

//=============================================================================
//!	FrameBufferObject
//=============================================================================
class FrameBuffer
{
public:
	enum class TARGET
	{
		COLOR,
		DEPTH,
		STENCIL
	};

	struct Desc
	{
		Desc(Texture::Param param, TARGET target = TARGET::COLOR)
		: _param(param)
		, _target(target)
		{
		}
		Texture::Param _param;
		TARGET _target;
	};
	//!	コンストラクタ
	FrameBuffer();
	//!	デストラクタ
	virtual ~FrameBuffer();

	//!	生成
	//!	@param	[in]	width	幅
	//!	@param	[in]	height	高さ
	//!	@param	[in]	params	テクスチャパラメータリスト
	//!	@return	new生成したFrameBuffer	失敗時はnullptr
	static FrameBuffer* Create(u32 width, u32 height, const std::vector<Desc>& descs);

	//!	生成	テクスチャ一つのみ
	//!	@param	[in]	width	幅
	//!	@param	[in]	height	高さ
	//!	@param	[in]	param	テクスチャパラメータ
	//!	@return	new生成したFrameBuffer	失敗時はnullptr
	static FrameBuffer* Create(u32 width, u32 height, const Desc& desc);

	//!	初期化
	//!	@param	[in]	width	幅
	//!	@param	[in]	height	高さ
	//!	@param	[in]	params	テクスチャパラメータリスト
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool Init(u32 width, u32 height, const std::vector<Desc>& descs);

	//!	初期化	テクスチャ一つのみ
	//!	@param	[in]	width	幅
	//!	@param	[in]	height	高さ
	//!	@param	[in]	param	テクスチャパラメータ
	//!	@retval	true	成功
	//!	@retval	false	失敗
	bool Init(u32 width, u32 height, const Desc& desc);
	//!	解放
	void Clear();

	//!	開始
	//!	@param	[in]	isClear	バッファクリア	default:true
	void Begin(bool isClear = true);
	//!	終了
	void End();

	//!	有効化
	void Bind();
	//!	無効化
	static void UnBind();

	//!	レンダーターゲット指定
	void SetTargetBuffer();

	void BufferClear();

	//	取得関数

	//!	FBO ID
	u32 GetId()const				{ return _fboId; }
	//!	幅
	u32 GetWidth()const				{ return _width; }
	//!	高さ
	u32 GetHeight()const			{ return _height; }
	//!	バッファ数
	size_t GetTextureSize()const		{ return _pRenderTargets.size(); }
	//!	動いてる
	bool IsRun()const				{ return _run; }

	//!	テクスチャ取得
	//!	@return	テクスチャポインタ
	RenderTarget* GetTexture(u32 index)
	{
		if (index >= _pRenderTargets.size()){ return nullptr; }
		return _pRenderTargets[index];
	}

private:
	u32 _fboId;				//!< ID
	u32 _width;				//!< 幅
	u32 _height;			//!< 高さ
	bool _run;
	std::vector<RenderTarget*> _pRenderTargets;
	RenderTarget* _pDepthTarget;
	RenderTarget* _pStencilTarget;
};
