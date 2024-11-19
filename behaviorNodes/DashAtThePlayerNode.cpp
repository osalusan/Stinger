#include "DashAtThePlayerNode.h"
#include "manager/objectManager.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void DashAtThePlayerNode::Init()
{
	// ‰½‚àÝ’è‚³‚ê‚Ä‚¢‚È‚¢Žž
	if (m_Range == 0.0f)
	{
		m_Range = 10.0f;
	}
}

NODE_STATUS DashAtThePlayerNode::Update(const float& deltaTime)
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
	if (range <= m_Range)
	{
		return NODE_STATUS::SUCCESS;
	}

	// ˆÚ“®ˆ—
	const XMFLOAT3& direction = m_BossCache->GetTargetDirection(playerPos);

	const float& moveSpeed = m_BossCache->GetMoveSpeed() * deltaTime;

	m_BossCache->AddVelocity({ direction.x * moveSpeed ,0.0f,direction.z * moveSpeed });

	return NODE_STATUS::RUNNING;
}
