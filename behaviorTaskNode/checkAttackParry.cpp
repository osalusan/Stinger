#include "checkAttackParry.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void CheckAttackParryTask::Init()
{
	m_TaskName = "パリィされたか確認";
}

NODE_STATE CheckAttackParryTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr )
	{
		return NODE_STATE::SUCCESS;
	}

	if (m_BossCache->GetParryRecoil())
	{
		if (!m_RunningChild)
		{
			// アニメーションを中断
			if (const TaskNode* taskNode = dynamic_cast<TaskNode*>(m_BossCache->GetRunningNode()))
			{
				if (taskNode == nullptr) return NODE_STATE::FAILURE;
				m_BossCache->SetAnimeTime(taskNode->GetMaxAnimTime());
			}
			
			// 状態を保存
			m_BossCache->SetRunningNode(nullptr);
		}


		const NODE_STATE& childResult = UpdateChildren(deltaTime);
		if (childResult == NODE_STATE::RUNNING)
		{
			m_RunningChild = true;
		}
		if (childResult == NODE_STATE::SUCCESS)
		{
			m_RunningChild = false;
			m_BossCache->SetParryRecoil(false);
		}

		return childResult;
	}

	return NODE_STATE::FAILURE;
}