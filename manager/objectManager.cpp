#include "objectManager.h"
#include "character/player.h"
#include "character/bossEnemy.h"
#include "meshFiled/meshFiled.h"
#include "camera/camera.h"
ObjectManager::~ObjectManager()
{
	for (int layer = 0; layer < static_cast<int>(OBJECT::MAX); layer++)
	{
		for (GameObject* object : m_GameObjects[layer])
		{
			delete object;
			object = nullptr;
		}
	}
	m_GameObjects->clear();
	delete m_Boss;
	m_Boss = nullptr;
	delete m_Player;
	m_Player = nullptr;
	delete m_Filed;
	m_Filed = nullptr;
	delete m_Camera;
	m_Camera = nullptr;
}

void ObjectManager::Init()
{
	if (m_Filed != nullptr)
	{
		m_Filed->Init();
	}
	if (m_Player != nullptr)
	{
		m_Player->Init();
	}
	if (m_Boss != nullptr)
	{
		m_Boss->Init();
	}

	for (int layer = 0; layer < static_cast<int>(OBJECT::MAX); layer++)
	{
		for (GameObject* object : m_GameObjects[layer])
		{
			if (object == nullptr) continue;

			object->Init();
		}
	}
}

void ObjectManager::Uninit()
{
	for (int layer = 0; layer < static_cast<int>(OBJECT::MAX); layer++)
	{
		for (GameObject* object : m_GameObjects[layer])
		{
			if (object == nullptr) continue;

			object->Uninit();
		}
	}
	
	if (m_Boss != nullptr)
	{
		m_Boss->Uninit();
	}
	if (m_Player != nullptr)
	{
		m_Player->Uninit();
	}
	if (m_Filed != nullptr)
	{
		m_Filed->Uninit();
	}
	if (m_Camera != nullptr)
	{
		m_Camera->Uninit();
	}
}

void ObjectManager::Update(const float& deltaTime)
{
	if (m_Filed != nullptr)
	{
		m_Filed->Update();
	}
	if (m_Player != nullptr)
	{
		m_Player->Update(deltaTime);
	}
	// プレイヤーの後に更新
	if (m_Camera != nullptr)
	{
		m_Camera->Update();
	}
	if (m_Boss != nullptr)
	{
		m_Boss->Update(deltaTime);
	}

	for (int layer = 0; layer < static_cast<int>(OBJECT::MAX); layer++)
	{
		for (GameObject* object : m_GameObjects[layer])
		{
			if (object == nullptr) continue;
			if (!object->GetEnable())continue;

			object->Update(deltaTime);
		}
	}
}

void ObjectManager::Draw()
{
	// 一番最初に描画
	if (m_Camera != nullptr)
	{
		m_Camera->Draw();
	}

	if (m_Filed != nullptr)
	{
		m_Filed->Draw();
	}

	if (m_Player != nullptr)
	{
		m_Player->Draw();
	}
	if (m_Boss != nullptr)
	{
		m_Boss->Draw();
	}

	// TODO : 位置を変更予定 / Polygon2DがGameObjectを継承しなくなったら
	for (int layer = 0; layer < static_cast<int>(OBJECT::MAX); layer++)
	{
		for (GameObject* object : m_GameObjects[layer])
		{
			if (object == nullptr) continue;
			if (!object->GetEnable())continue;

			object->Draw();
		}
	}
}

void ObjectManager::CreatePlayer()
{
	if (m_Player == nullptr)
	{
		m_Player = new Player;
	}
	if (m_Player != nullptr)
	{
		m_Player->Init();
	}
}
