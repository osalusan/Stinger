#include "swipingTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void SwipingTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\swiping_MawJ.fbx", "swiping_MawJ");
	m_TaskName = "‰£‚è";
	InitSkillData(m_TaskName);
}

NODE_STATE SwipingTask::Update(const float& deltaTime)
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

	// ”ÍˆÍ“à‚É“ü‚Á‚Ä‚¢‚½‚ç
	if (m_CurrentTime >= m_MaxAnimTime && m_BossCache->GetCurrentRange() == RANGE::SHROT)
	{
		if (node == nullptr)
		{
			m_CurrentTime = 0.0f;
			m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
		}
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimeName);
		// UŒ‚ó‘Ô‚ð•Û‘¶
		m_BossCache->SetRunningNode(this);

		return NODE_STATE::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// UŒ‚ó‘Ô‚ðíœ
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATE::SUCCESS;
		}
	}

	return NODE_STATE::FAILURE;
}

