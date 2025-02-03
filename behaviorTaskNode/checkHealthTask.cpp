#include "checkHealthTask.h"
#include "character/bossEnemy.h"

void CheckHealthTask::Init()
{
	m_TaskName = "‘Ì—ÍŠm”F";
}

NODE_STATE CheckHealthTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATE::SUCCESS;
	}
	
	const float& enemyHealth = m_BossCache->GetHealth();
	if (enemyHealth <= 0)
	{
		return NODE_STATE::SUCCESS;
	}

	return NODE_STATE::FAILURE;
}
