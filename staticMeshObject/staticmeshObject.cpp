#include "staticmeshObject.h"
#include "manager/objModelManager.h"
#include "renderer/objModelRenderer.h"

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

void StaticMeshObject::Uninit()
{

}

void StaticMeshObject::Draw()
{
	GameObject::Draw();

	if (m_ModelRenderer != nullptr)
	{
		m_ModelRenderer->Draw(ObjModelManager::GetModel(m_Model));
	}
}