#include "shortRangeAttackTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void ShortRangeAttackTask::Init()
{
	m_TaskName = "近距離攻撃管理タスク";
}

NODE_STATE ShortRangeAttackTask::Update(const float& deltaTime)
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
		if (m_BossCache->GetCurrentRange() != RANGE::SHORT)
		{
			return NODE_STATE::FAILURE;
		}

		if (DerivationChance() == -1)
		{
			return NODE_STATE::FAILURE;
		}
	}

	// 一つのタスクのみ更新
	return UpdateUseDerivationTask(deltaTime);
}