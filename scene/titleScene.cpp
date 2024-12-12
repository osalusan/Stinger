#include "titleScene.h"
#include "main/main.h"
#include "manager/objectManager.h"
#include "manager/sceneManager.h"
#include "manager/inputManager.h"
#include "manager/textureManager.h"
#include "manager/particleManager.h"
#include "polygon2D/polygon2D.h"
#include "scene/gameScene.h"
#include "camera/titleCamera.h"
#include "skydome/skydome.h"

#include "staticMeshObject/box.h"
#include "particle/spiralBlueorb.h"
#include "meshFiled/meshFiled.h"
#include "shadow/shadowVolume.h"
#include "manager/objModelManager.h"
#include "manager/fbxModelManager.h"
#include "behaviorTree/mawJLaygoTitleTree.h"
#include "character/mawJLaygo.h"

void TitleScene::Init()
{
	Scene::Init();

	if (m_ObjectManager == nullptr) return;

	m_ObjectManager->CreateCameraObject<TitleCamera>();
	m_ObjectManager->CreateMeshFiled<MeshFiled>();
	m_ObjectManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D,
		XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), PIVOT::CENTER, TEXTURE::TITLE, true, L"asset\\texture\\T_black.png");
	m_ObjectManager->AddGameObject<SkyDome>(OBJECT::SKYDOME);
	//m_ObjectManager->AddGameObjectArg<Box>(OBJECT::STATICMESH, XMFLOAT3(0.0f, 0.0f, 5.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 1.0f, 0.0f));

	m_ObjectManager->CreateBossEnemy<MawJLaygo>(new MawJLaygoTitleTree, XMFLOAT3(5.0f, 0.0f, 15.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 3.14f, 0.0f));

	m_ObjectManager->AddGameObjectArg<ShadowVolume>(OBJECT::STATICMESH, ANIMETION_MODEL::PLAYER);

	// オブジェクトの追加後に配置
	CreateParticleManager();
	if (m_ParticleManager == nullptr) return;

	m_ParticleManager->AddParticleObjectArg<SpiralBlueOrb>(PARTICLE::SPIRAL_BLUEORB,XMFLOAT3(0.0f,0.0f,15.0f));
}

void TitleScene::Update(const float& deltaTime)
{
	Scene::Update(deltaTime);
	if (InputManager::GetKeyPress('2'))
	{
		SceneManager::SetScene<GameScene>();
	}
}

