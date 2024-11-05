#include "staticmeshObject.h"
#include "renderer/modelRenderer.h"

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
		m_ModelRenderer = new ModelRenderer;
	}

	LoadStaticMesh();
}

void StaticMeshObject::Uninit()
{
	if (m_ModelRenderer != nullptr)
	{
		m_ModelRenderer->Uninit();
	}
}

void StaticMeshObject::Draw()
{
	GameObject::Draw();
	if (m_ModelRenderer != nullptr)
	{
		m_ModelRenderer->Draw();
	}
}

// ------------------------------- private -------------------------------
void StaticMeshObject::LoadModel(const std::string& fileName)
{
	m_ModelRenderer->Load(fileName.c_str());
}
