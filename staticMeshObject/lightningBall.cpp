#include "lightningBall.h"
#include "manager/objModelManager.h"
#include "component/boxCollisionComponent.h"

constexpr float HOMING_VALUE = 10.0f;	// ホーミングの強度
void LightningBall::MoveControl(const float& deltaTime)
{
	if (!m_IsAttack || m_TargetObject == nullptr) return;

	const XMFLOAT3& targetPos = m_TargetObject->GetPos();

	float currentAngle = m_Rotation.y;
	const float& targetAngle = atan2f(targetPos.x - m_Position.x, targetPos.z - m_Position.z);

	float angleDiff = targetAngle - currentAngle;
	while (angleDiff > XM_PI)
	{
		angleDiff -= XM_2PI;
	}
	while (angleDiff < -XM_PI)
	{
		angleDiff += XM_2PI;
	}

	// 少しずつ差を埋める
	currentAngle += angleDiff * HOMING_VALUE * deltaTime;

	m_Rotation.y = currentAngle;

	// 移動距離設定
	m_Position.x += (targetPos.x - m_Position.x) * m_Speed * deltaTime;
	m_Position.z += (targetPos.z - m_Position.z) * m_Speed * deltaTime;
}

LightningBall::LightningBall(const GameObject* target, const float& speed)
	:StaticMeshObject(STATICMESH_MODEL::SPHERE)
{
	ObjModelManager::ReservModel(m_Model, "asset\\model\\object\\sphere.obj");
	m_BoxCollCache = AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_ATTACK);
	m_Enable = false;
	m_TargetObject = target;
	m_Speed = speed;
}

void LightningBall::Attack(const XMFLOAT3& shotPos)
{
	m_Position = shotPos;
	m_IsAttack = true;
	m_Enable = true;
}
