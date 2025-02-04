#include "backJumpTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

// �A�j���[�V�����ɍ��킹��
constexpr float MOVE_MIN_TIME_VALUE = 0.4f;
constexpr float MOVE_MAX_TIME_VALUE = 0.75f;

void BackJumpTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\jumping_MawJ.fbx", "jump");
	m_TaskName = "�o�b�N�W�����v";
}

NODE_STATE BackJumpTask::Update(const float& deltaTime)
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

	// ������
	if (node == nullptr)
	{
		m_CurrentTime = 0.0f;
		m_UseDerivation = false;
		m_EnableDerivation = false;

		// �h���Z�U�蕪��
		DerivationChance();

		// �m��
		if (rand() % 100 < GetDerivationData(m_UseDerivNumber).Chance)
		{
			m_EnableDerivation = true;
		}
	}

	// �h���Z�̔����m�F
	if (GetDerivationData().size() > 0)
	{
		if (m_EnableDerivation && m_CurrentTime > m_MaxAnimTime * GetDerivationData(m_UseDerivNumber).TransTimeValue)
		{
			if (m_BossCache->GetHealth() <= m_BossCache->GetMaxHealth() * GetDerivationData(m_UseDerivNumber).Health)
			{
				m_UseDerivation = true;
				m_EnableDerivation = false;
				m_CurrentTime = m_MaxAnimTime;
				m_BossCache->SetRunningNode(nullptr);
			}
		}
	}

	// �h���Z�̔����m�F��ɔz�u
	if (m_UseDerivation)
	{
		return UpdateUseDerivationTask(deltaTime);
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimName);
		// ��Ԃ�ۑ�
		m_BossCache->SetRunningNode(this);

		if (m_CurrentTime > m_MaxAnimTime * MOVE_MIN_TIME_VALUE && m_CurrentTime < m_MaxAnimTime * MOVE_MAX_TIME_VALUE)
		{
			const XMFLOAT3& playerPos = m_PlayerCache->GetPos();
			const XMFLOAT3& direction = m_BossCache->GetTargetDirection(playerPos);

			// ����
			const float& moveSpeed = -m_BossCache->GetMoveSpeed();

			m_BossCache->AddVelocity({ direction.x * moveSpeed ,0.0f,direction.z * moveSpeed });
		}
		else
		{
			m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
		}

		return NODE_STATE::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// ��Ԃ��폜
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATE::SUCCESS;
		}
	}

	return NODE_STATE::FAILURE;
}