#include "leftSwipingTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void LeftSwipingTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\leftSwiping_MawJ.fbx", "leftSwiping");
	m_TaskName = "左殴り";
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

	// 範囲内に入っていたら
	if (m_CurrentTime >= m_MaxAnimTime && m_BossCache->GetCurrentRange() == RANGE::SHORT)
	{
		// 初期化
		if (node == nullptr)
		{
			m_UseDerivation = false;
			m_CurrentTime = 0.0f;
			m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
			// 派生技振り分け
			DerivationChance();
		}
	}

	// 派生技の発生確認
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

	// 派生技の発生確認後に配置
	if (m_UseDerivation)
	{
		// 一つのタスクのみ更新
		return UpdateUseDerivationTask(deltaTime);
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimName);
		// 攻撃状態を保存
		m_BossCache->SetRunningNode(this);
		// ダメージ発生
		UseAttack(ATTACK_PARTS::LEFT_ARM);

		return NODE_STATE::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// 攻撃状態を削除
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATE::SUCCESS;
		}
	}

	return NODE_STATE::FAILURE;
}

