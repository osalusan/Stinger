#pragma once
#include "playerState.h"
// TODO : β完成前に削除予定 / デバッグ用 
class PlayerStateJump :public PlayerState
{
private:
	float m_Accel = 0.0f;
	float m_CurrentTime = 0.0f;

public:
	using PlayerState::PlayerState;
	virtual void Init()override;
	virtual void Unit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void ChangeStateControl()override;
};