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
	virtual void MoveControl(const float& deltaTime) = 0;
	virtual bool CollisionControl();
	virtual void Finish();
public:
	EnemyAttackObject() = delete;
	EnemyAttackObject(const GameObject* target);
	virtual void Update(const float& deltaTime)override final;
	virtual void Spawn(const XMFLOAT3& shotPos, const float& damage);
	virtual void Attack() = 0;
	virtual void KeepPos(const XMFLOAT3& shotPos);
};