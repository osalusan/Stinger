#pragma once
#include "enemyAttackObject.h"

// 前方宣言
class LightningFallEffect;
class LightningCharge;
class MeshFiled;
// 落雷
class LightningFall final : public EnemyAttackObject
{
private:
	LightningFallEffect* m_LightningFallEffCache = nullptr;		// 落雷のビルボードエフェクト管理用
	LightningCharge* m_LightningChargeCache = nullptr;			// 雷のチャージのパーティクル管理用
	MeshFiled* m_FiledCache = nullptr;							// 落雷の着地点を地面に合わせる為
	virtual void AttackControl(const float& deltaTime)override;
	virtual void Finish()override;
public:
	LightningFall() = delete;
	LightningFall(const GameObject* target);
	virtual void Attack()override;
	void Spawn(const XMFLOAT3& shotPos, const float& damage, const float& baletTime);
};