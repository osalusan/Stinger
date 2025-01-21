#include "waitTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"
// TODO : 完成前に削除予定 / デバッグ用
#include "manager/inputManager.h"


void WaitTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\idle_MawJ.fbx", "idle_MawJ");
	m_TaskName = "待機";
}

NODE_STATE WaitTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATE::FAILURE;
	}

	BehaviorNode* node = m_BossCache->GetRunningNode();
	if (node != nullptr && node != this)
	{
		m_Wait = true;
		return NODE_STATE::FAILURE;
	}

	if (!m_Wait)
	{
		return NODE_STATE::FAILURE;
	}

	// TODO : 完成前に削除予定 / デバッグ用
	if (InputManager::GetKeyPress('V'))
	{
		m_PlayerCache->TakeDamage(1.0f);
	}

	if (m_CurrentTime < m_BossCache->GetMaxWaitTime())
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimeName);
		// 状態を保存
		m_BossCache->SetRunningNode(this);
		m_BossCache->RotToTarget(m_PlayerCache, deltaTime);

		return NODE_STATE::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// 状態を削除
			m_CurrentTime = 0.0f;
			m_Wait = false;
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATE::SUCCESS;
		}
	}

	return NODE_STATE::FAILURE;
}