#include "playerStateJump.h"

constexpr float INIT_ACCEL = 4200.0f;	// 初速度
constexpr float JUMP_MAX_TIME = 0.8f;	// ジャンプの上昇時間
constexpr float JUMP_RESIST = INIT_ACCEL / JUMP_MAX_TIME;	// 抵抗力

void PlayerStateJump::Init()
{
	if (m_PlayerMachine != nullptr)
	{
		// ジャンプ入力されていたら
		if (m_PlayerMachine->GetIsJump())
		{
			m_Accel = INIT_ACCEL;
		}
	}

}

void PlayerStateJump::Unit()
{
	m_Accel = 0.0f;
	m_CurrentTime = 0.0f;
}

void PlayerStateJump::Update(const float& deltaTime)
{
	if (m_PlayerMachine == nullptr) return;

	if (m_CurrentTime <= JUMP_MAX_TIME)
	{
		m_Accel -= JUMP_RESIST * deltaTime;
	}	

	m_PlayerMachine->SetVelocityY(m_Accel * deltaTime);

	m_CurrentTime += deltaTime;
}

void PlayerStateJump::ChangeStateControl()
{
	if (m_PlayerMachine == nullptr) return;

	if (m_PlayerMachine->GetIsGround() && !m_PlayerMachine->GetIsJump())
	{
		if (m_PlayerMachine->GetMoveRandL() != MOVE_DIRECTION::NONE || m_PlayerMachine->GetMoveFandB() != MOVE_DIRECTION::NONE)
		{
			ChangePlayerState(PLAYER_STATE::RUN);
		}
		else
		{
			ChangePlayerState(PLAYER_STATE::IDLE);
		}
	}
}
