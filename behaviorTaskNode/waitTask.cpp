#include "waitTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void WaitTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\idle_MawJ.fbx", "idle");
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

	// 初期設定
	if (node == nullptr)
	{
		// スタミナが多ければ多いほど、復帰時間が早くなる確率が上がる
		const int& randValue = rand() % static_cast<int>(m_BossCache->GetStamina());
		m_WaitValue = randValue * 0.008f;
		m_CurrentTime = 0.0f;
	}

	if (m_CurrentTime < m_BossCache->GetMaxWaitTime() * m_WaitValue)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimName);
		// スタミナ回復
		m_BossCache->UseStamina(-deltaTime * 0.7f);
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
			m_WaitValue = 1.0f;
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATE::SUCCESS;
		}
	}

	return NODE_STATE::FAILURE;
}