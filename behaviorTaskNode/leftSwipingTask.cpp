#include "leftSwipingTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void LeftSwipingTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\leftSwiping_MawJ.fbx", "leftSwiping");
	m_TaskName = "ç∂â£ÇË";
	InitSkillData(m_TaskName);
	m_ParryPossibleAtk = true;
}

NODE_STATE LeftSwipingTask::Update(const float& deltaTime)
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

	// îÕàÕì‡Ç…ì¸Ç¡ÇƒÇ¢ÇΩÇÁ
	if (m_CurrentTime >= m_MaxAnimTime && m_BossCache->GetCurrentRange() == RANGE::SHORT)
	{
		// èâä˙âª
		if (node == nullptr)
		{
			m_UseDerivation = false;
			m_CurrentTime = 0.0f;
			m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
		}
	}

	// îhê∂ãZÇÃî≠ê∂ämîF
	if (!m_UseDerivation && m_CurrentTime > m_MaxAnimTime * m_DerivationTimeValue)
	{
		if (m_Children.size() != 0 && m_BossCache->GetHealth() <= m_BossCache->GetMaxHealth() * m_DerivationHealth)
		{
			m_UseDerivation = true;
			m_CurrentTime = m_MaxAnimTime;
			m_BossCache->SetRunningNode(nullptr);
		}
	}

	// îhê∂ãZÇÃî≠ê∂ämîFå„Ç…îzíu
	if (m_UseDerivation)
	{
		return UpdateChildren(deltaTime);
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimeName);
		// çUåÇèÛë‘Çï€ë∂
		m_BossCache->SetRunningNode(this);
		// É_ÉÅÅ[ÉWî≠ê∂
		UseAttack(ATTACK_PARTS::LEFT_ARM);

		return NODE_STATE::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// çUåÇèÛë‘ÇçÌèú
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATE::SUCCESS;
		}
	}

	return NODE_STATE::FAILURE;
}

