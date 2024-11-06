#include "gameScene.h"
#include "manager/objectManager.h"
#include "manager/inputManager.h"
#include "manager/sceneManager.h"
#include "manager/textureManager.h"
#include "camera/playerCamera.h"
#include "skydome/skydome.h"
#include "polygon2D/polygon2D.h"
#include "scene/titleScene.h"

void GameScene::Init()
{
	Scene::Init();

	if (m_ObjectManager == nullptr) return;


	m_ObjectManager->AddPlayer();
	
	// プレイヤーの次に作成
	if (m_Camera == nullptr)
	{
		m_Camera = new PlayerCamera;
	}
	if (m_Camera != nullptr)
	{
		m_Camera->Init();
	}

	// プレイヤーの後に
	m_ObjectManager->AddGameObject<SkyDome>(OBJECT::SKYDOME);
	
}

void GameScene::Update(const float& deltaTime)
{
	Scene::Update(deltaTime);

	// TODO：動作確認用の仮処理　削除予定
	if (InputManager::GetKeyPress('1'))
	{
		SceneManager::SetScene<TitleScene>();
	}
}