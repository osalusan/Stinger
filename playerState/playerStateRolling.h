#pragma once
#include "playerState.h"

class PlayerStateRolling final :public PlayerState
{
private:
	XMFLOAT3 m_MoveVector = {};
	XMFLOAT3 m_NormalizeVelocity = {};
	float m_MaxAnimTime = 0.0f;
	float m_RollingSpeed = 0.0f;
	float m_RotSpeed = 0.0f;
public:
	using PlayerState::PlayerState;
	virtual void Init()override;
	virtual void Unit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void ChangeStateControl()override;
};