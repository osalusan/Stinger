#include "rightPunchTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void RightPunchTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\mutantPunch_MawJ.fbx", "rightPunch");
	m_TaskName = "‰E‚‘¬ƒpƒ“ƒ`";
	InitSkillData(m_TaskName);
	m_ParryPossibleAtk = true;
	m_AttackTask = true;
}

void RightPunchTask::RunningTask(const float& deltaTime)
{
	TaskNode::RunningTask(deltaTime);

	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return;
	}

	// UŒ‚”»’è
	UseAttack(ATTACK_PARTS::RIGHT_ARM);
	m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
}