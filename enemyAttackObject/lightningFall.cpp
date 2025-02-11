#include "lightningFall.h"
#include "manager/objModelManager.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "component/boxCollisionComponent.h"
#include "scene/scene.h"
#include "billboard/lightningFallEffect.h"

constexpr XMFLOAT3 DEFALUT_SCALE = { 1.0f,15.0f,1.0f };

// -------------------------------------- private --------------------------------------

void LightningFall::AttackControl(const float& deltaTime)
{
	//ƒˆ‰¼‘zŠÖ”‚Ìˆ×‰¼ŽÀ‘•
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
	m_Enable = true;
}

void LightningFall::Attack()
{
	//ƒˆ‰¼‘zŠÖ”‚Ìˆ×‰¼ŽÀ‘•
}

void LightningFall::Spawn(const XMFLOAT3& shotPos, const float& damage)
{
	EnemyAttackObject::Spawn(shotPos, damage);

	if (m_Enable)
	{
		if (m_LightningFallEffCache == nullptr) return;
		m_LightningFallEffCache->UseBillboard();
	}
}
