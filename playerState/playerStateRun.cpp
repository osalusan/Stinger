#include "playerStateRun.h"

constexpr float MOVE_SPEED_MAWJ = 2000.0f;
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
	XMVECTOR normalizedVelocityVec = XMVector3Normalize(velocityVec);
	XMFLOAT3 normalizedVelocity = {};
	XMStoreFloat3(&normalizedVelocity, normalizedVelocityVec);

	m_PlayerMachine->SetVelocityX(normalizedVelocity.x * MOVE_SPEED_MAWJ * deltaTime);
	m_PlayerMachine->SetVelocityZ(normalizedVelocity.z * MOVE_SPEED_MAWJ * deltaTime);

	m_PlayerMachine->SetAnimation(RUNNING_PLAYER);
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
