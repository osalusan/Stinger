#include "gameScene.h"
#include "manager/objectManager.h"
#include "manager/inputManager.h"
#include "manager/sceneManager.h"
#include "manager/textureManager.h"
#include "manager/particleManager.h"
#include "camera/playerCamera.h"
#include "skydome/skydome.h"
#include "polygon2D/polygon2D.h"
#include "scene/titleScene.h"
#include "staticMeshObject/box.h"
#include "character/mawJLaygo.h"
#include "behaviorTree/mawJLaygoBattleTree.h"
#include "staticMeshObject/transparentWall.h"

constexpr XMFLOAT3 WALL_MAX = { 150.0f, 100.0f, 150.0f }; // 壁の最大範囲
void GameScene::Init()
{
	Scene::Init();
	m_WorldWall = WALL_MAX;

	if (m_ObjectManager == nullptr) return;

	m_ObjectManager->AddGameObjectArg<Player>(OBJECT::PLAYER,XMFLOAT3(0.0f,0.0f,0.0f));
	m_ObjectManager->AddGameObjectArg<MawJLaygo>(OBJECT::BOSS,new MawJLaygoBattleTree,XMFLOAT3(-20.0f, 0.0f, 10.0f));
	
	// プレイヤーの次に作成
	m_ObjectManager->AddGameObject<PlayerCamera>(OBJECT::CAMERA_MAIN);

	// プレイヤーの後に
	m_ObjectManager->AddGameObject<SkyDome>(OBJECT::SKYDOME);
	m_ObjectManager->AddGameObjectArg<Box>(OBJECT::STATICMESH,XMFLOAT3(0.0f,0.0f,5.0f),XMFLOAT3(4.0f,4.0f,4.0f), XMFLOAT3(1.0f, 1.0f, 0.0f));
	// ワールド限界を可視化
	m_ObjectManager->AddGameObjectArg<TransparentWall>(OBJECT::FILED, XMFLOAT3(WALL_MAX.z, 0.0f, 0.0f), XMFLOAT3(5.0f, WALL_MAX.y, WALL_MAX.z));
	m_ObjectManager->AddGameObjectArg<TransparentWall>(OBJECT::FILED, XMFLOAT3(-WALL_MAX.z, 0.0f, 0.0f), XMFLOAT3(5.0f,WALL_MAX.y, WALL_MAX.z));
	m_ObjectManager->AddGameObjectArg<TransparentWall>(OBJECT::FILED, XMFLOAT3(0.0f, 0.0f, WALL_MAX.x), XMFLOAT3(WALL_MAX.x,WALL_MAX.y,5.0f));
	m_ObjectManager->AddGameObjectArg<TransparentWall>(OBJECT::FILED, XMFLOAT3(0.0f, 0.0f, -WALL_MAX.x), XMFLOAT3(WALL_MAX.x,WALL_MAX.y,5.0f));
	// オブジェクトの追加後に配置
	CreateParticleManager();
	if (m_ParticleManager == nullptr) return;


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