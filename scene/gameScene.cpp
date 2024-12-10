#include "gameScene.h"
#include "manager/objectManager.h"
#include "manager/inputManager.h"
#include "manager/sceneManager.h"
#include "manager/textureManager.h"
#include "camera/playerCamera.h"
#include "skydome/skydome.h"
#include "polygon2D/polygon2D.h"
#include "scene/titleScene.h"
#include "staticMeshObject/box.h"
#include "character/mawJLaygo.h"

GameScene::~GameScene()
{
	delete m_Camera;
	m_Camera = nullptr;
}

void GameScene::Init()
{
	Scene::Init();

	if (m_ObjectManager == nullptr) return;

	m_ObjectManager->CreatePlayer();
	m_ObjectManager->CreateBossEnemy<MawJLaygo>(XMFLOAT3(-20.0f, 0.0f, 10.0f));
	
	// プレイヤーの次に作成
	AddCameraObject<PlayerCamera>();

	// プレイヤーの後に
	m_ObjectManager->AddGameObject<SkyDome>(OBJECT::SKYDOME);
	m_ObjectManager->AddGameObjectArg<Box>(OBJECT::STATICMESH,XMFLOAT3(0.0f,0.0f,5.0f),XMFLOAT3(4.0f,4.0f,4.0f), XMFLOAT3(1.0f, 1.0f, 0.0f));
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