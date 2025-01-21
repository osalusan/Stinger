#pragma once
#include "playerState.h"

class ObjectManager;
class PlayerStateHoldWeapon final:public PlayerState
{
private:
	ObjectManager* m_ObjManagerCache = nullptr;
	float m_CurrentTime = 0.0f;

public:
	using PlayerState::PlayerState;
	virtual void Init()override;
	virtual void Unit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void ChangeStateControl()override;
};