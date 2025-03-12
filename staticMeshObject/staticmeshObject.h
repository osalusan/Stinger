#pragma once
#include "object/gameObject.h"

// 前方宣言
enum class STATICMESH_MODEL;
class ObjModelRenderer;

class StaticMeshObject :public GameObject
{
protected:
	STATICMESH_MODEL m_Model;						// モデル本体 / コンストラクタで初期化
	BoxCollisionComponent* m_BoxCollCache = nullptr;
	XMFLOAT3 m_OffsetCollPos = {};
	XMFLOAT3 m_OffsetCollScl = {};
	XMFLOAT4 m_Color = {0.0f,1.0f,0.0f,1.0f};
	XMMATRIX m_RotationMatrix = {};

	bool m_Visible = true;		// 見えなくなっても当たり判定は残る

	virtual void MoveControl(const float& deltaTime) = 0;
public:
	StaticMeshObject() = delete;
	StaticMeshObject(const STATICMESH_MODEL& model);
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;

	const ObjModelRenderer* GetModel()const;

	const STATICMESH_MODEL& GetStaticModel()const
	{
		return m_Model;
	}

	const XMMATRIX& GetRotationMatrixStaticMesh()const
	{
		return m_RotationMatrix;
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