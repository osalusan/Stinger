#pragma once
#include "enemyAttackObject.h"

// ëOï˚êÈåæ
class LightningFallEffect;
class LightningCharge;
class MeshFiled;

class LightningFall final : public EnemyAttackObject
{
private:
	LightningFallEffect* m_LightningFallEffCache = nullptr;
	LightningCharge* m_LightningChargeCache = nullptr;
	MeshFiled* m_FiledCache = nullptr;
	virtual void AttackControl(const float& deltaTime)override;
	virtual void Finish()override;
public:
	LightningFall() = delete;
	LightningFall(const GameObject* target);
	virtual void Attack()override;
	void Spawn(const XMFLOAT3& shotPos, const float& damage, const float& baletTime);
};