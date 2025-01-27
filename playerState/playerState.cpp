#include "playerState.h"
#include "manager/fbxModelManager.h"
#include "character/player.h"

// --------------------- protected ---------------------
void PlayerState::LoadAnimation(const std::string& fileName, const std::string& animationName)
{
	m_AnimeName = animationName;
	if (!m_LoadAnimation)
	{
		FbxModelManager::ReservAnimation(ANIMETION_MODEL::PLAYER, fileName, animationName);
		m_LoadAnimation = true;
	}
}

void PlayerState::RotToCameraDirection(const float& deltaTime)
{
	if (m_PlayerMachine == nullptr) return;
	const XMFLOAT3& forwardVector = m_PlayerMachine->GetCameraForward();

	float currentAngle = m_PlayerMachine->GetRotation().y;
	const float& targetAngle = atan2f(forwardVector.x, forwardVector.z);

	float angleDiff = targetAngle - currentAngle;
	while (angleDiff > XM_PI)
	{
		angleDiff -= XM_2PI;
	}
	while (angleDiff < -XM_PI)
	{
		angleDiff += XM_2PI;
	}

	// Get�̂� / �ҏW�s��
	const Player* playerCache = m_PlayerMachine->GetPlayerCache();
	if (playerCache == nullptr) return;
	const float& rotSpeed = playerCache->GetRotSpeed();
	// ���������𖄂߂�
	currentAngle += angleDiff * rotSpeed * deltaTime;

	m_PlayerMachine->SetRotationY(currentAngle);
}

// --------------------- public ---------------------
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
