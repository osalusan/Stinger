#include "waitTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void WaitTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\idle_MawJ.fbx", "idle_MawJ");
	// TODO :削除予定 / ビヘイビアツリーの可視化テスト後削除 
	m_TaskName = u8"待機";
}

NODE_STATUS WaitTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATUS::FAILURE;
	}

	BehaviorNode* node = m_BossCache->GetRunningNode();
	if (node != nullptr && node != this)
	{
		m_Wait = true;
		return NODE_STATUS::FAILURE;
	}

	if (!m_Wait)
	{
		return NODE_STATUS::FAILURE;
	}

	if (m_CurrentTime < m_BossCache->GetMaxWaitTime())
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimeName);
		// 状態を保存
		m_BossCache->SetRunningNode(this);
		m_BossCache->RotToTarget(m_PlayerCache, deltaTime);

		return NODE_STATUS::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// 状態を削除
			m_CurrentTime = 0.0f;
			m_Wait = false;
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATUS::SUCCESS;
		}
	}

	return NODE_STATUS::FAILURE;
}