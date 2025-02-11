#pragma once
#include "enemyAttackObject.h"

// ëOï˚êÈåæ
class LightningFallEffect;

class LightningFall final : public EnemyAttackObject
{
private:
	LightningFallEffect* m_LightningFallEffCache = nullptr;
	virtual void AttackControl(const float& deltaTime)override;
public:
	LightningFall() = delete;
	LightningFall(const GameObject* target);
	virtual void Attack()override;
	virtual void Spawn(const XMFLOAT3& shotPos, const float& damage)override;
};