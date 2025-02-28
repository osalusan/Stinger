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
	float deltaTimeSlow = 0.0f;

	if (m_SlowTime > 0.0f)
	{
		m_SlowTime -= deltaTime;
	}
	else
	{
		m_SlowValue = 1.0f;
	}

	for (int layer = 0; layer < static_cast<int>(OBJECT::MAX); layer++)
	{
		deltaTimeSlow = deltaTime;

		if (m_SlowTime > 0.0f)
		{
			// “G‚Ì‚Ý‘¬“x‚ð•Ï‚¦‚é
			if (m_SlowEnemy)
			{
				if (layer == static_cast<int>(OBJECT::BOSS))
				{
					deltaTimeSlow *= m_SlowValue;
				}

				if (m_SlowTime <= 0.0f)
				{
					m_SlowEnemy = false;
				}
			}
			else
			{
				if (layer != static_cast<int>(OBJECT::CAMERA_MAIN))
				{
					deltaTimeSlow *= m_SlowValue;
				}
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

void ObjectManager::SetSlowTime(const float& time)
{
	m_SlowTime = time;
	m_SlowEnemy = false;
}

void ObjectManager::SetSlowTimeEnemy(const float& time)
{
	m_SlowTime = time;
	m_SlowEnemy = true;
}
