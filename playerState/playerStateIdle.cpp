#include "playerStateIdle.h"
#include "playerStateMachine.h"

void PlayerStateIdle::Init()
{
	LoadAnimation("asset\\model\\player\\Idle_PaladinJNordstrom.fbx", "idle_Player");
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

	// �p���B�U���������ꂽ��
	if (m_PlayerMachine->GetIsParryAttackButton())
	{
		ChangePlayerState(PLAYER_STATE::ATTACK_PARRY);
	}// �ʏ�U���������ꂽ��
	else if (m_PlayerMachine->GetIsNormalAttackButton())
	{
		ChangePlayerState(PLAYER_STATE::ATTACK_NORMAL);
	}// �ړ����Ă����
	else if (m_PlayerMachine->GetMoveRandL() != MOVE_DIRECTION::NONE || m_PlayerMachine->GetMoveFandB() != MOVE_DIRECTION::NONE)
	{
		ChangePlayerState(PLAYER_STATE::RUN);
	}
}
