#include "playerState.h"
#include "manager/fbxModelManager.h"

void PlayerState::LoadAnimation(const std::string& fileName, const std::string& animationName)
{
	m_AnimeName = animationName;
	if (!m_LoadAnimation)
	{
		FbxModelManager::ReservAnimation(ANIMETION_MODEL::PLAYER, fileName, animationName);
		m_LoadAnimation = true;
	}
}

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

void PlayerState::Update(const float& deltaTime)
{
	if (m_PlayerMachine == nullptr) return;

	if (m_AnimeName != "")
	{
		m_PlayerMachine->SetAnimation(m_AnimeName);
	}	
}
