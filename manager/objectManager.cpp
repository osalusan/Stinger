#include "objectManager.h"
#include "character/player.h"
#include "character/bossEnemy.h"
#include "meshFiled/meshFiled.h"
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
	if (m_Boss != nullptr)
	{
		m_Boss->Update(deltaTime);
	}

	for (int layer = 0; layer < static_cast<int>(OBJECT::MAX); layer++)
	{
		for (GameObject* object : m_GameObjects[layer])
		{
			if (object == nullptr) continue;

			object->Update(deltaTime);
		}
	}
}

void ObjectManager::Draw()
{
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

	for (int layer = 0; layer < static_cast<int>(OBJECT::MAX); layer++)
	{
		for (GameObject* object : m_GameObjects[layer])
		{
			if (object == nullptr) continue;

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
