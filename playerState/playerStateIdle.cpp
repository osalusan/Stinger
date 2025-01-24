#include "playerStateIdle.h"
#include "playerStateMachine.h"

void PlayerStateIdle::Init()
{
	LoadAnimation("asset\\model\\player\\idle_Akai.fbx", "idle_Player");
	if (m_PlayerMachine != nullptr)
	{
		m_PlayerMachine->InitVelocity();
	}
}

void PlayerStateIdle::Unit()
{

}

void PlayerStateIdle::Update(const float& deltaTime)
{
	PlayerState::Update(deltaTime);
	if (m_PlayerMachine != nullptr)
	{
		m_PlayerMachine->InitVelocity();
	}
}

void PlayerStateIdle::ChangeStateControl()
{
	if (m_PlayerMachine == nullptr) return;
	
	// 優先順位順

	// 構えていると
	if (m_PlayerMachine->GetIsParryAttack())
	{
		ChangePlayerState(PLAYER_STATE::PARRY);
	}// 浮いている、ジャンプしていると
	else if (m_PlayerMachine->GetIsJump() || !m_PlayerMachine->GetIsGround())
	{
		ChangePlayerState(PLAYER_STATE::JUMP);
	}// 移動していると
	else if (m_PlayerMachine->GetMoveRandL() != MOVE_DIRECTION::NONE || m_PlayerMachine->GetMoveFandB() != MOVE_DIRECTION::NONE)
	{
		ChangePlayerState(PLAYER_STATE::RUN);
	}
}
