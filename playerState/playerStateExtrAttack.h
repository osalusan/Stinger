#pragma once
#include "playerState.h"

class ObjectManager;
class BossEnemy;
class PlayerStateExtrAttack final :public PlayerState
{
private:
	ObjectManager* m_ObjManagerCache = nullptr;
	BossEnemy* m_BossCache = nullptr;

	bool m_AttackAccept = false;
	float m_DamageValue = 0.0f;
public:
	using PlayerState::PlayerState;
	virtual void Init()override;
	virtual void Unit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void ChangeStateControl()override;
	bool CheckAttackAccept();
};