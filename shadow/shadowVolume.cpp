#include "shadowVolume.h"
#include "manager/objModelManager.h"
#include "renderer/objModelRenderer.h"
#include "component/boxCollisionComponent.h"
#include "manager/objModelManager.h"
#include "manager/fbxModelManager.h"

ShadowVolume::ShadowVolume(const STATICMESH_MODEL& model)
{
	m_StaticModel = model;
	m_AnimeModel = ANIMETION_MODEL::MAX;
	ObjModelManager::ReservModel(m_StaticModel, "asset\\model\\box.obj");

	m_Position = {0.0f,0.0f,3.0f};
}

ShadowVolume::~ShadowVolume()
{
	delete m_ModelRenderer;
	m_ModelRenderer = nullptr;
}

void ShadowVolume::Init()
{
	GameObject::Init();
	if (m_ModelRenderer == nullptr)
	{
		m_ModelRenderer = new ObjModelRenderer;
	}
}

void ShadowVolume::Update(const float& deltaTime)
{
	GameObject::Update(deltaTime);
	// “–‚½‚è”»’èˆ—‚Ì‘O‚É / ‰‰ñ‚Ì‚Ý
	if (m_ModelCenter.x == 0 && m_ModelCenter.y == 0 && m_ModelCenter.z == 0)
	{
		if (const MODEL* model = ObjModelManager::GetModel(m_StaticModel))
		{
			m_ModelCenter = model->Center;
			m_ModelScale = model->Scale;
		}

		m_ColliPosition = m_Position;
		m_ColliRotation = m_Rotation;
		m_ColliScale = m_Scale;
	}
	m_Rotation.y += 0.02f;
	m_Rotation.x += 0.02f;
	// Model‚ÌCenter‚âScale‚ðŠi”[‚µ‚½‚ç
	UpdateBoxCollisionInfo();
}

void ShadowVolume::Draw()
{
	GameObject::Draw();

	//Renderer::SetBlendMaskEnable(true);
	//Renderer::SetStencilEnable(true);
	Renderer::SetCullEnable(false);
	if (m_ModelRenderer != nullptr)
	{
		if (const MODEL* model = ObjModelManager::GetModel(m_StaticModel))
		{
			m_ModelRenderer->Draw(model);
		}
	}
	//Renderer::SetBlendMaskEnable(false);
	//Renderer::SetDepthEnable(true);
	Renderer::SetCullEnable(true);
}

