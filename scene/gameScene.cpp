#include "gameScene.h"
#include "manager/objectManager.h"
#include "manager/inputManager.h"
#include "manager/sceneManager.h"
#include "camera/playerCamera.h"
#include "skydome/skydome.h"

GameScene::~GameScene()
{
	delete m_PlayerCamera;
	m_PlayerCamera = nullptr;
	delete m_ObjectManager;
	m_ObjectManager = nullptr;
}

void GameScene::Init()
{
	if (m_ObjectManager == nullptr)
	{
		m_ObjectManager = new ObjectManager;
	}
	if (m_ObjectManager != nullptr)
	{
		m_ObjectManager->Init();
	}

	// プレイヤーの後に作成
	if (m_PlayerCamera == nullptr)
	{
		m_PlayerCamera = new PlayerCamera;
	}
	if (m_PlayerCamera != nullptr)
	{
		m_PlayerCamera->Init();
	}

	// プレイヤーの後に
	if (m_ObjectManager != nullptr)
	{
		m_ObjectManager->AddGameObject<SkyDome>(OBJECT::SKYDOME);
	}
}

void GameScene::Uninit()
{
	if (m_PlayerCamera != nullptr)
	{
		m_PlayerCamera->Uninit();
	}
	if (m_ObjectManager != nullptr)
	{
		m_ObjectManager->Uninit();
	}
}

void GameScene::Update(const float& deltaTime)
{
	if (m_ObjectManager != nullptr)
	{
		m_ObjectManager->Update(deltaTime);
	}

	// プレイヤーの後に更新
	if (m_PlayerCamera != nullptr)
	{
		m_PlayerCamera->Update();
	}
	// TODO：動作確認用の仮処理　削除予定
	if (InputManager::GetKeyTrigger('1'))
	{
		SceneManager::SetScene<GameScene>();
	}
}

void GameScene::Draw()
{
	// 一番最初に描画
	if (m_PlayerCamera != nullptr)
	{
		m_PlayerCamera->Draw();
	}

	if (m_ObjectManager != nullptr)
	{
		m_ObjectManager->Draw();
	}
}
