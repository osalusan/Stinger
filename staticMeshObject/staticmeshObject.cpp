#include "staticmeshObject.h"
#include "manager/objModelManager.h"
#include "renderer/objModelRenderer.h"
#include "component/shaderComponent.h"
#include "component/boxCollisionComponent.h"

StaticMeshObject::StaticMeshObject(const STATICMESH_MODEL& model)
{
	m_Model = model;
}

void StaticMeshObject::Update(const float& deltaTime)
{
	GameObject::Update(deltaTime);

	m_RotationMatrix = GetRotationMatrix();

	MoveControl(deltaTime);

	// 当たり判定処理の前に
	if (m_BoxCollCache == nullptr) return;

	if (ObjModelRenderer* model = ObjModelManager::GetModel(m_Model))
	{
		MODEL* modelData = model->GetModel();
		m_BoxCollCache->SetCollisionInfo(m_Position, m_Scale, modelData->Center, modelData->Scale, m_RotationMatrix);
	}
}

void StaticMeshObject::Draw()
{
	GameObject::Draw();

	if (!m_Visible) return;

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = m_Color;
	material.TextureEnable = false;

	Renderer::SetMaterial(material);

	if (ObjModelRenderer* model = ObjModelManager::GetModel(m_Model))
	{
		if (model == nullptr) return;
		model->Draw();
	}	
}

const ObjModelRenderer* StaticMeshObject::GetModel()const
{
	return ObjModelManager::GetModel(m_Model);
}
