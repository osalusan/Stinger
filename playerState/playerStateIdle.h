#pragma once
#include "playerState.h"

class PlayerStateIdle :public PlayerState
{
public:
	using PlayerState::PlayerState;
	virtual void Init()override;
	virtual void Unit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void ChangeStateControl()override;
};