#include "checkRangeTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

void CheckRangeTask::Init()
{
	m_TaskName = u8"範囲確認";
}

NODE_STATUS CheckRangeTask::Update(const float& deltaTime)
{
	TaskNode::Update(deltaTime);
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATUS::FAILURE;
	}
	// 攻撃実行中の為、状態を設定しない
	if (m_BossCache->GetRunningNode() != nullptr)
	{
		return NODE_STATUS::FAILURE;
	}

	const XMFLOAT3& playerPos = m_PlayerCache->GetPos();
	const XMFLOAT3& bossPos = m_BossCache->GetPos();

	const float& vectorX = abs(playerPos.x - bossPos.x);
	const float& vectorZ = abs(playerPos.z - bossPos.z);
	const float& range = vectorX + vectorZ;

	// 範囲内に入っていたら
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

	return NODE_STATUS::FAILURE;
}
