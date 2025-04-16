#include "loadScene.h"
#include "manager/objectManager.h"
#include "manager/textureManager.h"
#include "manager/sceneManager.h"
#include "manager/objModelManager.h"
#include "camera/loadCamera.h"
#include "polygon2D/polygon2D.h"
#include "skydome/skydome.h"
#include "character/mawJLaygo.h"
#include "behaviorTree/mawJLaygoLoadTree.h"

void LoadScene::Init()
{
	Scene::Init();

	if (m_ObjectManager == nullptr) return;
	m_ObjectManager->AddGameObject<LoadCamera>(OBJECT::CAMERA_MAIN);

	m_ObjectManager->AddGameObjectArg<MawJLaygo>(OBJECT::BOSS, new MawJLaygoLoadTree, XMFLOAT3(0.0f, 0.0f, 20.0f), XMFLOAT3(0.05f, 0.05f, 0.05f),XMFLOAT3(0.0f,3.2f,0.0f));

	m_ObjectManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D,
		XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), XMFLOAT2(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.2f), PIVOT::RIGHT_BOTTOM, TEXTURE::LOADING, L"asset\\texture\\loading.png");
	m_ObjectManager->AddGameObjectArg<SkyDome>(OBJECT::SKYDOME, STATICMESH_MODEL::SKYDOME_TITLE, "asset\\model\\sky\\titleSky.obj");
}

void LoadScene::Update(const float& deltaTime)
{
	Scene::Update(deltaTime);
	if (SceneManager::GetLoadFinish())
	{
		m_Load = false;
	}
}
