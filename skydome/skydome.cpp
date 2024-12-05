#include "skydome.h"
#include "scene/gameScene.h"
#include "manager/sceneManager.h"
#include "manager/objModelManager.h"
#include "camera/playerCamera.h"

SkyDome::SkyDome() :StaticMeshObject(STATICMESH_MODEL::SKYDOME)
{
	ObjModelManager::ReservModel(m_Model, "asset\\model\\sky.obj");
}

SkyDome::~SkyDome()
{
	m_CameraCashe = nullptr;
}

void SkyDome::Init()
{
	if (m_CameraCashe == nullptr)
	{
		StaticMeshObject::Init();
		m_Scale = { 400.0f,400.0f,400.0f };

		GameScene* Scene = SceneManager::GetScene<GameScene>();
		if (Scene == nullptr) return;

		m_CameraCashe = Scene->GetCamera();
	}
}

void SkyDome::Update(const float& deltaTime)
{
	if (m_CameraCashe != nullptr)
	{
		m_Position = m_CameraCashe->GetPos();
	}
}

void SkyDome::Draw()
{
	StaticMeshObject::Draw();
}
