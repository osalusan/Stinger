#include "playerStateNormalAttack.h"
#include "playerStateMachine.h"

void PlayerStateNormalAttack::Init()
{
	// LoadAnimation("asset\\model\\player\\idle_Akai.fbx", "idle_Player");
	m_CurrentTime = 0.0f;
	if (m_PlayerMachine != nullptr)
	{
		m_PlayerMachine->InitVelocity();
	}
}

void PlayerStateNormalAttack::Unit()
{
	m_CurrentTime = 0.0f;
}

void PlayerStateNormalAttack::Update(const float& deltaTime)
{
	PlayerState::Update(deltaTime);
	m_CurrentTime += deltaTime;
	if (m_PlayerMachine != nullptr)
	{
		m_PlayerMachine->InitVelocity();
	}
}

void PlayerStateNormalAttack::ChangeStateControl()
{
	if (m_PlayerMachine == nullptr) return;

	// �D�揇�ʏ�

	// TODO :�A�j���[�V�����̎��Ԃ�ݒ�
	if (m_CurrentTime >= 1.0f)
	{
		ChangePlayerState(PLAYER_STATE::IDLE);
	}
}

