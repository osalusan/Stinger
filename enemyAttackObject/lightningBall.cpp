#include "lightningBall.h"
#include "manager/objModelManager.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "component/boxCollisionComponent.h"
#include "scene/scene.h"
#include "billboard/lightningBallEffect.h"

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

bool LightningBall::CollisionControl()
{
	if (EnemyAttackObject::CollisionControl())
	{
		if (m_LightningBallEffCache == nullptr) return false;
		m_LightningBallEffCache->SetEnable(false);
	}

}

LightningBall::LightningBall(const GameObject* target, const float& speed)
{
	m_Scale = DEFALUT_SCALE;
	m_TargetObject = target;
	m_Speed = speed;

	Scene* scene = SceneManager::GetScene();
	if (scene == nullptr) return;
	ObjectManager* objManager = scene->GetObjectManager();
	if (objManager == nullptr) return;

	if (m_LightningBallEffCache == nullptr)
	{
		m_LightningBallEffCache = objManager->AddGameObjectArg<LightningBallEffect>(OBJECT::BILLBOARD,this);
	}
}

void LightningBall::Attack(const XMFLOAT3& shotPos,const float& damage)
{
	EnemyAttackObject::Attack(shotPos,damage);
	m_IsHoming = true;

	if (m_LightningBallEffCache == nullptr) return;
	m_LightningBallEffCache->UseBillboard();
}

void LightningBall::KeepPos(const XMFLOAT3& shotPos)
{
	EnemyAttackObject::KeepPos(shotPos);

	if (m_LightningBallEffCache == nullptr) return;
	m_LightningBallEffCache->UseBillboard();
}
