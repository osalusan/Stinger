#include "gameScene.h"
#include "manager/objectManager.h"
#include "manager/inputManager.h"
#include "manager/sceneManager.h"
#include "manager/textureManager.h"
#include "manager/audioManager.h"
#include "camera/playerCamera.h"
#include "skydome/skydome.h"
#include "polygon2D/polygon2D.h"
#include "scene/titleScene.h"
#include "staticMeshObject/box.h"
#include "character/mawJLaygo.h"
#include "behaviorTree/mawJLaygoBattleTree.h"
#include "object/transparentWall.h"

constexpr XMFLOAT3 WALL_MAX = { 250.0f, 100.0f, 250.0f }; // 壁の最大範囲
constexpr float CLEAR_TO_TITLE = 5.0f;
constexpr float INOPERABLE_TIME = 1.0f;
constexpr XMFLOAT2 RETRY_AND_GOTITLE_SCALE = { SCREEN_WIDTH * 0.28f,SCREEN_HEIGHT * 0.12f };
constexpr XMFLOAT2 RETRY_POS = { SCREEN_WIDTH * 0.3f,SCREEN_HEIGHT * 0.85f };
constexpr XMFLOAT2 GOTITLE_POS = { SCREEN_WIDTH * 0.7f,SCREEN_HEIGHT * 0.85f };
void GameScene::Init()
{
	Scene::Init();
	AudioManager::ReservAudio(AUDIO::MAWJ_BATTLE_BGM, "asset\\audio\\bgm\\rage_of_the_Forest.wav",true,true);
	m_WorldWall = WALL_MAX;

	if (m_ObjectManager == nullptr) return;

	m_ObjectManager->AddGameObjectArg<Player>(OBJECT::PLAYER,XMFLOAT3(0.0f,0.0f,0.0f));
	// プレイヤーの次に作成
	m_ObjectManager->AddGameObject<PlayerCamera>(OBJECT::CAMERA_MAIN);
	// カメラの後に作成
	m_ObjectManager->AddGameObjectArg<MawJLaygo>(OBJECT::BOSS,new MawJLaygoBattleTree,XMFLOAT3(-20.0f, 0.0f, 50.0f));

	// プレイヤーの後に
	m_ObjectManager->AddGameObject<SkyDome>(OBJECT::SKYDOME);

	// どこに作成しても良い
	m_ObjectManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D,
		XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), PIVOT::CENTER, TEXTURE::GAME_CLEAR,false, L"asset\\texture\\gameClear.png",false);	
	m_ObjectManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D,
		XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), PIVOT::CENTER, TEXTURE::GAME_OVER,false, L"asset\\texture\\gameOver.png",false);
	m_ObjectManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D,
		RETRY_POS, RETRY_AND_GOTITLE_SCALE, PIVOT::CENTER, TEXTURE::SELECTBACK1, false, L"asset\\texture\\white.png", false);
	m_ObjectManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D,
		GOTITLE_POS, RETRY_AND_GOTITLE_SCALE, PIVOT::CENTER, TEXTURE::SELECTBACK2, false, L"asset\\texture\\white.png", false);
	m_ObjectManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D,
		RETRY_POS, RETRY_AND_GOTITLE_SCALE, PIVOT::CENTER, TEXTURE::RETRY, false, L"asset\\texture\\retry.png", false);
	m_ObjectManager->AddGameObjectArg<Polygon2D>(OBJECT::POLYGON2D,
		GOTITLE_POS, RETRY_AND_GOTITLE_SCALE, PIVOT::CENTER, TEXTURE::GO_TITLE, false, L"asset\\texture\\goTitle.png", false);

	// ワールド限界を可視化
	m_ObjectManager->AddGameObjectArg<TransparentWall>(OBJECT::FILED, XMFLOAT3(WALL_MAX.z, 0.0f, 0.0f), XMFLOAT3(5.0f, WALL_MAX.y, WALL_MAX.z));
	m_ObjectManager->AddGameObjectArg<TransparentWall>(OBJECT::FILED, XMFLOAT3(-WALL_MAX.z, 0.0f, 0.0f), XMFLOAT3(5.0f,WALL_MAX.y, WALL_MAX.z));
	m_ObjectManager->AddGameObjectArg<TransparentWall>(OBJECT::FILED, XMFLOAT3(0.0f, 0.0f, WALL_MAX.x), XMFLOAT3(WALL_MAX.x,WALL_MAX.y,5.0f));
	m_ObjectManager->AddGameObjectArg<TransparentWall>(OBJECT::FILED, XMFLOAT3(0.0f, 0.0f, -WALL_MAX.x), XMFLOAT3(WALL_MAX.x,WALL_MAX.y,5.0f));
}

