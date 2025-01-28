#include "playerState.h"
#include "manager/fbxModelManager.h"
#include "character/player.h"

// --------------------- protected ---------------------
void PlayerState::LoadAnimation(const std::string& fileName, const std::string& animationName)
{
	m_AnimName = animationName;
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

	// Get‚Ì‚Ý / •ÒW•s‰Â
	const Player* playerCache = m_PlayerMachine->GetPlayerCache();
	if (playerCache == nullptr) return;
	const float& rotSpeed = playerCache->GetRotSpeed();
	// ­‚µ‚¸‚Â·‚ð–„‚ß‚é
	currentAngle += angleDiff * rotSpeed * deltaTime;

	m_PlayerMachine->SetRotationY(currentAngle);
}

float PlayerState::FindStateData(const std::unordered_map<std::string, float>& stateData, const std::string& dataName)
{
	if (stateData.count(dataName) >= 1)
	{
		return stateData.at(dataName);
	}
	else
	{
		return 0.0f;
	}
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
	m_BlendTime = DEFAULT_BLEND_VALUE;
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

	if (m_AnimName != "")
	{
		m_PlayerMachine->SetAnimation(m_AnimName);
	}
}
