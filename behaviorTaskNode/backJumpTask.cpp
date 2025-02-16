#include "backJumpTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"

// アニメーションに合わせる
constexpr float MOVE_MIN_TIME_VALUE = 0.4f;
constexpr float MOVE_MAX_TIME_VALUE = 0.75f;

void BackJumpTask::RunningTask(const float& deltaTime)
{
	TaskNode::RunningTask(deltaTime);
	if (m_CurrentTime > m_MaxAnimTime * MOVE_MIN_TIME_VALUE && m_CurrentTime < m_MaxAnimTime * MOVE_MAX_TIME_VALUE)
	{
		const XMFLOAT3& playerPos = m_PlayerCache->GetPos();
		const XMFLOAT3& direction = m_BossCache->GetTargetDirection(playerPos);

		// 後ろに
		const float& moveSpeed = -m_BossCache->GetMoveSpeed();

		m_BossCache->AddVelocity({ direction.x * moveSpeed ,0.0f,direction.z * moveSpeed });
	}
	else
	{
		m_BossCache->RotToTarget(m_PlayerCache, deltaTime);
	}
}

void BackJumpTask::Init()
{
	ReserveAnimation("asset\\model\\mawJ\\jumping_MawJ.fbx", "jump");
	m_TaskName = "バックジャンプ";
	m_AttackTask = true;
}