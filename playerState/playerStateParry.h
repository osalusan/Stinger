#pragma once
#include "playerState.h"

class ObjectManager;
class PlayerStateParry final :public PlayerState
{
private:
	float m_CurrentTime = 0.0f;
	ObjectManager* m_ObjManagerCache = nullptr;
public:
	using PlayerState::PlayerState;
	virtual void Init()override;
	virtual void Unit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void ChangeStateControl()override;
};