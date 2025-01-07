#include "roaringTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void RoaringTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\mutantRoaring_MawJ.fbx", "roaring_MawJ");
	// TODO :�폜�\�� / �r�w�C�r�A�c���[�̉����e�X�g��폜 
	m_TaskName = u8"�Њd";
}

NODE_STATUS RoaringTask::Update(const float& deltaTime)
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
	if (m_CurrentTime >= m_MaxAnimTime && m_BossCache->GetCurrentRange() == RANGE::MIDDLE)
	{
		if(node == nullptr)
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
