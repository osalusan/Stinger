#include "objectManager.h"
#include "character/player.h"

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

	delete m_Player;
	m_Player = nullptr;
}

void ObjectManager::Init()
{
	if (m_Player != nullptr)
	{
		m_Player->Init();
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

	if (m_Player != nullptr)
	{
		m_Player->Uninit();
	}
}

void ObjectManager::Update(const float& deltaTime)
{
	if (m_Player != nullptr)
	{
		m_Player->Update(deltaTime);
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
	if (m_Player != nullptr)
	{
		m_Player->Draw();
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
