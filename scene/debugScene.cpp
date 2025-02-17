#include "debugScene.h"
#include "manager/objectManager.h"
#include "manager/inputManager.h"
#include "manager/sceneManager.h"
#include "manager/textureManager.h"
#include "camera/playerCamera.h"
#include "skydome/skydome.h"
#include "polygon2D/polygon2D.h"
#include "scene/titleScene.h"
#include "scene/gameScene.h"
#include "staticMeshObject/box.h"
#include "character/mawJLaygo.h"
#include "behaviorTree/mawJLaygoBattleTree.h"
#include "behaviorTree/mawJLaygoLoadTree.h"
#include "billboard/billboard.h"

void DebugScene::Init()
{
	Scene::Init();


	if (m_ObjectManager == nullptr) return;

	m_ObjectManager->AddGameObjectArg<Player>(OBJECT::PLAYER, XMFLOAT3(0.0f, 0.0f, 0.0f));
	// プレイヤーの次に作成
	m_ObjectManager->AddGameObject<PlayerCamera>(OBJECT::CAMERA_MAIN);
	// カメラの後に作成
	m_ObjectManager->AddGameObjectArg<MawJLaygo>(OBJECT::BOSS, new MawJLaygoLoadTree, XMFLOAT3(-20.0f, 0.0f, 10.0f));

	// プレイヤーの後に
	m_ObjectManager->AddGameObject<SkyDome>(OBJECT::SKYDOME);
}

void DebugScene::Update(const float& deltaTime)
{
	Scene::Update(deltaTime);

#if _DEBUG
	if (InputManager::GetKeyPress('1'))
	{
		SceneManager::SetScene<TitleScene>();
	}
	if (InputManager::GetKeyPress('2'))
	{
		SceneManager::SetScene<GameScene>();
	}
#endif // _DEBUG
}
