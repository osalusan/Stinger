#include "airLightningBallTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"
#include "scene/scene.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "enemyAttackObject/lightningBall.h"

constexpr float SPAWN_LIGHTNINGBALL_VALUE = 0.37f;

void AirLightningBallTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\standing2HMagicAttack_MawJ.fbx", "standing2HMagicAttack");
	m_TaskName = "雷球発射";
	InitSkillData(m_TaskName);

	Scene* scene = SceneManager::GetScene();
	if (scene == nullptr) return;
	ObjectManager* objManager = scene->GetObjectManager();
	if (objManager == nullptr) return;

	if (m_LightningBallCache == nullptr)
	{
		m_LightningBallCache = objManager->AddGameObjectArg<LightningBall>(OBJECT::STATICMESH, objManager->GetPlayer(), m_BulletSpeed);
	}
}

NODE_STATE AirLightningBallTask::Update(const float& deltaTime)
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
		m_SpawnBall = false;
		m_UseDerivation = false;
		m_CurrentTime = 0.0f;
		DerivationChance();
	}

	// 派生技の発生確認
	if (!m_UseDerivation && m_CurrentTime > m_MaxAnimTime * GetDerivationData(m_UseDerivNumber).TransTimeValue)
	{
		if (GetDerivationData().size() > 0)
		{
			if (m_BossCache->GetHealth() <= m_BossCache->GetMaxHealth() * GetDerivationData(m_UseDerivNumber).Health)
			{
				m_UseDerivation = true;
				m_CurrentTime = m_MaxAnimTime;
				m_BossCache->SetRunningNode(nullptr);
			}
		}
	}

	// 派生技の発生確認後に配置
	if (m_UseDerivation)
	{
		// 一つのタスクのみ更新
		return UpdateUseDerivationTask(deltaTime);
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimName);
		// 状態を保存
		m_BossCache->SetRunningNode(this);

		if (!m_SpawnBall && m_CurrentTime >= m_MaxAnimTime * SPAWN_LIGHTNINGBALL_VALUE)
		{
			m_SpawnBall = true;
		}
		else if (!m_SpawnBall)
		{
			m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
			m_BossCache->InitGravity();
			if (m_LightningBallCache != nullptr)
			{
				const XMFLOAT3& bossPos = m_BossCache->GetPos();
				const XMFLOAT3& bossScale = m_BossCache->GetScale();
				const XMFLOAT3& bossFoward = m_BossCache->GetForward();
				// 胸の位置に調整
				const XMFLOAT3& spawnBulletPos = { bossPos.x + bossFoward.x * ((1.0f / bossScale.x) * 0.5f),bossPos.y + ((1.0f / bossScale.y) * 0.7f),bossPos.z + bossFoward.z * ((1.0f / bossScale.z) * 0.5f) };
				m_LightningBallCache->Attack(spawnBulletPos, m_BossCache->GetAttack() * m_DamageValue);
			}
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