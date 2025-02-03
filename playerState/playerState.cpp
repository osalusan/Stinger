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
	// Getのみ / 編集不可
	if (m_PlayerCache == nullptr)
	{
		const Player* playerCache = m_PlayerMachine->GetPlayerCache();
		m_PlayerCache = playerCache;
		if (m_PlayerCache == nullptr) return;
	}
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

	const float& rotSpeed = m_PlayerCache->GetRotSpeed();
	// 少しずつ差を埋める
	currentAngle += angleDiff * rotSpeed * deltaTime;

	m_PlayerMachine->SetRotationY(currentAngle);
}

void PlayerState::RotToInputKeyDirection(const float& deltaTime, const bool& rotAway)
{
	if (m_PlayerMachine == nullptr) return;
	// Getのみ / 編集不可
	if (m_PlayerCache == nullptr)
	{
		const Player* playerCache = m_PlayerMachine->GetPlayerCache();
		m_PlayerCache = playerCache;
		if (m_PlayerCache == nullptr) return;
	}
	const XMFLOAT3& forwardVector = m_PlayerMachine->GetCameraForward();
	const XMFLOAT3& rightVector = m_PlayerMachine->GetCameraRight();

	XMFLOAT3 keyDirection = {};

	if (m_PlayerMachine->GetMoveRandL() == MOVE_DIRECTION::RIGHT)
	{
		keyDirection.x += rightVector.x;
		keyDirection.z += rightVector.z;
	}
	else if (m_PlayerMachine->GetMoveRandL() == MOVE_DIRECTION::LEFT)
	{
		keyDirection.x += (-rightVector.x);
		keyDirection.z += (-rightVector.z);
	}

	if (m_PlayerMachine->GetMoveFandB() == MOVE_DIRECTION::FORWARD)
	{
		keyDirection.x += forwardVector.x;
		keyDirection.z += forwardVector.z;
	}
	else if (m_PlayerMachine->GetMoveFandB() == MOVE_DIRECTION::BACKWARD)
	{
		keyDirection.x += (-forwardVector.x);
		keyDirection.z += (-forwardVector.z);
	}

	if (keyDirection.x == 0.0f && keyDirection.z == 0.0f) return;

	float currentAngle = m_PlayerMachine->GetRotation().y;
	const float& targetAngle = atan2f(keyDirection.x, keyDirection.z);
	if (!rotAway)
	{
		float angleDiff = targetAngle - currentAngle;
		while (angleDiff > XM_PI)
		{
			angleDiff -= XM_2PI;
		}
		while (angleDiff < -XM_PI)
		{
			angleDiff += XM_2PI;
		}

		const float& rotSpeed = m_PlayerCache->GetRotSpeed();
		// 少しずつ差を埋める
		currentAngle += angleDiff * rotSpeed * deltaTime;
	}
	else if (rotAway)
	{
		currentAngle = targetAngle;
	}


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
