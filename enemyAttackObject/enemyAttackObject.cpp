#include "enemyAttackObject.h"
#include "component/boxCollisionComponent.h"

// ---------------------------- protected ----------------------------
bool EnemyAttackObject::CollisionControl()
{
	if (m_BoxCollCache == nullptr) return false;

	if (m_IsAttack && m_BoxCollCache->CheckHitObject(COLLISION_TAG::PLAYER))
	{
		Player* player = m_BoxCollCache->GetHitGameObject<Player>();
		if (player == nullptr) return false;

		player->TakeDamage(m_Damage);

		Finish();

		return true;
	}
	return false;
}

// ---------------------------- public ----------------------------
EnemyAttackObject::EnemyAttackObject(const GameObject* target)
{
	m_Enable = false;
	m_BoxCollCache = AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::ENEMY_ATTACK);
	m_TargetObject = target;
}

void EnemyAttackObject::Update(const float& deltaTime)
{
	GameObject::Update(deltaTime);

	AttackControl(deltaTime);

	if (m_BoxCollCache != nullptr)
	{
		m_BoxCollCache->SetCollisionInfo(m_Position, m_Scale, { 0.0f,0.0f,0.0f }, { 2.0f,2.0f,2.0f }, GetRotationMatrix());
	}

	CollisionControl();
}

void EnemyAttackObject::Spawn(const XMFLOAT3& shotPos, const float& damage)
{
	if (m_Enable) return;

	m_Position = shotPos;
	m_Enable = true;
	m_IsAttack = true;
	m_Damage = damage;
	if (m_BoxCollCache != nullptr)
	{
		m_BoxCollCache->SetCollisionInfo(m_Position, m_Scale, { 0.0f,0.0f,0.0f }, { 2.0f,2.0f,2.0f }, GetRotationMatrix());
	}
}

void EnemyAttackObject::KeepPos(const XMFLOAT3& shotPos)
{
	m_Position = shotPos;

	if (m_BoxCollCache != nullptr)
	{
		m_BoxCollCache->SetCollisionInfo(m_Position, m_Scale, { 0.0f,0.0f,0.0f }, { 2.0f,2.0f,2.0f }, GetRotationMatrix());
	}
}


