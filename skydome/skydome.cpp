#include "skydome.h"
#include "scene/gameScene.h"
#include "manager/sceneManager.h"
#include "manager/objModelManager.h"
#include "manager/objectManager.h"
#include "camera/playerCamera.h"
#include "component/shaderComponent.h"

// 大きすぎても映らない / 小さすぎたらオブジェクトが映らない
constexpr XMFLOAT3 SKY_SCALE = { 1000.0f,1000.0f,1000.0f };

void SkyDome::MoveControl(const float& deltaTime)
{
	if (m_CameraCache != nullptr)
	{
		m_Position = m_CameraCache->GetPos();
	}
}

SkyDome::SkyDome(const STATICMESH_MODEL& model, std::string filePath)
	:StaticMeshObject(model)
{
	ObjModelManager::ReservModel(m_Model, filePath);
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

		m_Scale = SKY_SCALE;

		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;

		ObjectManager* objManager = scene->GetObjectManager();
		if (objManager == nullptr) return;

		m_CameraCache = objManager->GetCamera();
	}
}
