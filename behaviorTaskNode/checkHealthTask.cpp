#include "checkHealthTask.h"
#include "character/bossEnemy.h"

void CheckHealthTask::Init()
{
	// �������z�֐��̈׋����
}

NODE_STATUS CheckHealthTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATUS::FAILURE;
	}
	

	return NODE_STATUS::FAILURE;
}
