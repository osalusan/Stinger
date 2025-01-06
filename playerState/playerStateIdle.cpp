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
	
	// —Dæ‡ˆÊ‡

	// •‚‚¢‚Ä‚¢‚éAƒWƒƒƒ“ƒv‚µ‚Ä‚¢‚é‚Æ
	if (m_PlayerMachine->GetIsJump() || !m_PlayerMachine->GetIsGround())
	{
		ChangePlayerState(PLAYER_STATE::JUMP);
	}// ˆÚ“®‚µ‚Ä‚¢‚é‚Æ
	else if (m_PlayerMachine->GetMoveRandL() != MOVE_DIRECTION::NONE || m_PlayerMachine->GetMoveFandB() != MOVE_DIRECTION::NONE)
	{
		ChangePlayerState(PLAYER_STATE::RUN);
	}
}
