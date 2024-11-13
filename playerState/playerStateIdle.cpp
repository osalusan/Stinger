#include "playerStateIdle.h"
#include "playerStateMachine.h"

void PlayerStateIdle::Init()
{
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
	if (m_PlayerMachine != nullptr)
	{
		m_PlayerMachine->InitVelocity();
	}
}

void PlayerStateIdle::ChangeStateControl()
{
	if (m_PlayerMachine == nullptr) return;
	
	// —Dæ‡ˆÊ‡

	if (m_PlayerMachine->GetIsJump())
	{
		ChangePlayerState(PLAYER_STATE::JUMP);
	}
	else if (m_PlayerMachine->GetMoveRandL() != MOVE_DIRECTION::NONE || m_PlayerMachine->GetMoveFandB() != MOVE_DIRECTION::NONE)
	{
		ChangePlayerState(PLAYER_STATE::RUN);
	}


}
