#pragma once
#include "enemyAttackObject.h"

// ëOï˚êÈåæ
class LightningFallEffect;
class LightningCharge;

class LightningFall final : public EnemyAttackObject
{
private:
	LightningFallEffect* m_LightningFallEffCache = nullptr;
	LightningCharge* m_LightningChargeCache = nullptr;
	virtual void AttackControl(const float& deltaTime)override;
public:
	LightningFall() = delete;
	LightningFall(const GameObject* target);
	virtual void Attack()override;
	virtual void Spawn(const XMFLOAT3& shotPos, const float& damage)override;
};