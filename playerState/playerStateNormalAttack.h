#pragma once
#include "playerState.h"

class ObjectManager;
class BossEnemy;
class PlayerStateNormalAttack final :public PlayerState
{
private:
	float m_CurrentTime = 0.0f;
	bool m_AttackAccept = false;
	ObjectManager* m_ObjManagerCache = nullptr;
	BossEnemy* m_BossCache = nullptr;
public:
	using PlayerState::PlayerState;
	virtual void Init()override;
	virtual void Unit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void ChangeStateControl()override;
};