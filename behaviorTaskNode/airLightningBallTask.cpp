#include "airLightningBallTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"
#include "scene/scene.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "staticMeshObject/lightningBall.h"

constexpr float SPAWN_LIGHTNINGBALL_VALUE = 0.37f;

void AirLightningBallTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\standing2HMagicAttack_MawJ.fbx", "airLightningBall");
	m_TaskName = "óããÖî≠éÀ";
	InitSkillData(m_TaskName);

	Scene* scene = SceneManager::GetScene();
	if (scene == nullptr) return;
	ObjectManager* objManager = scene->GetObjectManager();
	if (objManager == nullptr) return;

	// ÉvÉåÉCÉÑÅ[ÇÃëïîı
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
	if (node != nullptr && node != this)
	{
		return NODE_STATE::FAILURE;
	}

	// èâä˙âª
	if (node == nullptr)
	{
		m_SpawnBall = false;
		m_CurrentTime = 0.0f;
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimeName);
		// èÛë‘Çï€ë∂
		m_BossCache->SetRunningNode(this);

		if (!m_SpawnBall && m_CurrentTime >= m_MaxAnimTime * SPAWN_LIGHTNINGBALL_VALUE)
		{
			m_SpawnBall = true;
			m_LightningBallCache->Attack(m_BossCache->GetPos(), m_BossCache->GetAttack() * m_DamageValue);
		}
		else if (!m_SpawnBall)
		{
			m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
			m_BossCache->InitGravity();
		}

		return NODE_STATE::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// èÛë‘ÇçÌèú
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATE::SUCCESS;
		}
	}

	return NODE_STATE::FAILURE;
}