#include "loadIdleTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void LoadIdleTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\loadIdle_MawJ.fbx", "loadIdle_MawJ");
}

NODE_STATUS LoadIdleTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr)
	{
		return NODE_STATUS::FAILURE;
	}

	m_BossCache->ChangeAnimation(m_AnimeName);

	return NODE_STATUS::SUCCESS;
}