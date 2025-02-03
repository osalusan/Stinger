#include "jumpAttackTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

// ���n�ɍ��킹��p
constexpr float LANDING_MIN_VALUE = 0.2f;
constexpr float LANDING_MAX_VALUE = 0.48f;
constexpr float JUMP_MAX_VALUE = 0.36f;
constexpr float INIT_ACCEL = 60.0f;														// �����x
constexpr float JUMP_RESIST = INIT_ACCEL / (JUMP_MAX_VALUE - LANDING_MIN_VALUE);		// ��R��
void JumpAttackTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\jumpAttack_IsPlace_MawJ.fbx", "jumpAttack");
	m_TaskName = "�W�����v�U��";
	InitSkillData(m_TaskName);
	m_ParryPossibleAtk = true;
	m_UseDerivation = true;
}

NODE_STATE JumpAttackTask::Update(const float& deltaTime)
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
		const RANGE& currentRange = m_BossCache->GetCurrentRange();
		// �͈͓��ɓ����Ă�����
		if (m_CurrentTime >= m_MaxAnimTime && currentRange == RANGE::MIDDLE)
		{
			const float& maxStamina = m_BossCache->GetaMaxStamina();
			if (m_BossCache->UseStamina(maxStamina * m_UseStaminaValue))
			{
				m_CurrentTime = 0.0f;
				m_Accel = INIT_ACCEL;
				m_UseDerivation = false;
				m_EnableDerivation = false;

				// �m��
				if (rand() % 100 < GetDerivationData(0).Chance)
				{
					m_EnableDerivation = true;
				}
			}
		}
		
		// �����ɓ��Ă͂܂�Ȃ�������
		if(m_CurrentTime != 0.0f)
		{
			return NODE_STATE::FAILURE;
		}
	}

	// �h���Z�̔����m�F
	if (m_EnableDerivation && m_CurrentTime > m_MaxAnimTime * m_DerivationTimeValue)
	{
		if (m_Children.size() != 0 && m_BossCache->GetHealth() <= m_BossCache->GetMaxHealth() * GetDerivationData(0).Health)
		{
			m_UseDerivation = true;
			m_EnableDerivation = false;
			m_CurrentTime = m_MaxAnimTime;
			m_BossCache->SetRunningNode(nullptr);
		}
	}

	// �h���Z�̔����m�F��ɔz�u
	if (m_UseDerivation)
	{
		return UpdateChildren(deltaTime);
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimeName);
		// ��Ԃ�ۑ�
		m_BossCache->SetRunningNode(this);


		// �ړ� / �W�����v
		if (m_CurrentTime >= m_MaxAnimTime * LANDING_MIN_VALUE && m_CurrentTime < m_MaxAnimTime * LANDING_MAX_VALUE)
		{
			m_Accel -= (JUMP_RESIST / m_MaxAnimTime)* deltaTime;
			m_BossCache->AddVelocity({ 0.0f ,m_Accel ,0.0f });
			m_BossCache->AddVelocity(m_MoveVector);
			m_BossCache->InitGravity();
		}
		// �U���ꏊ�w��
		else if (m_CurrentTime < m_MaxAnimTime * LANDING_MIN_VALUE)
		{
			const XMFLOAT3& playerPos = m_PlayerCache->GetPos();
			const XMFLOAT3& bossPos = m_BossCache->GetPos();
			m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
			// �ړ������ݒ�
			m_MoveVector.x = (playerPos.x - bossPos.x) / (m_MaxAnimTime * (LANDING_MAX_VALUE - LANDING_MIN_VALUE));
			m_MoveVector.z = (playerPos.z - bossPos.z) / (m_MaxAnimTime * (LANDING_MAX_VALUE - LANDING_MIN_VALUE));
		}
		// �_���[�W����
		UseAttack(ATTACK_PARTS::ALL);

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
