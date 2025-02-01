#pragma once
#include "staticmeshObject.h"

class LightningBall : public StaticMeshObject
{
private:
	const GameObject* m_TargetObject = nullptr;
	bool m_IsAttack = false;
	float m_Speed = 0.0f;
	virtual void MoveControl(const float& deltaTime)override;
public:
	LightningBall() = delete;
	LightningBall(const GameObject* target,const float& speed);
	void Attack(const XMFLOAT3& shotPos);
};