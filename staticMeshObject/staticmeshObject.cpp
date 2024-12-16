#include "staticmeshObject.h"
#include "manager/objModelManager.h"
#include "renderer/objModelRenderer.h"
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
	// “–‚½‚è”»’èˆ—‚Ì‘O‚É / ‰‰ñ‚Ì‚Ý
	if (m_ModelCenter.x == 0 && m_ModelCenter.y == 0 && m_ModelCenter.z == 0)
	{
		if (const MODEL* model = ObjModelManager::GetModel(m_Model))
		{
			m_ModelCenter = model->Center;
			m_ModelScale = model->Scale;
		}

		m_ColliPosition = m_Position;
		m_ColliRotation = m_Rotation;
		m_ColliScale = m_Scale;
	}
	// Model‚ÌCenter‚âScale‚ðŠi”[‚µ‚½‚ç
	UpdateBoxCollisionInfo();
}

void StaticMeshObject::Draw()
{
	GameObject::Draw();

	//ƒ}ƒeƒŠƒAƒ‹Ý’è
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = m_Color;
	material.TextureEnable = false;

	Renderer::SetMaterial(material);

	// TODO,HACK :•ÏX—\’è / FBXRenderer‚Æ“¯‚¶Œ`Ž®‚É
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
