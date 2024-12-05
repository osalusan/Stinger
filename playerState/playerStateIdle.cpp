#include "playerStateIdle.h"
#include "playerStateMachine.h"

constexpr const char* IDLE_PLAYER = "IdlePlayer";

void PlayerStateIdle::Init()
{
	LoadAnimation("asset\\model\\Akai_Idle.fbx", IDLE_PLAYER);
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

	m_PlayerMachine->SetAnimation(IDLE_PLAYER);
}

void PlayerStateIdle::ChangeStateControl()
{
	if (m_PlayerMachine == nullptr) return;
	
	// �D�揇�ʏ�

	if (m_PlayerMachine->GetIsJump() || !m_PlayerMachine->GetIsGround())
	{// �����Ă���A�W�����v���Ă����
		ChangePlayerState(PLAYER_STATE::JUMP);
	}
	else if (m_PlayerMachine->GetMoveRandL() != MOVE_DIRECTION::NONE || m_PlayerMachine->GetMoveFandB() != MOVE_DIRECTION::NONE)
	{// �ړ����Ă����
		ChangePlayerState(PLAYER_STATE::RUN);
	}
}
