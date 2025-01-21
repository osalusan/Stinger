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
	m_BossCache = nullptr;
	m_PlayerCache = nullptr;
	m_FiledCache = nullptr;
	m_CameraCache = nullptr;
}

void ObjectManager::Init()
{
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
}

void ObjectManager::Update(const float& deltaTime)
{
	float deltaTimeSlow = deltaTime;

	for (int layer = 0; layer < static_cast<int>(OBJECT::MAX); layer++)
	{
		if (m_SlowTime > 0.0f)
		{
			if (layer == static_cast<int>(OBJECT::PLAYER) || layer == static_cast<int>(OBJECT::BOSS))
			{
				m_SlowTime -= deltaTime;
				deltaTimeSlow *= m_SlowValue;
			}
		}

		for (GameObject* object : m_GameObjects[layer])
		{
			if (object == nullptr) continue;
			if (!object->GetEnable())continue;

			object->Update(deltaTimeSlow);
		}
	}
}

void ObjectManager::Draw()
{
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
