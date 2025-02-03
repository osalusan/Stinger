#include "jumpAttackTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

// 着地に合わせる用
constexpr float LANDING_MIN_VALUE = 0.2f;
constexpr float LANDING_MAX_VALUE = 0.48f;
constexpr float JUMP_MAX_VALUE = 0.36f;
constexpr float INIT_ACCEL = 60.0f;														// 初速度
constexpr float JUMP_RESIST = INIT_ACCEL / (JUMP_MAX_VALUE - LANDING_MIN_VALUE);		// 抵抗力
void JumpAttackTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\jumpAttack_IsPlace_MawJ.fbx", "jumpAttack");
	m_TaskName = "ジャンプ攻撃";
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

	// 初期化
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
				m_Accel = INIT_ACCEL;
				m_UseDerivation = false;
				m_EnableDerivation = false;

				// 確率
				if (rand() % 100 < GetDerivationData(0).Chance)
				{
					m_EnableDerivation = true;
				}
			}
		}
		
		// 条件に当てはまらなかったら
		if(m_CurrentTime != 0.0f)
		{
			return NODE_STATE::FAILURE;
		}
	}

	// 派生技の発生確認
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

	// 派生技の発生確認後に配置
	if (m_UseDerivation)
	{
		return UpdateChildren(deltaTime);
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimeName);
		// 状態を保存
		m_BossCache->SetRunningNode(this);


		// 移動 / ジャンプ
		if (m_CurrentTime >= m_MaxAnimTime * LANDING_MIN_VALUE && m_CurrentTime < m_MaxAnimTime * LANDING_MAX_VALUE)
		{
			m_Accel -= (JUMP_RESIST / m_MaxAnimTime)* deltaTime;
			m_BossCache->AddVelocity({ 0.0f ,m_Accel ,0.0f });
			m_BossCache->AddVelocity(m_MoveVector);
			m_BossCache->InitGravity();
		}
		// 攻撃場所指定
		else if (m_CurrentTime < m_MaxAnimTime * LANDING_MIN_VALUE)
		{
			const XMFLOAT3& playerPos = m_PlayerCache->GetPos();
			const XMFLOAT3& bossPos = m_BossCache->GetPos();
			m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
			// 移動距離設定
			m_MoveVector.x = (playerPos.x - bossPos.x) / (m_MaxAnimTime * (LANDING_MAX_VALUE - LANDING_MIN_VALUE));
			m_MoveVector.z = (playerPos.z - bossPos.z) / (m_MaxAnimTime * (LANDING_MAX_VALUE - LANDING_MIN_VALUE));
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
