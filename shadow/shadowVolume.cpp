#include "shadowVolume.h"
#include "renderer/fbxModelRenderer.h"
#include "renderer/objModelRenderer.h"
#include "component/boxCollisionComponent.h"
#include "manager/objModelManager.h"
#include "manager/fbxModelManager.h"
#include "manager/sceneManager.h"
#include "scene/scene.h"
#include "character/character.h"
#include "staticMeshObject/staticmeshObject.h"
#include "meshFiled/meshFiled.h"
#include "component/shaderComponent.h"

// 疑似的な光源
constexpr XMFLOAT3 LIGHT_ROT = { 0.0f,0.0f,0.0f };

ShadowVolume::ShadowVolume(StaticMeshObject* staticMesh)
{
	m_StaticModel = STATICMESH_MODEL::MAX;
	m_AnimeModel = ANIMETION_MODEL::MAX;

	if (m_StaticMeshCache != nullptr || staticMesh == nullptr) return;

	m_StaticMeshCache = staticMesh;
	m_StaticModel = m_StaticMeshCache->GetStaticModel();
}

ShadowVolume::ShadowVolume(Character* character)
{
	m_StaticModel = STATICMESH_MODEL::MAX;
	m_AnimeModel = ANIMETION_MODEL::MAX;
	if (m_CharacterCache != nullptr || character == nullptr) return;

	m_CharacterCache = character;
	m_AnimeModel = m_CharacterCache->GetAnimeModel();

	// GameObject::Init()の前に
	AddComponent<ShaderComponent>(this, "cso\\skinningVS.cso", "cso\\skinningPS.cso");
}

ShadowVolume::~ShadowVolume()
{
	delete m_ModelRenderer;
	m_ModelRenderer = nullptr;
}

void ShadowVolume::Init()
{
	GameObject::Init();
	// TODO :変更予定 / ObjModelManagerの修正後に変更
	if (m_StaticMeshCache != nullptr)
	{
		if (m_ModelRenderer == nullptr)
		{
			m_ModelRenderer = new ObjModelRenderer;
		}
	}

	// メッシュフィールドの生成より後に
	Scene* scene = SceneManager::GetScene<Scene>();
	if (scene == nullptr) return;
	ObjectManager* objectManager = scene->GetObjectManager();
	if (objectManager == nullptr) return;
	MeshFiled* filed = objectManager->GetMeshFiled();
	
	m_MeshFiled = filed;
}

void ShadowVolume::Update(const float& deltaTime)
{
	GameObject::Update(deltaTime);

	if (m_StaticMeshCache != nullptr)
	{
		m_Position.x = m_StaticMeshCache->GetPos().x;
		m_Position.z = m_StaticMeshCache->GetPos().z;
		m_Scale = m_StaticMeshCache->GetScale();
		m_Rotation = m_StaticMeshCache->GetRot();
		m_ModelCenter = m_StaticMeshCache->GetModelCenter();
		m_ModelScale = m_StaticMeshCache->GetModelScale();
	}
	else if (m_CharacterCache != nullptr)
	{
		m_Position.x = m_CharacterCache->GetPos().x;
		m_Position.z = m_CharacterCache->GetPos().z;
		m_Scale = m_CharacterCache->GetScale();
		m_Rotation = m_CharacterCache->GetRot();
		m_ModelCenter = m_CharacterCache->GetModelCenter();
		m_ModelScale = m_CharacterCache->GetModelScale();
	}

	m_Rotation.x += LIGHT_ROT.x;
	m_Rotation.y += LIGHT_ROT.y;
	m_Rotation.z += LIGHT_ROT.z;

	if (m_MeshFiled != nullptr)
	{
		m_Position.y = m_MeshFiled->GetHeight(m_Position) - (m_ModelScale.y * m_Scale.y);
	}
}

void ShadowVolume::Draw()
{
	GameObject::Draw();

	// シャドウ用の設定
	Renderer::SetBlendMaskEnable(true);
	Renderer::SetStencilEnable(true);
	Renderer::SetCullEnable(false);

	if (m_StaticMeshCache != nullptr)
	{
		if (m_ModelRenderer != nullptr)
		{
			if (const MODEL* model = ObjModelManager::GetModel(m_StaticModel))
			{
				m_ModelRenderer->Draw(model);
			}
		}
	}
	else if (m_CharacterCache != nullptr)
	{
		if (FbxModelRenderer* model = FbxModelManager::GetAnimationModel(m_AnimeModel))
		{
			model->Draw();
		}
	}

	// シャドウ用の設定
	Renderer::SetBlendMaskEnable(false);
	Renderer::SetDepthEnable(true);
	Renderer::SetCullEnable(true);
}

