#include "animationAttackTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

constexpr float MAX_TIME_ANIMEATTACK = 1.0f;

void AnimationAttackTask::Init()
{
	m_CurrentTime = MAX_TIME_ANIMEATTACK;
}

NODE_STATUS AnimationAttackTask::Update(const float& deltaTime)
{
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATUS::FAILURE;
	}

	// ”ÍˆÍ“à‚É“ü‚Á‚Ä‚¢‚½‚ç
	if (m_BossCache->GetCurrentRange() == RANGE::SHROT && m_CurrentTime >= MAX_TIME_ANIMEATTACK)
	{
		m_CurrentTime = 0.0f;
	}

	if (m_CurrentTime < MAX_TIME_ANIMEATTACK)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->AddRotation({ 0.0f,0.2f,0.0f });
		return NODE_STATUS::RUNNING;
	}
	else
	{
		return NODE_STATUS::SUCCESS;
	}

	return NODE_STATUS::FAILURE;
}
