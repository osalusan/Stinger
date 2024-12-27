#include "staticmeshObject.h"
#include "manager/objModelManager.h"
#include "renderer/objModelRenderer.h"
#include "component/shaderComponent.h"
#include "component/boxCollisionComponent.h"

StaticMeshObject::StaticMeshObject(const STATICMESH_MODEL& model)
{
	m_Model = model;
}

StaticMeshObject::~StaticMeshObject()
{
	delete m_ModelRenderer;
	m_ModelRenderer = nullptr;
}

void StaticMeshObject::Init()
{
	GameObject::Init();
	if (m_ModelRenderer == nullptr)
	{
		m_ModelRenderer = new ObjModelRenderer;
	}
}

void StaticMeshObject::Update(const float& deltaTime)
{
	GameObject::Update(deltaTime);

	// 当たり判定処理の前に
	if (m_BoxCollCache == nullptr) return;

	if (const MODEL* model = ObjModelManager::GetModel(m_Model))
	{
		m_BoxCollCache->SetBoxCollisionInfo(m_Position, m_Scale, model->Center, model->Scale, GetRotationMatrix());
	}
}

void StaticMeshObject::Draw()
{
	GameObject::Draw();

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = m_Color;
	material.TextureEnable = false;

	Renderer::SetMaterial(material);

	// TODO,HACK :変更予定 / FBXRendererと同じ形式に
	if (m_ModelRenderer != nullptr)
	{
		if (const MODEL* model = ObjModelManager::GetModel(m_Model))
		{
			m_ModelRenderer->Draw(model);
		}
	}
}

const MODEL* StaticMeshObject::GetModel()
{
	return ObjModelManager::GetModel(m_Model);
}
