#include "lightningFallFowardRainTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"
#include "scene/scene.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "enemyAttackObject/lightningFall.h"

constexpr int LINGHTNINGFALL_NUM_X = 4;
constexpr int LINGHTNINGFALL_NUM_Z = 8;
constexpr int MAX_LINGHTNINGFALL = LINGHTNINGFALL_NUM_X * LINGHTNINGFALL_NUM_Z;
constexpr float DISTANCE = 9.2f;
constexpr float SPAWN_LIGHTNINGFALL_VALUE = 0.58f;

LightningFallFowardRainTask::~LightningFallFowardRainTask()
{
	m_LightningFallCache.clear();
}
void LightningFallFowardRainTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\standing2HCastSpell01_MawJ.fbx", "standing2HCastSpell01");
	m_TaskName = "ライトニングフォワードレイン";
	InitSkillData(m_TaskName);
	m_AttackTask = true;

	Scene* scene = SceneManager::GetScene();
	if (scene == nullptr) return;
	ObjectManager* objManager = scene->GetObjectManager();
	if (objManager == nullptr) return;

	if (m_LightningFallCache.size() <= 0)
	{
		for (int i = 0; i < MAX_LINGHTNINGFALL; i++)
		{
			m_LightningFallCache.emplace_back(objManager->AddGameObjectArg<LightningFall>(OBJECT::ATTACK, nullptr));
		}
	}
}

void LightningFallFowardRainTask::InitTask(const float& deltaTime)
{
	TaskNode::InitTask(deltaTime);
	m_SpawnLingtning = false;
}

void LightningFallFowardRainTask::RunningTask(const float& deltaTime)
{
	TaskNode::RunningTask(deltaTime);

	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return;
	}

	if (!m_SpawnLingtning && m_CurrentTime >= m_MaxAnimTime * SPAWN_LIGHTNINGFALL_VALUE)
	{
		const float damage = m_BossCache->GetAttack() * m_DamageValue;

		XMFLOAT3 bossForward = m_BossCache->GetForward();
		XMFLOAT3 bossRight = m_BossCache->GetRight();
		XMFLOAT3 bossPos = m_BossCache->GetPos();

		int x = 0;
		int z = 0;

		for (LightningFall* fall : m_LightningFallCache)
		{
			if (fall->GetEnable())
			{
				continue;
			}

			XMFLOAT3 spawnPos;
			spawnPos.x = bossPos.x + bossForward.x * (z * DISTANCE) + bossRight.x * ((x - 1) * DISTANCE);
			spawnPos.y = bossPos.y;
			spawnPos.z = bossPos.z + bossForward.z * (z * DISTANCE) + bossRight.z * ((x - 1) * DISTANCE);

			fall->Spawn(spawnPos, damage, m_BulletActionTime);

			z++;

			if (z >= LINGHTNINGFALL_NUM_Z)
			{
				x++;
				z = 0;

				if (x >= LINGHTNINGFALL_NUM_X)
				{
					break;
				}
			}
		}
		m_SpawnLingtning = true;
	}
	m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
}
