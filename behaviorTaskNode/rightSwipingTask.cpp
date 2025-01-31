#include "rightSwipingTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void RightSwipingTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\rightSwiping_MawJ.fbx", "rightSwiping");
	m_TaskName = "�E����";
	InitSkillData(m_TaskName);
	m_ParryPossibleAtk = true;
}

NODE_STATE RightSwipingTask::Update(const float& deltaTime)
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

	// �͈͓��ɓ����Ă�����
	if (m_CurrentTime >= m_MaxAnimTime && m_BossCache->GetCurrentRange() == RANGE::SHROT)
	{
		if (node == nullptr)
		{
			const float& startAnimeTime = m_MaxAnimTime * 0.3f;
			m_CurrentTime = startAnimeTime;
			m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
			m_BossCache->SetNextAnimationTime(startAnimeTime);
		}
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimeName);
		// �U����Ԃ�ۑ�
		m_BossCache->SetRunningNode(this);
		// �_���[�W����
		UseAttack(ATTACK_PARTS::RIGHT_ARM);

		return NODE_STATE::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// �U����Ԃ��폜
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATE::SUCCESS;
		}
	}

	return NODE_STATE::FAILURE;
}

