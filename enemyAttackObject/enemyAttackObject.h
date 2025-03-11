#pragma once
#include "object/gameObject.h"

class BoxCollisionComponent;

class EnemyAttackObject : public GameObject
{
protected:
	const GameObject* m_TargetObject = nullptr;
	BoxCollisionComponent* m_BoxCollCache = nullptr;
	bool m_IsAttack = false;
	float m_Damage = 0.0f;
	virtual void AttackControl(const float& deltaTime) = 0;
	virtual bool CollisionControl();
	virtual void Finish() = 0;
public:
	EnemyAttackObject() = delete;
	EnemyAttackObject(const GameObject* target);
	virtual void Update(const float& deltaTime)override final;
	virtual void Spawn(const XMFLOAT3& shotPos, const float& damage);
	virtual void Spawn(const XMFLOAT3& shotPos, const float& damage, const float& balletTime);
	virtual void Attack() = 0;
	virtual void KeepPos(const XMFLOAT3& shotPos);
};