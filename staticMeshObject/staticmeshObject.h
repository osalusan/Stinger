#pragma once
#include "object/gameObject.h"

// 前方宣言
class ObjModelRenderer;
enum class STATICMESH_MODEL;
struct MODEL;

class StaticMeshObject :public GameObject
{
protected:
	STATICMESH_MODEL m_Model;						// モデル本体 / コンストラクタで初期化
	ObjModelRenderer* m_ModelRenderer = nullptr;
	BoxCollisionComponent* m_BoxCollCache = nullptr;
	XMFLOAT4 m_Color = {0.0f,1.0f,0.0f,1.0f};

public:
	StaticMeshObject() = delete;
	StaticMeshObject(const STATICMESH_MODEL& model);
	virtual ~StaticMeshObject()override;
	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;

	const MODEL* GetModel();

	const STATICMESH_MODEL& GetStaticModel()
	{
		return m_Model;
	}
};