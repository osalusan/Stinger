#pragma once
#include "enemyAttackObject.h"

// 前方宣言
class LightningBallEffect;
class LightningBallCharge;
// プレイヤーに向かって誘導する雷の弾
class LightningBall final: public EnemyAttackObject
{
private:
	LightningBallEffect* m_LightningBallEffCache = nullptr;			// 雷の弾のビルボードエフェクト管理用
	LightningBallCharge* m_LightningBallChargeCache = nullptr;		// 雷のチャージのパーティクル管理用

	bool m_IsHoming = false;		// ホーミング中はプレイヤーの方向に回転する
	float m_Speed = 0.0f;			// 弾の速度
	virtual void AttackControl(const float& deltaTime)override;
	virtual bool CollisionDamageControl()override;
	virtual void Finish()override;
public:
	LightningBall() = delete;
	LightningBall(const GameObject* target,const float& speed);
	virtual void Attack()override;
	virtual void Spawn(const XMFLOAT3& shotPos, const float& damage, const float& balletTime)override;
};