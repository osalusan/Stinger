#include "checkRangeTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void CheckRangeTask::Init()
{
}

NODE_STATUS CheckRangeTask::Update(const float& deltaTime)
{
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATUS::FAILURE;
	}

	const XMFLOAT3& playerPos = m_PlayerCache->GetPos();
	const XMFLOAT3& bossPos = m_BossCache->GetPos();

	const float& vectorX = abs(playerPos.x - bossPos.x);
	const float& vectorZ = abs(playerPos.z - bossPos.z);
	const float& range = vectorX + vectorZ;

	// ”ÍˆÍ“à‚É“ü‚Á‚Ä‚¢‚½‚ç
	if (range <= m_BossCache->GetShortRange())
	{
		m_BossCache->SetCurrentRange(RANGE::SHROT);
	}
	else if (range <= m_BossCache->GetMiddelRange())
	{
		m_BossCache->SetCurrentRange(RANGE::MIDDLE);
	}
	else if (range <= m_BossCache->GetLongRange())
	{
		m_BossCache->SetCurrentRange(RANGE::LONG);
	}
	else
	{
		m_BossCache->SetCurrentRange(RANGE::NONE);
	}

	return NODE_STATUS::SUCCESS;
}
