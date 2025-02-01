#pragma once
#include "staticmeshObject.h"

class LightningBall : public StaticMeshObject
{
private:
	const GameObject* m_TargetObject = nullptr;
	bool m_IsAttack = false;
	bool m_IsHoming = false;
	float m_Speed = 0.0f;
	float m_Damage = 0.0f;
	virtual void MoveControl(const float& deltaTime)override;
	void CollisionControl();
public:
	LightningBall() = delete;
	LightningBall(const GameObject* target,const float& speed);
	void Attack(const XMFLOAT3& shotPos, const float& damage);
};