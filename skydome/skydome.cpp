#include "skydome.h"
#include "scene/gameScene.h"
#include "manager/sceneManager.h"
#include "camera/playerCamera.h"

SkyDome::~SkyDome()
{
	m_Camera = nullptr;
}

void SkyDome::Init()
{
	if (m_Camera == nullptr)
	{
		StaticMeshObject::Init();
		m_Scale = { 100.0f,100.0f,100.0f };

		GameScene* Scene = SceneManager::GetScene<GameScene>();
		if (Scene == nullptr) return;

		PlayerCamera* PlayerCameraObject = Scene->GetPlayerCamera();

		m_Camera = std::shared_ptr<PlayerCamera>(PlayerCameraObject, [](PlayerCamera*)
			{
				// delete‚µ‚È‚¢‚æ‚¤‚É
			});
	}
}

void SkyDome::Update(const float& deltaTime)
{
	if (m_Camera != nullptr)
	{
		m_Position = m_Camera->GetPos();
	}
}

void SkyDome::Draw()
{
	StaticMeshObject::Draw();
}

// ------------------------------- private -------------------------------
void SkyDome::LoadStaticMesh()
{
	LoadModel("asset\\model\\sky.obj");
}