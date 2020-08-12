//=============================================================================
//!
//!	@file	GameObjectBase.h
//!	@brief	オブジェクト基底クラス
//!
//!	@author	YKCH
//!
//=============================================================================
#pragma once

// TODO キーやマウスなどのイベントクラスを入れる

//	描画はシーンに登録していれば全て自動になる
//	子オブジェクト、自身の追加メッシュはオブジェクトに登録しておけば自動で描画する
//	コンポーネントもオブジェクトに登録することで自動で実行する
//	オーバーライドは基本的にはUpdateのみで初期化処理が必要な場合はInitも
//	Renderは基本的には必要ないが記述は可能なので必要であればオーバーライドする
//	Update,Renderは自動で呼び出される
//-------------------------------------------------------------------
//!	オブジェクト基底クラス
//-------------------------------------------------------------------
class GameObjectBase : public BasicBase
{
	friend class SceneBase;
public:
	//!	コンストラクタ
	GameObjectBase();
	//!	コンストラクタ
	//!	@param	[in]	position	座標
	//!	@param	[in]	rotate		回転
	//!	@param	[in]	Scale		サイズ	default:Vector3::ONE
	GameObjectBase(const Vector3& position, const Quaternion& rotate, const Vector3& scale = Vector3::ONE, bool enable = true);
	//!	デストラクタ
	virtual ~GameObjectBase();

	//!	親設定
	//!	@param	[in]	pParentObject	親のポインタ
	void SetParentObject(GameObjectBase* pParentObject);

	//!	親取得
	//!	@return	親オブジェクトポインタ
	//!	@retval	nullptr	親なし
	GameObjectBase* GetParentObject()const;


	//!	子追加
	//!	@param	[in]	pChildObject	子のポインタ
	void AddChildObject(GameObjectBase* pChildObject);
	//!	子削除
	//!	@param	[in]	pChildObject	子のポインタ
	//!	@return	解放後の参照数 0なら削除済み
	s32 ReleaseChildObject(GameObjectBase* pChildObject);

	//!	親からの解放
	void ReleaseParent();

	//!	子オブジェクトタグ検索
	//!	@param	[in]	tag	オブジェクトのタグ
	//!	@return	オブジェクトのポインタ
	//!	@retval	nullptr	データなし
	GameObjectBase* GetChildObject(const std::string& tag)const;

	//!	子オブジェクトの解放
	void ReleaseChildObjects();

	//	設定関数
	//!	ワールド行列設定
	//!	@param	[in]	matrix		行列
	void SetWorldMatrix(const Matrix& matrix);
	//!	ワールド座標
	//!	@param	[in]	position	座標
	void SetWorldPosition(const Vector3& position);
	//!	ワールド回転
	//!	@param	[in]	rotate		回転
	void SetWorldRotate(const Quaternion& rotate);
	//!	ワールドサイズ
	//!	@param	[in]	scale		サイズ
	void SetWorldScale(const Vector3& scale);

	//!	ワールド回転
	//!	@param	[in]	rotate		回転
	void AddWorldPosition(const Vector3& position);
	//!	ワールド回転
	//!	@param	[in]	rotate		回転
	void AddWorldRotate(const Quaternion& rotate);
	//!	ワールドサイズ
	//!	@param	[in]	scale		サイズ
	void AddWorldScale(const Vector3& scale);

	//!	ローカル行列
	//!	@param	[in]	matrix		行列
	void SetLocalMatrix(const Matrix& matrix);

	//!	ローカル座標
	//!	@param	[in]	position	座標
	void SetLocalPosition(const Vector3& position);
	//!	ローカル回転
	//!	@param	[in]	rotate		回転
	void SetLocalRotate(const Quaternion& rotate);
	//!	ローカルサイズ
	//!	@param	[in]	scale		サイズ
	void SetLocalScale(const Vector3& scale);

	//!	ローカル座標
	//!	@param	[in]	position	座標
	void AddLocalPosition(const Vector3& position);
	//!	ローカル回転
	//!	@param	[in]	rotate		回転
	void AddLocalRotate(const Quaternion& rotate);
	//!	ローカルサイズ
	//!	@param	[in]	scale		サイズ
	void AddLocalScale(const Vector3& scale);

	//!	親ローカル座標
	//!	@param	[in]	position	座標
	void SetRelativePosition(const Vector3& position);
	//!	親ローカル回転
	//!	@param	[in]	rotate		回転
	void SetRelativeRotate(const Quaternion& rotate);

	//!	親ローカル座標
	//!	@param	[in]	position	座標
	void AddRelativePosition(const Vector3& position);
	//!	親ローカル回転
	//!	@param	[in]	rotate		回転
	void AddRelativeRotate(const Quaternion& rotate);

	//!	ワールド行列作成
	//!	@param	[in]	myMatrix	掛け足す行列
	//!	@param	[in]	pParent		親オブジェクトのポインタ default:nullptr
	Matrix  CreateWorldMatrix(const Matrix& myMatrix, GameObjectBase* pParent = nullptr) const;


