#include "playerStateRun.h"

constexpr float MOVE_SPEED_MAWJ = 6000.0f;
constexpr float ROTATE_SPEED_PLAYER_RUN = 5.0f;	// 回転速度
constexpr const char* RUNNING_PLAYER = "RunningPlayer";

void PlayerStateRun::Init()
{
	LoadAnimation("asset\\model\\Akai_Run.fbx", RUNNING_PLAYER);
}

void PlayerStateRun::Unit()
{

}

void PlayerStateRun::Update(const float& deltaTime)
{
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

	//斜めでも速度が変わらないように
	XMVECTOR velocityVec = XMLoadFloat3(&moveVector);
	XMVECTOR normalizdVelocityVec = XMVector3Normalize(velocityVec);
	XMFLOAT3 normalizeVelocity = {};
	XMStoreFloat3(&normalizeVelocity, normalizdVelocityVec);

	m_PlayerMachine->SetVelocityX(normalizeVelocity.x * MOVE_SPEED_MAWJ * deltaTime);
	m_PlayerMachine->SetVelocityZ(normalizeVelocity.z * MOVE_SPEED_MAWJ * deltaTime);

	m_PlayerMachine->SetAnimation(RUNNING_PLAYER);

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

	// 少しずつ差を埋める
	currentAngle += angleDiff * ROTATE_SPEED_PLAYER_RUN * deltaTime;

	if (normalizeVelocity.x != 0.0f || normalizeVelocity.z != 0.0f)
	{
		m_PlayerMachine->SetRotationY(currentAngle);
	}
}

void PlayerStateRun::ChangeStateControl()
{
	if (m_PlayerMachine == nullptr) return;

	if (m_PlayerMachine->GetIsJump() || !m_PlayerMachine->GetIsGround())
	{// 浮いている、ジャンプしていると
		ChangePlayerState(PLAYER_STATE::JUMP);
	}
	else if (m_PlayerMachine->GetMoveRandL() == MOVE_DIRECTION::NONE && m_PlayerMachine->GetMoveFandB() == MOVE_DIRECTION::NONE)
	{// 移動していないと
		ChangePlayerState(PLAYER_STATE::IDLE);
	}
}
