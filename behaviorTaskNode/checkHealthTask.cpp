#include "checkHealthTask.h"
#include "character/bossEnemy.h"

void CheckHealthTask::Init()
{
	// ƒˆ‰¼‘zŠÖ”‚Ìˆ×‹óŽÀ‘•
}

NODE_STATUS CheckHealthTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATUS::SUCCESS;
	}
	
	const int& enemyHealth = m_BossCache->GetHealth();
	if (enemyHealth <= 0)
	{
		return NODE_STATUS::SUCCESS;
	}

	return NODE_STATUS::FAILURE;
}
