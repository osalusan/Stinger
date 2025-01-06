#include "loadScene.h"
#include "manager/objectManager.h"
#include "manager/textureManager.h"
#include "manager/inputManager.h"
#include "camera/loadCamera.h"
#include "polygon2D/polygon2D.h"
#include "skydome/skydome.h"
#include "character/mawJLaygo.h"
#include "behaviorTree/mawJLaygoLoadTree.h"

void LoadScene::Init()
{
	Scene::Init();

	if (m_ObjectManager == nullptr) return;
	m_ObjectManager->CreateCameraObject<LoadCamera>();

	m_ObjectManager->CreateBossEnemy<MawJLaygo>(new MawJLaygoLoadTree, XMFLOAT3(0.0f, 0.0f, 40.0f), XMFLOAT3(0.05f, 0.05f, 0.05f));

	m_ObjectManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D,
		XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), XMFLOAT2(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.2f), PIVOT::RIGHT_BOTTOM, TEXTURE::LOADING, L"asset\\texture\\loading.png");
	m_ObjectManager->AddGameObject<SkyDome>(OBJECT::SKYDOME);
}

void LoadScene::Update(const float& deltaTime)
{
	Scene::Update(deltaTime);
}
