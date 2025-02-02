#include "lightningBall.h"
#include "manager/objModelManager.h"
#include "component/boxCollisionComponent.h"

constexpr float HOMING_VALUE = 26.0f;	// ホーミングの強度
constexpr float HOMING_RANGE = 10.0f;	// ホーミング終了の距離
constexpr XMFLOAT3 DEFALUT_SCALE = { 3.0f,3.0f,3.0f };

void LightningBall::MoveControl(const float& deltaTime)
{
	if (!m_IsAttack || m_TargetObject == nullptr) return;

	const XMFLOAT3& targetPos = m_TargetObject->GetPos();

	const float& vectorX = abs(targetPos.x - m_Position.x);
	const float& vectorY = abs(targetPos.y - m_Position.y);
	const float& vectorZ = abs(targetPos.z - m_Position.z);
	const float& range = vectorX + vectorZ + vectorY;

	// 一定範囲でホーミング終了
	if (range < HOMING_RANGE)
	{
		m_IsHoming = false;
		CollisionControl();
	}
	else
	{
		if (!m_IsHoming)
		{
			m_Enable = false;
		}
	}

	if (m_IsHoming)
	{
		// 横
		float currentAngleY = m_Rotation.y;
		const float& targetAngleY = atan2f(targetPos.x - m_Position.x, targetPos.z - m_Position.z);

		float angleDiffY = targetAngleY - currentAngleY;
		while (angleDiffY > XM_PI)
		{
			angleDiffY -= XM_2PI;
		}
		while (angleDiffY < -XM_PI)
		{
			angleDiffY += XM_2PI;
		}

		// 少しずつ差を埋める
		currentAngleY += angleDiffY * HOMING_VALUE * deltaTime;

		m_Rotation.y = currentAngleY;

		// 縦
		float directionX = targetPos.x - m_Position.x;
		float directionY = targetPos.y - m_Position.y;
		float directionZ = targetPos.z - m_Position.z;
		float horizontalDist = sqrtf(directionX * directionX + directionZ * directionZ);

		float currentAngleX = m_Rotation.x;
		float targetAngleX = atan2f(directionY, horizontalDist);

		float angleDiffX = targetAngleX - currentAngleX;
		while (angleDiffX > XM_PI) 
		{ 
			angleDiffX -= XM_2PI; 
		}
		while (angleDiffX < -XM_PI) 
		{ 
			angleDiffX += XM_2PI; 
		}

		// 少しずつ差を埋める
		currentAngleX += angleDiffX * HOMING_VALUE * deltaTime;
		m_Rotation.x = currentAngleX;
	}

	// 移動距離設定
	m_Position.x += GetForward().x * m_Speed * deltaTime;
	m_Position.y -= GetForward().y * m_Speed * deltaTime;
	m_Position.z += GetForward().z * m_Speed * deltaTime;
}

void LightningBall::CollisionControl()
{
	if (m_BoxCollCache == nullptr) return;

	if (m_BoxCollCache->CheckHitObject(COLLISION_TAG::PLAYER))
	{
		Player* player = m_BoxCollCache->GetHitGameObject<Player>();
		if (player == nullptr) return;

		player->TakeDamage(m_Damage);

		m_Enable = false;
	}

}

LightningBall::LightningBall(const GameObject* target, const float& speed)
	:StaticMeshObject(STATICMESH_MODEL::SPHERE)
{
	ObjModelManager::ReservModel(m_Model, "asset\\model\\object\\sphere.obj");
	m_BoxCollCache = AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_ATTACK);
	m_Scale = DEFALUT_SCALE;
	m_Enable = false;
	m_TargetObject = target;
	m_Speed = speed;
}

void LightningBall::Attack(const XMFLOAT3& shotPos,const float& damage)
{
	m_Position = shotPos;
	m_IsAttack = true;
	m_Enable = true;
	m_IsHoming = true;
	m_Damage = damage;
}
