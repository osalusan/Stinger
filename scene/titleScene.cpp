#include "titleScene.h"
#include "main/main.h"
#include "manager/objectManager.h"
#include "manager/sceneManager.h"
#include "manager/inputManager.h"
#include "manager/textureManager.h"
#include "polygon2D/polygon2D.h"
#include "scene/gameScene.h"
#include "camera/titleCamera.h"
#include "skydome/skydome.h"

void TitleScene::Init()
{
	Scene::Init();
	AddCameraObject<TitleCamera>();

	if (m_ObjectManager != nullptr)
	{
		m_ObjectManager->AddGameObject<SkyDome>(OBJECT::SKYDOME);
		m_ObjectManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D,
			XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), PIVOT::CENTER, TEXTURE::TITLE, L"asset\\texture\\T_black.png");
	}
}

void TitleScene::Update(const float& deltaTime)
{
	Scene::Update(deltaTime);
	if (InputManager::GetKeyPress('2'))
	{
		SceneManager::SetScene<GameScene>();
	}
}

