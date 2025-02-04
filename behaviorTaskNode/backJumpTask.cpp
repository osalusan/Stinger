#include "backJumpTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

// アニメーションに合わせる
constexpr float MOVE_MIN_TIME_VALUE = 0.4f;
constexpr float MOVE_MAX_TIME_VALUE = 0.75f;

void BackJumpTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\jumping_MawJ.fbx", "jump");
	m_TaskName = "バックジャンプ";
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

	// 初期化
	if (node == nullptr)
	{
		m_CurrentTime = 0.0f;
		m_UseDerivation = false;
		m_EnableDerivation = false;

		// 派生技振り分け
		DerivationChance();

		// 確率
		if (rand() % 100 < GetDerivationData(m_UseDerivNumber).Chance)
		{
			m_EnableDerivation = true;
		}
	}

	// 派生技の発生確認
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

	// 派生技の発生確認後に配置
	if (m_UseDerivation)
	{
		return UpdateUseDerivationTask(deltaTime);
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimName);
		// 状態を保存
		m_BossCache->SetRunningNode(this);

		if (m_CurrentTime > m_MaxAnimTime * MOVE_MIN_TIME_VALUE && m_CurrentTime < m_MaxAnimTime * MOVE_MAX_TIME_VALUE)
		{
			const XMFLOAT3& playerPos = m_PlayerCache->GetPos();
			const XMFLOAT3& direction = m_BossCache->GetTargetDirection(playerPos);

			// 後ろに
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
			// 状態を削除
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATE::SUCCESS;
		}
	}

	return NODE_STATE::FAILURE;
}