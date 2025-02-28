#include "deadTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void DeadTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\dying_MawJ.fbx", "dead");
	m_TaskName = "死亡";
	InitSkillData(m_TaskName);
}

NODE_STATE DeadTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATE::FAILURE;
	}

	if (m_BossCache->GetHealth() > 0.0f)
	{
		return NODE_STATE::FAILURE;
	}

	// 死んだときに強制的に入る初期化
	BehaviorNode* node = m_BossCache->GetRunningNode();
	if (node != this)
	{
		m_CurrentTime = 0.0f;
		m_BossCache->SetAnimationSpeedValue(m_AnimSpeedValue);
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime * m_AnimSpeedValue;
		m_BossCache->ChangeAnimation(m_AnimName);
		// 状態を保存
		m_BossCache->SetRunningNode(this);

		return NODE_STATE::RUNNING;
	}

	// アニメーション仕切ったら動きを止める
	m_BossCache->SetAnimeTime(m_MaxAnimTime - deltaTime);
	return NODE_STATE::SUCCESS;
}