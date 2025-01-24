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
	
	// �D�揇�ʏ�

	// �\���Ă����
	if (m_PlayerMachine->GetIsParryAttack())
	{
		ChangePlayerState(PLAYER_STATE::PARRY);
	}// �����Ă���A�W�����v���Ă����
	else if (m_PlayerMachine->GetIsJump() || !m_PlayerMachine->GetIsGround())
	{
		ChangePlayerState(PLAYER_STATE::JUMP);
	}// �ړ����Ă����
	else if (m_PlayerMachine->GetMoveRandL() != MOVE_DIRECTION::NONE || m_PlayerMachine->GetMoveFandB() != MOVE_DIRECTION::NONE)
	{
		ChangePlayerState(PLAYER_STATE::RUN);
	}
}
