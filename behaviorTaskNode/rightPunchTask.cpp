#include "rightPunchTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void RightPunchTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\mutantPunch_MawJ.fbx", "rightPunch");
	m_TaskName = "âEÉpÉìÉ`";
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

	// èâä˙âª
	if (node == nullptr)
	{
		m_CurrentTime = 0.0f;
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimeName);
		// èÛë‘Çï€ë∂
		m_BossCache->SetRunningNode(this);

		return NODE_STATE::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// èÛë‘ÇçÌèú
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATE::SUCCESS;
		}
	}

	return NODE_STATE::FAILURE;
}