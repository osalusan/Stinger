#include "leftSwipingTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void LeftSwipingTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\leftSwiping_MawJ.fbx", "leftSwiping");
	m_TaskName = "������";
	InitSkillData(m_TaskName);
	m_ParryPossibleAtk = true;
}

NODE_STATE LeftSwipingTask::Update(const float& deltaTime)
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

	// �͈͓��ɓ����Ă�����
	if (m_CurrentTime >= m_MaxAnimTime && m_BossCache->GetCurrentRange() == RANGE::SHORT)
	{
		// ������
		if (node == nullptr)
		{
			m_UseDerivation = false;
			m_CurrentTime = 0.0f;
			m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
			// �h���Z�U�蕪��
			DerivationChance();
		}
	}

	// �h���Z�̔����m�F
	if (!m_UseDerivation && m_CurrentTime > m_MaxAnimTime * GetDerivationData(m_UseDerivNumber).TransTimeValue)
	{
		if (GetDerivationData().size() > 0)
		{
			if (m_BossCache->GetHealth() <= m_BossCache->GetMaxHealth() * GetDerivationData(m_UseDerivNumber).Health)
			{
				m_UseDerivation = true;
				m_CurrentTime = m_MaxAnimTime;
				m_BossCache->SetRunningNode(nullptr);
			}
		}
	}

	// �h���Z�̔����m�F��ɔz�u
	if (m_UseDerivation)
	{
		// ��̃^�X�N�̂ݍX�V
		return UpdateUseDerivationTask(deltaTime);
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimName);
		// �U����Ԃ�ۑ�
		m_BossCache->SetRunningNode(this);
		// �_���[�W����
		UseAttack(ATTACK_PARTS::LEFT_ARM);

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

