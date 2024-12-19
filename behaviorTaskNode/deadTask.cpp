#include "deadTask.h"

#include "character/player.h"
#include "character/bossEnemy.h"

void DeadTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\Dying_MawJ.fbx", "Dead_MawJ");
}

NODE_STATUS DeadTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATUS::FAILURE;
	}

	BehaviorNode* node = m_BossCache->GetRunningNode();
	if (node != nullptr && node != this)
	{
		m_CurrentTime = 0.0f;
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimeName);
		// 状態を保存
		m_BossCache->SetRunningNode(this);

		return NODE_STATUS::RUNNING;
	}

	// アニメーション仕切ったら動きを止める
	m_BossCache->SetAnimeTime(m_MaxAnimTime - deltaTime);
	return NODE_STATUS::SUCCESS;
}