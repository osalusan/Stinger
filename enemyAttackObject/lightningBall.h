#pragma once
#include "enemyAttackObject.h"

// ëOï˚êÈåæ
class LightningBallEffect;

class LightningBall : public EnemyAttackObject
{
private:
	const GameObject* m_TargetObject = nullptr;
	LightningBallEffect* m_LightningBallEffCache = nullptr;
	bool m_IsHoming = false;
	float m_Speed = 0.0f;
	virtual void MoveControl(const float& deltaTime)override;
	bool CollisionControl()override;
public:
	LightningBall() = delete;
	LightningBall(const GameObject* target,const float& speed);
	void Attack(const XMFLOAT3& shotPos, const float& damage);
	void KeepPos(const XMFLOAT3& shotPos);
};