void GameScene::Update(const float& deltaTime)
{
	Scene::Update(deltaTime);

	if (m_ObjectManager == nullptr) return;

	Player* player = m_ObjectManager->GetPlayer();
	if (player == nullptr) return;
	BossEnemy* boss = m_ObjectManager->GetBossEnemy();
	if (boss == nullptr) return;

	// ゲームクリア画面
	if (boss->GetHealth() <= 0.0f && m_CurrentGameOverTime == 0.0f)
	{
		m_CurrentClearTime += deltaTime;
		std::vector<GameObject*> objects = {};

		m_ObjectManager->GetGameObjects(objects, OBJECT::POLYGON2D);
		for (GameObject* obj : objects)
		{
			if (obj->GetEnable())
			{
				continue;
			}
			if (Polygon2D* poly = dynamic_cast<Polygon2D*>(obj))
			{
				if (poly->GetTexture() == TEXTURE::GAME_CLEAR)
				{
					poly->SetEnable(true);
				}
			}
		}

		if (m_CurrentClearTime >= CLEAR_TO_TITLE)
		{
			SceneManager::SetScene<TitleScene>();
		}
	}

	// ゲームオーバー画面
	if (player->GetHealth() <= 0.0f && m_CurrentClearTime == 0.0f)
	{
		m_CurrentGameOverTime += deltaTime;
		std::vector<GameObject*> objects = {};

		m_ObjectManager->GetGameObjects(objects, OBJECT::POLYGON2D);
		for (GameObject* obj : objects)
		{
			if (Polygon2D* poly = dynamic_cast<Polygon2D*>(obj))
			{
				if (poly->GetTexture() == TEXTURE::GAME_OVER)
				{
					poly->SetEnable(true);
				}
				else if (poly->GetTexture() == TEXTURE::RETRY)
				{
					poly->SetEnable(true);
				}
				else if (poly->GetTexture() == TEXTURE::GO_TITLE)
				{
					poly->SetEnable(true);
				}
				else if (poly->GetTexture() == TEXTURE::SELECTBACK1 || poly->GetTexture() == TEXTURE::SELECTBACK2)
				{
					if (m_SelectKey == 1)
					{
						if (poly->GetTexture() == TEXTURE::SELECTBACK1)
						{
							poly->SetEnable(true);
						}
						if (poly->GetTexture() == TEXTURE::SELECTBACK2)
						{
							poly->SetEnable(false);
						}
					}
					else if (m_SelectKey == 2)
					{
						if (poly->GetTexture() == TEXTURE::SELECTBACK1)
						{
							poly->SetEnable(false);
						}
						if (poly->GetTexture() == TEXTURE::SELECTBACK2)
						{
							poly->SetEnable(true);
						}
					}
					else
					{
						poly->SetEnable(false);
					}
				}
			}
		}

		if (InputManager::GetKeyPress('A'))
		{
			m_SelectKey = 1;
		}
		if (InputManager::GetKeyPress('D'))
		{
			m_SelectKey = 2;
		}

		if (InputManager::GetKeyPress(VK_SPACE) && m_CurrentGameOverTime >= INOPERABLE_TIME)
		{
			if (m_SelectKey == 1)
			{
				SceneManager::SetScene<GameScene>();		
			}
			else if (m_SelectKey == 2)
			{
				SceneManager::SetScene<TitleScene>();
			}
		}
	}
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