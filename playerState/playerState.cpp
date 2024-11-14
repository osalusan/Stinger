#include "playerState.h"

PlayerState::~PlayerState()
{
	m_PlayerMachine = nullptr;
}

PlayerState::PlayerState(PlayerStateMachine* machine)
{
	if (machine != nullptr && m_PlayerMachine == nullptr)
	{
		m_PlayerMachine = machine;
	}
}

void PlayerState::ChangePlayerState(const PLAYER_STATE& playerState)
{
	if (m_PlayerMachine != nullptr)
	{
		m_PlayerMachine->SetPlayerState(playerState);
	}
}
