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
		m_IsAttack = false;
		m_Enable = false;
	}
}

void LightningFall::Finish()
{
	m_IsAttack = false;
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

	if (m_FiledCache == nullptr)
	{
		m_FiledCache = objManager->GetMeshFiled();
	}
	if (m_LightningFallEffCache == nullptr)
	{
		m_LightningFallEffCache = objManager->AddGameObjectArg<LightningFallEffect>(OBJECT::BILLBOARD, this);
	}
	if (m_LightningChargeCache == nullptr)
	{
		m_LightningChargeCache = objManager->AddGameObjectArg<LightningCharge>(OBJECT::PARTICLE,false);
	}
	m_Enable = false;
}

void LightningFall::Attack()
{
	//ƒˆ‰¼‘zŠÖ”‚Ìˆ×‰¼ŽÀ‘•
}

void LightningFall::Spawn(const XMFLOAT3& shotPos, const float& damage, const float& balletTime)
{
	if (m_Enable || m_FiledCache == nullptr) return;

	XMFLOAT3 pos = shotPos;
	pos.y = m_FiledCache->GetHeight(shotPos);

	EnemyAttackObject::Spawn(pos, damage, balletTime);

	if (m_BoxCollCache != nullptr)
	{
		m_BoxCollCache->SetCollisionInfo(pos, m_Scale, { 0.0f,0.0f,0.0f }, { 2.0f,2.0f,2.0f }, GetRotationMatrix());
	}

	m_LightningChargeCache->SetPos(pos);
	m_LightningChargeCache->Start(balletTime);
}