	//	取得関数

	//!	ローカル行列
	//!	@return 行列
	Matrix  GetLocalMatrix()const		{ return _localMatrix; }
	//!	ローカル座標
	//!	@return 座標
	Vector3 GetLocalPosition()const		{ return _localPosition; }
	//!	ローカル回転
	//!	@return 回転
	Quaternion GetLocalRotate()const	{ return _localRotate; }
	//!	ローカルサイズ
	//!	@return サイズ
	Vector3 GetLocalScale()const		{ return _localScale; }


	//!	ワールド行列
	//!	@return	行列
	Matrix  GetWorldMatrix()const
	{
		return _worldMatrix;
	}

	//!	親のワールド行列
	//!	@return	行列
	Matrix  GetWorldParentMatrix()const
	{
		if (_pParentObject)
		{
			return _pParentObject->_worldMatrix;
		}
		return Matrix::IDENTITY;
	}

	//!	ワールド座標
	//!	@return	座標
	Vector3 GetWorldPosition()const
	{
		return GetWorldMatrix().GetPosition();
	}
	//!	ワールド回転
	//!	@return	回転
	Quaternion GetWorldRotate()const
	{
		return GetWorldMatrix().GetRotateQuaternion();
	}
	//!	ワールドサイズ
	//!	@return	サイズ
	Vector3 GetWorldScale()const
	{
		return GetWorldMatrix().GetScale();
	}
	//!	オブジェクトワールドXベクトル
	//!	@return	オブジェクトワールドXベクトル
	Vector3 GetWorldXVector()const
	{
		return GetWorldMatrix().GetXVector();
	}
	//!	オブジェクトワールドYベクトル
	//!	@return	オブジェクトワールドYベクトル
	Vector3 GetWorldYVector()const
	{
		return GetWorldMatrix().GetYVector();
	}
	//!	オブジェクトワールドZベクトル
	//!	@return	オブジェクトワールドZベクトル
	Vector3 GetWorldZVector()const
	{
		return GetWorldMatrix().GetZVector();
	}

	//!	Processオブジェクト作成
	//!	@tparam	T		オブジェクト型
	//!	@tparam	Args	引数リスト型
	//!	@param	args	引数リスト
	//!	@return	Processオブジェクト
	template <typename T, typename... Args>
	RefPointer<T> CreateProcess(Args&&... args)
	{
		RefPointer<T> result = CreateRefPointer<T>(args...);
		AddProcess(result.get());
		return result;
	}

	//!	Processオブジェクト作成 (Init)
	//!	@tparam	T		オブジェクト型
	//!	@tparam	Args	引数リスト型
	//!	@param	args	引数リスト
	//!	@return	Processオブジェクト
	template <typename T, typename... Args>
	RefPointer<T> CreateProcessInit(Args&&... args)
	{
		RefPointer<T> result = CreateRefPointerInit<T>(args...);
		AddProcess(result.get());
		return result;
	}

	//!	Meshオブジェクト作成
	//!	@tparam	T		オブジェクト型
	//!	@tparam	Args	引数リスト型
	//!	@param	args	引数リスト
	//!	@return	Meshオブジェクト
	template <typename T, typename... Args>
	RefPointer<T> CreateMesh(Args&&... args)
	{
		RefPointer<T> result = CreateRefPointer<T>(args...);
		AddMesh(result.get());
		return result;
	}

	//!	Meshオブジェクト作成 (Init)
	//!	@tparam	T		オブジェクト型
	//!	@tparam	Args	引数リスト型
	//!	@param	args	引数リスト
	//!	@return	Meshオブジェクト
	template <typename T, typename... Args>
	RefPointer<T> CreateMeshInit(Args&&... args)
	{
		RefPointer<T> result = CreateRefPointerInit<T>(args...);
		AddMesh(result.get());
		return result;
	}

	//!	子オブジェクト作成
	//!	@tparam	T		オブジェクト型
	//!	@tparam	Args	引数リスト型
	//!	@param	args	引数リスト
	//!	@return	子オブジェクト
	template <typename T, typename... Args>
	RefPointer<T> CreateChildObject(Args&&... args)
	{
		RefPointer<T> result = CreateGameObject<T>(args...);
		AddChildObject(result.get());
		return result;
	}

	//!	子オブジェクト作成 (Init)
	//!	@tparam	T		オブジェクト型
	//!	@tparam	Args	引数リスト型
	//!	@param	args	引数リスト
	//!	@return	子オブジェクト
	template <typename T, typename... Args>
	RefPointer<T> CreateChildObjectInit(Args&&... args)
	{
		RefPointer<T> result = CreateGameObjectInit<T>(args...);
		AddChildObject(result.get());
		return result;
	}

	//!	Processオブジェクト追加
	//!	@param	[in]	pProcess	Processオブジェクトのポインタ
	void AddProcess(ProcessBase* pProcess);

