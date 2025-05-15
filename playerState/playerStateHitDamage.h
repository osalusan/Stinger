#pragma once
#include "playerState.h"
// ダメージを受けたときのステート
class PlayerStateHitDamage final :public PlayerState
{
public:
	using PlayerState::PlayerState;
	virtual void Init()override;
	virtual void Unit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void ChangeStateControl()override;
};