#include "playerStateIdle.h"
#include "playerStateMachine.h"

void PlayerStateIdle::Init()
{
	LoadAnimation("asset\\model\\player\\Idle_PaladinJNordstrom.fbx", "idle");
	if (m_PlayerMachine != nullptr)
	{
		m_PlayerMachine->InitVelocity();
		m_PlayerMachine->SetAnimeBlendTimeValue(m_BlendTime);
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

	// UŒ‚‚ðŽó‚¯‚½‚ç
	if (m_PlayerMachine->GetIsHitAttack())
	{
		ChangePlayerState(PLAYER_STATE::HITDAMAGE);
	}
	// ƒpƒŠƒBUŒ‚‚ª‰Ÿ‚³‚ê‚½‚ç
	else if (m_PlayerMachine->GetIsParryAttackButton())
	{
		ChangePlayerState(PLAYER_STATE::ATTACK_PARRY);
	}
	// ’ÊíUŒ‚‚ª‰Ÿ‚³‚ê‚½‚ç
	else if (m_PlayerMachine->GetIsNormalAttackButton())
	{
		ChangePlayerState(PLAYER_STATE::ATTACK_NORMAL);
	}
	// ‰ñ”ð‚ª‰Ÿ‚³‚ê‚½‚ç
	else if (m_PlayerMachine->GetIsRollingButton())
	{
		ChangePlayerState(PLAYER_STATE::ROLLING);
	}
	// ˆÚ“®‚µ‚Ä‚¢‚é‚Æ
	else if (m_PlayerMachine->GetMoveRandL() != MOVE_DIRECTION::NONE || m_PlayerMachine->GetMoveFandB() != MOVE_DIRECTION::NONE)
	{
		ChangePlayerState(PLAYER_STATE::RUN);
	}
}
