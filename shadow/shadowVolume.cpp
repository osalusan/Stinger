#include "shadowVolume.h"
#include "renderer/fbxModelRenderer.h"
#include "renderer/objModelRenderer.h"
#include "component/boxCollisionComponent.h"
#include "manager/objModelManager.h"
#include "manager/fbxModelManager.h"
#include "manager/sceneManager.h"
#include "scene/scene.h"
#include "character/character.h"
#include "staticMeshObject/staticMeshObject.h"
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

void ShadowVolume::Init()
{
	GameObject::Init();

	// メッシュフィールドの生成より後に
	Scene* scene = SceneManager::GetScene();
	if (scene == nullptr) return;
	ObjectManager* objectManager = scene->GetObjectManager();
	if (objectManager == nullptr) return;
	MeshFiled* filed = objectManager->GetMeshFiled();
	
	m_MeshFiled = filed;
}

void ShadowVolume::Update(const float& deltaTime)
{
	GameObject::Update(deltaTime);

	if (m_StaticMeshCache == nullptr) return;

	m_Position.x = m_StaticMeshCache->GetPos().x;
	m_Position.z = m_StaticMeshCache->GetPos().z;
	m_Scale = m_StaticMeshCache->GetScale();
	m_Rotation = m_StaticMeshCache->GetRot();

	m_Rotation.x += LIGHT_ROT.x;
	m_Rotation.y += LIGHT_ROT.y;
	m_Rotation.z += LIGHT_ROT.z;

	if (m_MeshFiled != nullptr)
	{
		if (ObjModelRenderer* model = ObjModelManager::GetModel(m_StaticMeshCache->GetStaticModel()))
		{
			if (model == nullptr) return;

			MODEL* modelData = model->GetModel();
			m_Position.y = m_MeshFiled->GetHeight(m_Position) - (modelData->Scale.y * m_Scale.y);
		}
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

		if (ObjModelRenderer* model = ObjModelManager::GetModel(m_StaticMeshCache->GetStaticModel()))
		{
			if (model == nullptr) return;

			model->Draw();
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

