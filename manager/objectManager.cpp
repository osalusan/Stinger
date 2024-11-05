#include "objectManager.h"
#include "character/player.h"

ObjectManager::~ObjectManager()
{
	delete m_Player;
	m_Player = nullptr;
}

void ObjectManager::Init()
{
	if (m_Player == nullptr)
	{
		m_Player = new Player;
	}
	if (m_Player != nullptr)
	{
		m_Player->Init();
	}

	for (int Layer = 0; Layer < static_cast<int>(OBJECT::MAX); Layer++)
	{
		for (GameObject* Object : m_GameObjects[Layer])
		{
			if (Object == nullptr) continue;

			Object->Init();
		}
	}
}

void ObjectManager::Uninit()
{
	if (m_Player != nullptr)
	{
		m_Player->Uninit();
	}

	for (int Layer = 0; Layer < static_cast<int>(OBJECT::MAX); Layer++)
	{
		for (GameObject* Object : m_GameObjects[Layer])
		{
			if (Object == nullptr) continue;

			Object->Uninit();
		}
	}
}

void ObjectManager::Update(const float& deltaTime)
{
	if (m_Player != nullptr)
	{
		m_Player->Update(deltaTime);
	}

	for (int Layer = 0; Layer < static_cast<int>(OBJECT::MAX); Layer++)
	{
		for (GameObject* Object : m_GameObjects[Layer])
		{
			if (Object == nullptr) continue;

			Object->Update(deltaTime);
		}
	}
}

void ObjectManager::Draw()
{
	if (m_Player != nullptr)
	{
		m_Player->Draw();
	}

	for (int Layer = 0; Layer < static_cast<int>(OBJECT::MAX); Layer++)
	{
		for (GameObject* Object : m_GameObjects[Layer])
		{
			if (Object == nullptr) continue;

			Object->Draw();
		}
	}
}
