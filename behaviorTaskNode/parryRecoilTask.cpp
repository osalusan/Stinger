#include "parryRecoilTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void ParryRecoilTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\standingReact_MawJ.fbx", "parryRecoil_MawJ");
	m_TaskName = "ƒpƒŠƒB‚Å‚Ì‚æ‚ë‚¯";
}

NODE_STATE ParryRecoilTask::Update(const float& deltaTime)
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
	if (node == nullptr)
	{
		m_CurrentTime = 0.0f;
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimeName);
		// ó‘Ô‚ð•Û‘¶
		m_BossCache->SetRunningNode(this);

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
