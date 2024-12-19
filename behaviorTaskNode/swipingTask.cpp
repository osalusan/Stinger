#include "swipingTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void SwipingTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\Swiping_MawJ.fbx", "Swiping_MawJ");
}

NODE_STATUS SwipingTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATUS::FAILURE;
	}

	BehaviorNode* node = m_BossCache->GetRunningNode();
	if (node != nullptr && node != this)
	{
		return NODE_STATUS::FAILURE;
	}

	// �͈͓��ɓ����Ă�����
	if (m_CurrentTime >= m_MaxAnimTime && m_BossCache->GetCurrentRange() == RANGE::SHROT)
	{
		if (node == nullptr)
		{
			m_CurrentTime = 0.0f;
		}
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimeName);
		// �U����Ԃ�ۑ�
		m_BossCache->SetRunningNode(this);

		return NODE_STATUS::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// �U����Ԃ��폜
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATUS::SUCCESS;
		}
	}

	return NODE_STATUS::FAILURE;
}

