#include "playerStateRolling.h"
#include "manager/fbxModelManager.h"
#include "renderer/fbxModelRenderer.h"
#include "character/player.h"

void PlayerStateRolling::Init()
{
	LoadAnimation("asset\\model\\player\\rolling_PaladinJNordstrom.fbx", "rolling_Player");
	m_CurrentTime = 0.0f;
}

void PlayerStateRolling::Unit()
{
	m_CurrentTime = 0.0f;
}

void PlayerStateRolling::Update(const float& deltaTime)
{
	PlayerState::Update(deltaTime);
	m_CurrentTime += deltaTime;

	// ‰‰ñ‚Ì‚Ý
	if (m_MaxAnimTime == 0.0f)
	{
		if (FbxModelRenderer* model = FbxModelManager::GetAnimationModel(ANIMETION_MODEL::PLAYER))
		{
			m_MaxAnimTime = model->GetMaxAnimeTime(m_AnimeName);
		}
	}

	if (m_PlayerMachine == nullptr) return;
	const XMFLOAT3& forwardVector = m_PlayerMachine->GetCameraForward();
	const XMFLOAT3& rightVector = m_PlayerMachine->GetCameraRight();

	XMFLOAT3 moveVector = {};

	if (m_PlayerMachine->GetMoveRandL() == MOVE_DIRECTION::RIGHT)
	{
		moveVector.x += rightVector.x;
		moveVector.z += rightVector.z;
	}
	else if (m_PlayerMachine->GetMoveRandL() == MOVE_DIRECTION::LEFT)
	{
		moveVector.x += (-rightVector.x);
		moveVector.z += (-rightVector.z);
	}

	if (m_PlayerMachine->GetMoveFandB() == MOVE_DIRECTION::FORWARD)
	{
		moveVector.x += forwardVector.x;
		moveVector.z += forwardVector.z;
	}
	else if (m_PlayerMachine->GetMoveFandB() == MOVE_DIRECTION::BACKWARD)
	{
		moveVector.x += (-forwardVector.x);
		moveVector.z += (-forwardVector.z);
	}

	//ŽÎ‚ß‚Å‚à‘¬“x‚ª•Ï‚í‚ç‚È‚¢‚æ‚¤‚É
	XMVECTOR velocityVec = XMLoadFloat3(&moveVector);
	XMVECTOR normalizdVelocityVec = XMVector3Normalize(velocityVec);
	XMFLOAT3 normalizeVelocity = {};
	XMStoreFloat3(&normalizeVelocity, normalizdVelocityVec);

	// Get‚Ì‚Ý / •ÒW•s‰Â
	const Player* playerCache = m_PlayerMachine->GetPlayerCache();
	if (playerCache == nullptr) return;
	const float& rollingSpeed = playerCache->GetMoveSpeed() * playerCache->GetRollingSpeedValue();
	const float& rotSpeed = playerCache->GetRotSpeed();

	m_PlayerMachine->SetVelocityX(normalizeVelocity.x * rollingSpeed * deltaTime);
	m_PlayerMachine->SetVelocityZ(normalizeVelocity.z * rollingSpeed * deltaTime);

	float currentAngle = m_PlayerMachine->GetRotation().y;
	const float& targetAngle = atan2f(normalizeVelocity.x, normalizeVelocity.z);

	float angleDiff = targetAngle - currentAngle;
	while (angleDiff > XM_PI)
	{
		angleDiff -= XM_2PI;
	}
	while (angleDiff < -XM_PI)
	{
		angleDiff += XM_2PI;
	}

	// ­‚µ‚¸‚Â·‚ð–„‚ß‚é
	currentAngle += angleDiff * rotSpeed * deltaTime;

	if (normalizeVelocity.x != 0.0f || normalizeVelocity.z != 0.0f)
	{
		m_PlayerMachine->SetRotationY(currentAngle);
	}
}

void PlayerStateRolling::ChangeStateControl()
{
	if (m_CurrentTime >= m_MaxAnimTime)
	{
		ChangePlayerState(PLAYER_STATE::IDLE);
	}
}
