#include "rightSwipingTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void RightSwipingTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\rightSwiping_MawJ.fbx", "rightSwiping");
	m_TaskName = "右殴り";
	InitSkillData(m_TaskName);
	m_ParryPossibleAtk = true;
	m_AttackTask = true;
}

void RightSwipingTask::InitTask(const float& deltaTime)
{
	TaskNode::InitTask(deltaTime);
	const float& startAnimeTime = m_MaxAnimTime * 0.3f;
	m_CurrentTime = startAnimeTime;
	m_BossCache->SetNextAnimationTime(startAnimeTime);
}

void RightSwipingTask::RunningTask(const float& deltaTime)
{
	TaskNode::RunningTask(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return;
	}
	// ダメージ発生
	UseAttack(ATTACK_PARTS::RIGHT_ARM);

	if (m_CurrentTime < m_MaxAnimTime * m_AttackDisableTimeValue)
	{
		m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
	}
}