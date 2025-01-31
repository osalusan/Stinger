#include "jumpAttackTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

// 着地に合わせる用
constexpr float LANDING_MIN_VALUE = 0.1f;
constexpr float LANDING_MAX_VALUE = 0.48f;
void JumpAttackTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\jumpAttack_MawJ.fbx", "jumpAttack");
	m_TaskName = "ジャンプ攻撃";
	InitSkillData(m_TaskName);
	m_ParryPossibleAtk = true;
}

NODE_STATE JumpAttackTask::Update(const float& deltaTime)
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

	if (node == nullptr)
	{
		const RANGE& currentRange = m_BossCache->GetCurrentRange();
		// 範囲内に入っていたら
		if (m_CurrentTime >= m_MaxAnimTime && currentRange == RANGE::MIDDLE)
		{
			const float& maxStamina = m_BossCache->GetaMaxStamina();
			if (m_BossCache->UseStamina(maxStamina * m_UseStaminaValue))
			{
				m_CurrentTime = 0.0f;
				const XMFLOAT3& playerPos = m_PlayerCache->GetPos();
				const XMFLOAT3& bossPos = m_BossCache->GetPos();
				m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
				// 移動距離設定
				m_MoveVector.x = (playerPos.x - bossPos.x) / (m_MaxAnimTime * (LANDING_MAX_VALUE - LANDING_MIN_VALUE));
				m_MoveVector.z = (playerPos.z - bossPos.z) / (m_MaxAnimTime * (LANDING_MAX_VALUE - LANDING_MIN_VALUE));
			}
		}
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimeName);
		// 状態を保存
		m_BossCache->SetRunningNode(this);

		if (m_CurrentTime >= m_MaxAnimTime * LANDING_MIN_VALUE && m_CurrentTime < m_MaxAnimTime * LANDING_MAX_VALUE)
		{
			m_BossCache->AddVelocity(m_MoveVector);
		}
		// ダメージ発生
		UseAttack(ATTACK_PARTS::ALL);

		return NODE_STATE::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// 状態を削除
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATE::SUCCESS;
		}
	}

	return NODE_STATE::FAILURE;
}
