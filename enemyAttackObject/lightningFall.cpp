#include "lightningFall.h"
#include "manager/objModelManager.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "component/boxCollisionComponent.h"
#include "scene/scene.h"
#include "billboard/lightningFallEffect.h"
#include "particle/lightningCharge.h"

constexpr XMFLOAT3 DEFALUT_SCALE = { 1.0f,15.0f,1.0f };

// -------------------------------------- private --------------------------------------

void LightningFall::AttackControl(const float& deltaTime)
{
	m_LightningChargeCache->SetPos(m_Position);

	if (m_LightningChargeCache->Finish())
	{
		m_LightningFallEffCache->Attack();
		m_IsAttack = true;
	}
	if (m_LightningFallEffCache->Finish())
	{
		m_Enable = false;
	}
}

// -------------------------------------- public --------------------------------------

LightningFall::LightningFall(const GameObject* target)
	:EnemyAttackObject(target)
{
	m_Scale = DEFALUT_SCALE;
	m_TargetObject = target;

	m_Position = { 10.0f,0.0f,10.0f };
	Scene* scene = SceneManager::GetScene();
	if (scene == nullptr) return;
	ObjectManager* objManager = scene->GetObjectManager();
	if (objManager == nullptr) return;

	if (m_LightningFallEffCache == nullptr)
	{
		m_LightningFallEffCache = objManager->AddGameObjectArg<LightningFallEffect>(OBJECT::BILLBOARD, this);
	}
	if (m_LightningChargeCache == nullptr)
	{
		m_LightningChargeCache = objManager->AddGameObjectArg<LightningCharge>(OBJECT::BILLBOARD,false);
	}
	m_Enable = false;
}

void LightningFall::Attack()
{
	//ƒˆ‰¼‘zŠÖ”‚Ìˆ×‰¼ŽÀ‘•
}

void LightningFall::Spawn(const XMFLOAT3& shotPos, const float& damage, const float& baletTime)
{
	if (m_Enable) return;

	m_Position = shotPos;
	m_Enable = true;
	m_Damage = damage;
	if (m_BoxCollCache != nullptr)
	{
		m_BoxCollCache->SetCollisionInfo(m_Position, m_Scale, { 0.0f,0.0f,0.0f }, { 2.0f,2.0f,2.0f }, GetRotationMatrix());
	}

	m_LightningChargeCache->Start(baletTime);
	m_IsAttack = false;
}
