#include "titleScene.h"
#include "main/main.h"
#include "manager/objectManager.h"
#include "manager/sceneManager.h"
#include "manager/inputManager.h"
#include "manager/textureManager.h"
#include "manager/particleManager.h"
#include "manager/fbxModelManager.h"
#include "polygon2D/polygon2D.h"
#include "scene/gameScene.h"
#include "camera/titleCamera.h"
#include "skydome/skydome.h"
#include "particle/spiralBlueorb.h"
#include "meshFiled/meshFiled.h"
#include "shadow/shadowVolume.h"
#include "behaviorTree/mawJLaygoTitleTree.h"
#include "character/mawJLaygo.h"
#include "staticMeshObject/box.h"
#include "staticMeshObject/staticmeshObject.h"
#include "staticMeshObject/moveRandSizeBox.h"

constexpr float DEFAULT_POSZ = 60.0f;
constexpr float NEXT_SCENE_TIME = 3.0f;
void TitleScene::Init()
{
	Scene::Init();

	if (m_ObjectManager == nullptr) return;

	m_ObjectManager->AddGameObject<TitleCamera>(OBJECT::CAMERA_MAIN);
	m_ObjectManager->AddGameObject<MeshFiled>(OBJECT::FILED);
	m_ObjectManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D,
		XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), PIVOT::CENTER, TEXTURE::SHADOW, true, L"asset\\texture\\T_black.png");
	m_ObjectManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D,
		XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), PIVOT::CENTER, TEXTURE::TITLE, L"asset\\texture\\T_Stinger.png");
	m_ObjectManager->AddGameObject<SkyDome>(OBJECT::SKYDOME);
	
	//m_ObjectManager->AddGameObjectArg<Box>(OBJECT::STATICMESH, XMFLOAT3(0.0f, 0.0f, -10.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 1.0f, 0.0f));

	//m_ObjectManager->CreateBossEnemy<MawJLaygo>(new MawJLaygoTitleTree, XMFLOAT3(-20.0f, 0.0f, 20.0f), XMFLOAT3(0.05f, 0.05f, 0.05f), XMFLOAT3(0.0f, 2.64f, 0.0f));
	Camera* camera = m_ObjectManager->GetCamera();
	float cameraPosZ = camera->GetPos().z;
	for (int i = 0; i < 20; i++)
	{
		m_ObjectManager->AddGameObjectArg<MoveRandSizeBox>(OBJECT::STATICMESH, cameraPosZ, DEFAULT_POSZ);
	}
	// オブジェクトの追加後に配置
	CreateParticleManager();
	if (m_ParticleManager == nullptr) return;

	m_ParticleManager->AddParticleObjectArg<SpiralBlueOrb>(PARTICLE::SPIRAL_BLUEORB,XMFLOAT3(0.0f,4.0f, DEFAULT_POSZ),false);

	CreateShadowVolume();
}

void TitleScene::Update(const float& deltaTime)
{
	Scene::Update(deltaTime);
	if (InputManager::GetKeyPress('2'))
	{
		m_NextScene = true;
		std::vector<GameObject*> objects = {};

		if (m_ObjectManager == nullptr) return;

		m_ObjectManager->GetGameObjects(objects, OBJECT::POLYGON2D);

		for (GameObject* obj : objects)
		{
			if (Polygon2D* poly = dynamic_cast<Polygon2D*>(obj))
			{
				poly->SetEnable(false);
				if (poly->GetTexture() == TEXTURE::TITLE)
				{
					poly->SetEnable(false);
					break;
				}
			}
		}
		
		if (m_ParticleManager == nullptr) return;
		std::vector<ParticleEmiter*> particles = {};
		m_ParticleManager->GetParticleObjects(particles,PARTICLE::SPIRAL_BLUEORB);

		for (ParticleEmiter* particle : particles)
		{
			particle->SetEnable(true);
		}
	}
	if (m_NextScene)
	{
		if (m_NextSceneTime > NEXT_SCENE_TIME)
		{
			SceneManager::SetScene<GameScene>();
		}
		m_NextSceneTime += deltaTime;
	}
}

void TitleScene::CreateShadowVolume()
{
	if (m_ObjectManager == nullptr) return;

	//m_ObjectManager->AddGameObjectArg<ShadowVolume>(OBJECT::SHADOW, m_ObjectManager->GetBossEnemy());

	std::vector<GameObject*> staticMeshObj = {};
	m_ObjectManager->GetGameObjects(staticMeshObj,OBJECT::STATICMESH);

	for (GameObject* obj : staticMeshObj)
	{
		if (StaticMeshObject* staticMesh = dynamic_cast<StaticMeshObject*>(obj))
		{
			m_ObjectManager->AddGameObjectArg<ShadowVolume>(OBJECT::SHADOW, staticMesh);
		}
	}
}

