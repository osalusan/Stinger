#include "waitTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void WaitTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\idle_MawJ.fbx", "idle_MawJ");
	m_TaskName = "‘Ò‹@";
}

NODE_STATE WaitTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATE::FAILURE;
	}

	BehaviorNode* node = m_BossCache->GetRunningNode();
	if (node != nullptr && node != this)
	{
		m_Wait = true;
		return NODE_STATE::FAILURE;
	}

	if (!m_Wait)
	{
		return NODE_STATE::FAILURE;
	}

	if (m_CurrentTime < m_BossCache->GetMaxWaitTime())
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimeName);
		// ó‘Ô‚ð•Û‘¶
		m_BossCache->SetRunningNode(this);
		m_BossCache->RotToTarget(m_PlayerCache, deltaTime);

		return NODE_STATE::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// ó‘Ô‚ðíœ
			m_CurrentTime = 0.0f;
			m_Wait = false;
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATE::SUCCESS;
		}
	}

	return NODE_STATE::FAILURE;
}