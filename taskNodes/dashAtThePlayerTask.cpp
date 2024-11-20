#include "dashAtThePlayerTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void DashAtThePlayerTask::Init()
{
	// ‰½‚àÝ’è‚³‚ê‚Ä‚¢‚È‚¢Žž
	if (m_Range == 0.0f)
	{
		m_Range = 10.0f;
	}
}

NODE_STATUS DashAtThePlayerTask::Update(const float& deltaTime)
{
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATUS::FAILURE;
	}

	// ”ÍˆÍ“à‚É“ü‚Á‚Ä‚¢‚½‚ç
	if (m_BossCache->GetCurrentRange() == RANGE::SHROT)
	{
		return NODE_STATUS::SUCCESS;
	}

	// ˆÚ“®ˆ—
	const XMFLOAT3& playerPos = m_PlayerCache->GetPos();
	const XMFLOAT3& direction = m_BossCache->GetTargetDirection(playerPos);

	const float& moveSpeed = m_BossCache->GetMoveSpeed() * deltaTime;

	m_BossCache->AddVelocity({ direction.x * moveSpeed ,0.0f,direction.z * moveSpeed });

	return NODE_STATUS::RUNNING;
}
