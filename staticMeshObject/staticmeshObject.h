#pragma once
#include "object/gameObject.h"

// 前方宣言
enum class STATICMESH_MODEL;
class ObjModelRenderer;
// OBJを読み込んで描画する基底クラス
class StaticMeshObject :public GameObject
{
protected:
	STATICMESH_MODEL m_Model;								// モデル本体 / コンストラクタで初期化
	BoxCollisionComponent* m_BoxCollCache = nullptr;		// 四角の当たり判定用 / TODO :スフィアの当たり判定を作成したら2つ目を追加
	// 初期設定した数値を保存しておく用
	XMFLOAT3 m_OffsetCollPos = {};
	XMFLOAT3 m_OffsetCollScl = {};

	XMFLOAT4 m_Color = {1.0f,1.0f,1.0f,1.0f};
	XMMATRIX m_RotScaleMatrix = {};							//  マトリクスの回転として描画処理や当たり判定に伝える場合に使用

	bool m_Visible = true;		// 見えなくなっても当たり判定は残る

	virtual void MoveControl(const float& deltaTime) = 0;
public:
	StaticMeshObject() = delete;
	StaticMeshObject(const STATICMESH_MODEL& model);		// StaticMesh内で種類分けする為に
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;

	const ObjModelRenderer* GetModel()const;

	const STATICMESH_MODEL& GetStaticModel()const
	{
		return m_Model;
	}

	const XMMATRIX& GetRotationMatrixStaticMesh()const
	{
		return m_RotScaleMatrix;
	}

	void SetOffsetCollPos(const XMFLOAT3& pos)
	{
		m_OffsetCollPos = pos;
	}
	void SetOffsetCollScl(const XMFLOAT3& scl)
	{
		m_OffsetCollScl = scl;
	}
};