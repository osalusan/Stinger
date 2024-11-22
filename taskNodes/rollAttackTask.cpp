#include "rollAttackTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

constexpr float MAX_TIME_ROLLATTACK = 1.0f;

void RollAttackTask::Init()
{
	m_CurrentTime = MAX_TIME_ROLLATTACK;
}

NODE_STATUS RollAttackTask::Update(const float& deltaTime)
{
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATUS::FAILURE;
	}

	BehaviorNode* node = m_BossCache->GetRunningNode();
	if (node != nullptr && node != this)
	{
		return NODE_STATUS::FAILURE;
	}
	// ”ÍˆÍ“à‚É“ü‚Á‚Ä‚¢‚½‚ç
	if (m_CurrentTime >= MAX_TIME_ROLLATTACK && m_BossCache->GetCurrentRange() == RANGE::SHROT)
	{
		if (node == nullptr)
		{
			m_CurrentTime = 0.0f;
		}
	}

	if (m_CurrentTime < MAX_TIME_ROLLATTACK)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->AddRotation({ 0.0f,0.2f,0.0f });
		// UŒ‚ó‘Ô‚ð•Û‘¶
		m_BossCache->SetRunningNode(this);
		return NODE_STATUS::RUNNING;
	}
	else
	{	
		if (node == this)
		{
			// UŒ‚ó‘Ô‚ðíœ
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATUS::SUCCESS;
		}
	}

	return NODE_STATUS::FAILURE;
}
