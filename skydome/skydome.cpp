#include "skydome.h"
#include "scene/gameScene.h"
#include "manager/sceneManager.h"
#include "manager/objModelManager.h"
#include "manager/objectManager.h"
#include "camera/playerCamera.h"
#include "component/shaderComponent.h"

SkyDome::SkyDome() :StaticMeshObject(STATICMESH_MODEL::SKYDOME)
{
	ObjModelManager::ReservModel(m_Model, "asset\\model\\sky.obj");
}

SkyDome::~SkyDome()
{
	m_CameraCache = nullptr;
}

void SkyDome::Init()
{
	AddComponent<ShaderComponent>(this);
	if (m_CameraCache == nullptr)
	{
		StaticMeshObject::Init();
		m_Scale = { 1000.0f,1000.0f,1000.0f };

		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;

		ObjectManager* objManager = scene->GetObjectManager();
		if (objManager == nullptr) return;

		m_CameraCache = objManager->GetCamera();
	}
}

void SkyDome::Update(const float& deltaTime)
{
	if (m_CameraCache != nullptr)
	{
		m_Position = m_CameraCache->GetPos();
	}
}