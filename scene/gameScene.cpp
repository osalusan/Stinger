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
	
	// �v���C���[�̎��ɍ쐬
	if (m_Camera == nullptr)
	{
		m_Camera = new PlayerCamera;
	}
	if (m_Camera != nullptr)
	{
		m_Camera->Init();
	}

	// �v���C���[�̌��
	m_ObjectManager->AddGameObject<SkyDome>(OBJECT::SKYDOME);
	
}

void GameScene::Update(const float& deltaTime)
{
	Scene::Update(deltaTime);

	// TODO�F����m�F�p�̉������@�폜�\��
	if (InputManager::GetKeyPress('1'))
	{
		SceneManager::SetScene<TitleScene>();
	}
}