	//!	Meshオブジェクト追加
	//!	@param	[in]	pProcess	Meshオブジェクトのポインタ
	void AddMesh(MeshBase* pMesh);

	//!	Processオブジェクト検索
	//!	@tparam	T	検索するProcessオブジェクトの型
	//!	@return	検索結果
	template <class T>
	std::vector<T*> GetProcesss()
	{
		std::vector<T*> result;
		for(auto* pProcess : _pProcesss)
		{
			T* p = dynamic_cast<T*>(pProcess);
			if(p)
			{
				result.push_back(p);
			}
		}
		return result;
	}

	//!	Meshオブジェクト検索
	//!	@tparam	T	検索するMeshオブジェクトの型
	//!	@return	検索結果
	template <class T>
	std::vector<T*> GetMeshs()
	{
		std::vector<T*> result;
		for(auto* pMesh : _pMeshs)
		{
			T* p = dynamic_cast<T*>(pMesh);
			if(p)
			{
				result.push_back(p);
			}
		}
		return result;
	}

	//!	行列のバインド
	void BindWorldMatrix();


	//!	Processオブジェクトのタグ検索
	//!	@param	[in]	tag	タグ
	//!	@return	検索結果
	std::vector<ProcessBase*> GetProcesss(const std::string& tag);

	//!	Processオブジェクトのタグ検索
	//!	@param	[in]	tags	タグ
	//!	@return	検索結果
	std::vector<ProcessBase*> GetProcesss(const std::vector<std::string>& tags);

	//!	Meshオブジェクトのタグ検索
	//!	@param	[in]	tag	タグ
	//!	@return	検索結果
	std::vector<MeshBase*> GetMeshs(const std::string& tag);
	//!	Meshオブジェクトのタグ検索
	//!	@param	[in]	tags	タグ
	//!	@return	検索結果
	std::vector<MeshBase*> GetMeshs(const std::vector<std::string>& tags);


	//!	有効化フラグ設定
	//!	@param	[in]	enable	有効化
	void Enable(bool enable)
	{
		_enable = enable;
	}

	//!	有効化フラグ取得
	//!	@retval	true	有効
	//!	@retval	false	無効
	bool IsEnable()const
	{
		return _enable;
	}
protected:
	//!	行列の更新
	void MatrixUpdate()
	{
		_localMatrix = Matrix::Affine(_localPosition, _localRotate, _localScale);
		if(_pParentObject)
		{
			_worldMatrix = _localMatrix * _pParentObject->_worldMatrix;
		}
		else
		{
			_worldMatrix = _localMatrix;
		}
		for(auto* pChild : _pChildrenObjects)
		{
			pChild->MatrixUpdate();
		}
	}

private:
	//!	子オブジェクトの更新
	void ChildrenUpdate()
	{
		for (auto& pObject : _pChildrenObjects)
		{
			if(pObject && pObject->IsEnable())
			{
				pObject->Update();
				if (!pObject)continue;	//	消えてたら次のオブジェクトへ
				pObject->ProcessUpdate();
				if (!pObject)continue;	//	消えてたら次のオブジェクトへ
				pObject->ChildrenUpdate();
			}
		}

		//	消えたオブジェクトをリストから消す
		auto itr = _pChildrenObjects.begin();
		while (itr != _pChildrenObjects.end())
		{
			if (!(*itr))
			{
				itr = _pChildrenObjects.erase(itr);
				continue;
			}
			++itr;
		}

	}
	//!	子オブジェクトの描画
	void ChildrenRender()
	{
		auto itr = _pChildrenObjects.begin();
		while(itr != _pChildrenObjects.end())
		{
			GameObjectBase* p = (*itr);
			if(p && p->IsEnable())
			{
				p->BindWorldMatrix();
				p->Render();
				p->MeshRender();
				p->ChildrenRender();
			}
			++itr;
		}
	}

	//!	プロセス更新
	void ProcessUpdate();
	//!	メッシュ描画
	void MeshRender();

protected:
	//!	プロセス解放
	//!	@param	[in]	pProcess	プロセスのポインタ
	void ReleaseProcess(ProcessBase* pProcess);
	//!	メッシュ解放
	//!	@param	[in]	pMesh		メッシュのポインタ
	void ReleaseMesh(MeshBase* pMesh);

protected:
	GameObjectBase*				_pParentObject;		//!< 親オブジェクト
	std::list<GameObjectBase*>	_pChildrenObjects;	//!< 子オブジェクト

	std::list<MeshBase*>		_pMeshs;			//!< メッシュ
	std::list<ProcessBase*>		_pProcesss;			//!< プロセス

	Vector3		_localPosition;	//!< 座標
	Quaternion	_localRotate;	//!< 回転方向
	Vector3		_localScale;	//!< サイズ
	Matrix		_localMatrix;	//!< マトリックス

	Matrix		_worldMatrix;	//!< マトリックス(ワールド)

	bool _enable;	//!< 有効フラグ
};
