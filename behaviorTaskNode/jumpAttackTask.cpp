#include "jumpAttackTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

// íÖínÇ…çáÇÌÇπÇÈóp
constexpr float LANDING_MIN_VALUE = 0.1f;
constexpr float LANDING_MAX_VALUE = 0.48f;
void JumpAttackTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\jumpAttack_MawJ.fbx", "jumpAttack_MawJ");
	InitSkillData(1);
}

NODE_STATUS JumpAttackTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATUS::FAILURE;
	}

	BehaviorNode* node = m_BossCache->GetRunningNode();
	if (node != nullptr && node != this)
	{
		return NODE_STATUS::FAILURE;
	}

	const RANGE& currentRange = m_BossCache->GetCurrentRange();
	// îÕàÕì‡Ç…ì¸Ç¡ÇƒÇ¢ÇΩÇÁ
	if (m_CurrentTime >= m_MaxAnimTime && currentRange == RANGE::MIDDLE)
	{
		const float& maxStamina = m_BossCache->GetaMaxStamina();
		if (m_BossCache->UseStamina(maxStamina * m_UseStaminaValue))
		{
			if (node == nullptr)
			{
				m_CurrentTime = 0.0f;
				const XMFLOAT3& playerPos = m_PlayerCache->GetPos();
				const XMFLOAT3& bossPos = m_BossCache->GetPos();
				m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
				// à⁄ìÆãóó£ê›íË
				m_MoveVector.x = (playerPos.x - bossPos.x) / (m_MaxAnimTime * (LANDING_MAX_VALUE - LANDING_MIN_VALUE));
				m_MoveVector.z = (playerPos.z - bossPos.z) / (m_MaxAnimTime * (LANDING_MAX_VALUE - LANDING_MIN_VALUE));
			}
		}
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimeName);
		// èÛë‘Çï€ë∂
		m_BossCache->SetRunningNode(this);

		if (m_CurrentTime >= m_MaxAnimTime * LANDING_MIN_VALUE && m_CurrentTime < m_MaxAnimTime * LANDING_MAX_VALUE)
		{
			m_BossCache->AddVelocity(m_MoveVector);
		}

		return NODE_STATUS::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// èÛë‘ÇçÌèú
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATUS::SUCCESS;
		}
	}

	return NODE_STATUS::FAILURE;
}
