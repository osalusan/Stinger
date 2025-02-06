#pragma once
#include "object/gameObject.h"

class BoxCollisionComponent;

class EnemyAttackObject : public GameObject
{
protected:
	BoxCollisionComponent* m_BoxCollCache = nullptr;
	bool m_IsAttack = false;
	float m_Damage = 0.0f;
	virtual void MoveControl(const float& deltaTime) = 0;
	virtual bool CollisionControl();
public:
	EnemyAttackObject();
	virtual void Update(const float& deltaTime)override final;
	void Attack(const XMFLOAT3& shotPos, const float& damage);
	void KeepPos(const XMFLOAT3& shotPos);
};