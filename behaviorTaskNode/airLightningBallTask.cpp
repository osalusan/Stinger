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
	m_TaskName = "—‹‹…”­ŽË";
	InitSkillData(m_TaskName);
	m_AttackTask = true;

	Scene* scene = SceneManager::GetScene();
	if (scene == nullptr) return;
	ObjectManager* objManager = scene->GetObjectManager();
	if (objManager == nullptr) return;

	if (m_LightningBallCache == nullptr)
	{
		m_LightningBallCache = objManager->AddGameObjectArg<LightningBall>(OBJECT::ATTACK, objManager->GetPlayer(), m_BulletSpeed);
	}
}


void AirLightningBallTask::InitTask(const float& deltaTime)
{
	TaskNode::InitTask(deltaTime);
	m_Attack = false;
	const XMFLOAT3& bossPos = m_BossCache->GetPos();
	const XMFLOAT3& bossScale = m_BossCache->GetScale();
	const XMFLOAT3& bossFoward = m_BossCache->GetForward();
	const XMFLOAT3& spawnBulletPos = { bossPos.x + bossFoward.x * ((1.0f / bossScale.x) * 0.5f),bossPos.y + ((1.0f / bossScale.y) * 0.7f),bossPos.z + bossFoward.z * ((1.0f / bossScale.z) * 0.5f) };
	if (m_LightningBallCache != nullptr)
	{
		m_LightningBallCache->Spawn(spawnBulletPos, m_BossCache->GetAttack() * m_DamageValue);
	}
}

void AirLightningBallTask::RunningTask(const float& deltaTime)
{
	TaskNode::RunningTask(deltaTime);
	if (!m_Attack && m_CurrentTime >= m_MaxAnimTime * SPAWN_LIGHTNINGBALL_VALUE)
	{
		m_Attack = true;
	}
	else if (!m_Attack)
	{
		m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
		m_BossCache->InitGravity();
		if (m_LightningBallCache != nullptr)
		{
			const XMFLOAT3& bossPos = m_BossCache->GetPos();
			const XMFLOAT3& bossScale = m_BossCache->GetScale();
			const XMFLOAT3& bossFoward = m_BossCache->GetForward();
			// ‹¹‚ÌˆÊ’u‚É’²®
			const XMFLOAT3& spawnBulletPos = { bossPos.x + bossFoward.x * ((1.0f / bossScale.x) * 0.5f),bossPos.y + ((1.0f / bossScale.y) * 0.7f),bossPos.z + bossFoward.z * ((1.0f / bossScale.z) * 0.5f) };
			m_LightningBallCache->KeepPos(spawnBulletPos);
		}
	}

	if (m_Attack)
	{
		if (m_LightningBallCache != nullptr)
		{
			m_LightningBallCache->Attack();
		}
	}
}