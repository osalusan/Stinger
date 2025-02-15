#include "lightningBattlecryTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"
#include "scene/scene.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "enemyAttackObject/lightningFall.h"

constexpr int LIGHTNINGFALL_NUM1 = 8;
constexpr int LIGHTNINGFALL_NUM2 = 12;
constexpr int LIGHTNINGFALL_NUM3 = 18;
constexpr int MAX_LINGHTNINGFALL = LIGHTNINGFALL_NUM1 + LIGHTNINGFALL_NUM2 + LIGHTNINGFALL_NUM3;
constexpr float SPAWN_LIGHTNINGFALL1_VALUE = 0.29f;
constexpr float SPAWN_LIGHTNINGFALL2_VALUE = 0.59f;
constexpr float SPAWN_LIGHTNINGFALL3_VALUE = 0.88f;
constexpr float RADIUS = 10.0f;
LightningBattlecryTask::~LightningBattlecryTask()
{
	m_LightningFallCache.clear();
}

void LightningBattlecryTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\standingBattlecry_MawJ.fbx", "standingBattlecry");
	m_TaskName = "ライトニングバースト";
	InitSkillData(m_TaskName);

	Scene* scene = SceneManager::GetScene();
	if (scene == nullptr) return;
	ObjectManager* objManager = scene->GetObjectManager();
	if (objManager == nullptr) return;

	if (m_LightningFallCache.size() <= 0)
	{
		for (int i = 0; i< MAX_LINGHTNINGFALL;i++)
		{
			m_LightningFallCache.emplace_back(objManager->AddGameObjectArg<LightningFall>(OBJECT::ATTACK, nullptr));
		}
	}
}

NODE_STATE LightningBattlecryTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATE::FAILURE;
	}

	BehaviorNode* node = m_BossCache->GetRunningNode();
	if (!CheckRunningNode(node))
	{
		return NODE_STATE::FAILURE;
	}

	// 初期化
	if (node == nullptr)
	{
		m_CurrentTime = 0.0f;
		m_UseLingtning = 0;
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimName);
		// 状態を保存
		m_BossCache->SetRunningNode(this);

		if (m_UseLingtning == 0 && m_CurrentTime >= m_MaxAnimTime * SPAWN_LIGHTNINGFALL1_VALUE)
		{
			const float& damage = m_BossCache->GetAttack() * m_DamageValue;

			float angleStep = 2.0f * static_cast<float>(XM_PI) / static_cast<float>(LIGHTNINGFALL_NUM1);
			int i = 0;

			for (LightningFall* fall : m_LightningFallCache)
			{
				if (fall->GetEnable())
				{
					continue;
				}
				float angle = angleStep * i;

				float offsetX = std::cos(angle) * RADIUS;
				float offsetZ = std::sin(angle) * RADIUS;

				XMFLOAT3 spawnPos = {};
				spawnPos.x = m_BossCache->GetPos().x + offsetX;
				spawnPos.y = m_BossCache->GetPos().y;
				spawnPos.z = m_BossCache->GetPos().z + offsetZ;

				fall->Spawn(spawnPos, damage, 1.0f);

				i++;

				if (i >= LIGHTNINGFALL_NUM1)
				{
					break;
				}
			}
			m_UseLingtning++;
		}
		else if (m_UseLingtning == 1 && m_CurrentTime >= m_MaxAnimTime * SPAWN_LIGHTNINGFALL2_VALUE)
		{
			const float& damage = m_BossCache->GetAttack() * m_DamageValue;

			float angleStep = 2.0f * static_cast<float>(XM_PI) / static_cast<float>(LIGHTNINGFALL_NUM2);
			int i = 0;

			for (LightningFall* fall : m_LightningFallCache)
			{
				if (fall->GetEnable())
				{
					continue;
				}
				float angle = angleStep * i;

				float offsetX = std::cos(angle) * (RADIUS * 2);
				float offsetZ = std::sin(angle) * (RADIUS * 2);

				XMFLOAT3 spawnPos = {};
				spawnPos.x = m_BossCache->GetPos().x + offsetX;
				spawnPos.y = m_BossCache->GetPos().y;
				spawnPos.z = m_BossCache->GetPos().z + offsetZ;

				fall->Spawn(spawnPos, damage, 1.0f);

				i++;

				if (i >= LIGHTNINGFALL_NUM2)
				{
					break;
				}
			}
			m_UseLingtning++;
		}
		else if (m_UseLingtning == 2 && m_CurrentTime >= m_MaxAnimTime * SPAWN_LIGHTNINGFALL3_VALUE)
		{
			const float& damage = m_BossCache->GetAttack() * m_DamageValue;

			float angleStep = 2.0f * static_cast<float>(XM_PI) / static_cast<float>(LIGHTNINGFALL_NUM3);
			int i = 0;

			for (LightningFall* fall : m_LightningFallCache)
			{
				if (fall->GetEnable())
				{
					continue;
				}
				float angle = angleStep * i;

				float offsetX = std::cos(angle) * (RADIUS * 3);
				float offsetZ = std::sin(angle) * (RADIUS * 3);

				XMFLOAT3 spawnPos = {};
				spawnPos.x = m_BossCache->GetPos().x + offsetX;
				spawnPos.y = m_BossCache->GetPos().y;
				spawnPos.z = m_BossCache->GetPos().z + offsetZ;

				fall->Spawn(spawnPos, damage, 1.0f);

				i++;

				if (i >= LIGHTNINGFALL_NUM3)
				{
					break;
				}
			}
			m_UseLingtning++;
		}

		return NODE_STATE::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// 状態を削除
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATE::SUCCESS;
		}
	}

	return NODE_STATE::FAILURE;
}