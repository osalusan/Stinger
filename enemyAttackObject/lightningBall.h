#pragma once
#include "enemyAttackObject.h"

// ëOï˚êÈåæ
class LightningBallEffect;

class LightningBall final: public EnemyAttackObject
{
private:
	LightningBallEffect* m_LightningBallEffCache = nullptr;
	bool m_IsHoming = false;
	float m_Speed = 0.0f;
	virtual void MoveControl(const float& deltaTime)override;
	virtual bool CollisionControl()override;
	virtual void Finish()override;
public:
	LightningBall() = delete;
	LightningBall(const GameObject* target,const float& speed);
	virtual void Attack()override;
	virtual void Spawn(const XMFLOAT3& shotPos, const float& damage)override;
};