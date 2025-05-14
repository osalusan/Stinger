#include "lightningBall.h"
#include "manager/objModelManager.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "component/boxCollisionComponent.h"
#include "scene/scene.h"
#include "billboard/lightningBallEffect.h"
#include "particle/lightningBallChage.h"

constexpr float HOMING_VALUE = 26.0f;	// ホーミングの強度
constexpr float HOMING_RANGE = 10.0f;	// ホーミング終了の距離
constexpr XMFLOAT3 DEFALUT_SCALE = { 3.0f,3.0f,3.0f };

// -------------------------------------- private --------------------------------------
void LightningBall::AttackControl(const float& deltaTime)
{

	if (m_LightningBallChargeCache == nullptr) return;

	if (m_LightningBallChargeCache->Finish())
	{
		m_IsAttack = true;
	}


	if (!m_IsHoming || m_TargetObject == nullptr) return;

	const XMFLOAT3& targetPos = m_TargetObject->GetPos();

	const float& vectorX = abs(targetPos.x - m_Position.x);
	const float& vectorY = abs(targetPos.y - m_Position.y);
	const float& vectorZ = abs(targetPos.z - m_Position.z);
	const float& range = vectorX + vectorZ + vectorY;

	// 一定範囲でホーミング終了
	if (range < HOMING_RANGE)
	{
		m_IsHoming = false;
		CollisionDamageControl();
	}
	else
	{
		if (!m_IsHoming)
		{
			m_Enable = false;
		}
	}

	// プレイヤーの方向に回転して、fowardを取得し前に進む
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

bool LightningBall::CollisionDamageControl()
{
	if (EnemyAttackObject::CollisionDamageControl())
	{
		if (m_LightningBallEffCache == nullptr) return false;
		m_LightningBallEffCache->SetEnable(false);
		m_IsHoming = false;
	}
	return false;
}

void LightningBall::Finish()
{
	m_Enable = false;
	m_IsAttack = false;
	m_Rotation = {};
}

// -------------------------------------- public --------------------------------------

LightningBall::LightningBall(const GameObject* target, const float& speed)
	:EnemyAttackObject(target)
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
	if (m_LightningBallChargeCache == nullptr)
	{
		m_LightningBallChargeCache = objManager->AddGameObjectArg<LightningBallCharge>(OBJECT::PARTICLE, false);
	}
}

void LightningBall::Attack()
{
	if (m_Enable)
	{
		m_IsHoming = true;
	}
}

void LightningBall::Spawn(const XMFLOAT3& shotPos, const float& damage, const float& lifeTime)
{
	if (m_Enable) return;

	EnemyAttackObject::Spawn(shotPos, damage, lifeTime);

	if (m_TargetObject == nullptr) return;

	m_Position = shotPos;
	const XMFLOAT3& targetPos = m_TargetObject->GetPos();
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

	m_Rotation.y = angleDiffY;

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

	m_Rotation.x = angleDiffX;

	m_IsAttack = false;

	if (m_LightningBallEffCache != nullptr)
	{
		m_LightningBallEffCache->UseBillboard();
	}
	if (m_LightningBallChargeCache != nullptr)
	{
		m_LightningBallChargeCache->SetPos(m_Position);
		m_LightningBallChargeCache->Start(lifeTime);
	}
}
