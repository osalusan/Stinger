#include "rightPunchTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void RightPunchTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\mutantPunch_MawJ.fbx", "rightPunch");
	m_TaskName = "‰Eƒpƒ“ƒ`";
	InitSkillData(m_TaskName);
	m_ParryPossibleAtk = true;
}

NODE_STATE RightPunchTask::Update(const float& deltaTime)
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

	// ‰Šú‰»
	if (node == nullptr)
	{
		if (m_CurrentTime >= m_MaxAnimTime && m_BossCache->GetCurrentRange() == RANGE::SHORT)
		{
			m_CurrentTime = 0.0f;
		}
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimeName);
		// ó‘Ô‚ð•Û‘¶
		m_BossCache->SetRunningNode(this);

		// UŒ‚”»’è
		UseAttack(ATTACK_PARTS::RIGHT_ARM);

		return NODE_STATE::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// ó‘Ô‚ðíœ
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATE::SUCCESS;
		}
	}

	return NODE_STATE::FAILURE;
}