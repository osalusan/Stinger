#include "swipingTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void SwipingTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\swiping_MawJ.fbx", "swiping_MawJ");
	InitSkillData(0);
	// TODO :削除予定 / ビヘイビアツリーの可視化テスト後削除 
	m_TaskName = u8"殴り";
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

	// 範囲内に入っていたら
	if (m_CurrentTime >= m_MaxAnimTime && m_BossCache->GetCurrentRange() == RANGE::SHROT)
	{
		if (node == nullptr)
		{
			m_CurrentTime = 0.0f;
			m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
		}
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimeName);
		// 攻撃状態を保存
		m_BossCache->SetRunningNode(this);

		return NODE_STATUS::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// 攻撃状態を削除
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATUS::SUCCESS;
		}
	}

	return NODE_STATUS::FAILURE;
}

