#include "gameObject.h"
#include "renderer/renderer.h"
#include "component/component.h"
#include "component/boxCollisionComponent.h"

// --------------------- private ---------------------

void GameObject::AddBoxCollisionComponent(const COLLISION_TAG& tag)
{
	BoxCollisionComponent* boxCollision = new BoxCollisionComponent(this);
	if (boxCollision == nullptr) return;

	boxCollision->Init();
	boxCollision->SetCollisionTag(tag);

	CollisionData* colliData = new CollisionData;
	if (colliData == nullptr) return;

	colliData->BoxCollisions = boxCollision;

	m_BoxCollisions.emplace_back(colliData);
}

void GameObject::UpdateBoxCollisionInfo()
{
	// ‰½‚à’Ç‰Á‚µ‚È‚¢Žž‚Ì‰Šú’l
	for (CollisionData* colliData : m_BoxCollisions)
	{
		if (colliData == nullptr) continue;
		BoxCollisionComponent* boxCollision = colliData->BoxCollisions;
		if (boxCollision == nullptr) continue;

		boxCollision->SetBoxCollisionInfo(colliData->ColliPosition, colliData->ColliScale, m_ModelCenter, m_ModelScale, colliData->GetColliRotationMatrix());
	}
}

// --------------------- public ---------------------

GameObject::~GameObject()
{
	for (CollisionData* colliData : m_BoxCollisions)
	{
		BoxCollisionComponent* boxCollision = colliData->BoxCollisions;

		delete boxCollision;
		boxCollision = nullptr;

		delete colliData;
		colliData = nullptr;
	}
}

void GameObject::Init()
{
	for (Component* component : m_Components)
	{
		if (component == nullptr) continue;

		component->Init();
	}
}

void GameObject::Uninit()
{
	for (Component* component : m_Components)
	{
		if (component == nullptr) continue;

		component->Uninit();
	}
	for (CollisionData* colliData : m_BoxCollisions)
	{
		BoxCollisionComponent* boxCollision = colliData->BoxCollisions;
		if (boxCollision == nullptr) continue;

		boxCollision->Uninit();
	}
}

void GameObject::Update(const float& deltaTime)
{
	for (CollisionData* colliData : m_BoxCollisions)
	{
		BoxCollisionComponent* boxCollision = colliData->BoxCollisions;
		if (boxCollision == nullptr) continue;

		boxCollision->Update();
	}
	for (Component* component : m_Components)
	{
		if (component == nullptr) continue;

		component->Update();
	}
}

void GameObject::Draw()
{
	// ˆê”ÔÅ‰‚É•`‰æ
#if _DEBUG
	UpdateBoxCollisionInfo();
	for (CollisionData* colliData : m_BoxCollisions)
	{
		BoxCollisionComponent* boxCollision = colliData->BoxCollisions;
		if (boxCollision == nullptr) continue;

		boxCollision->Draw();
	}
#endif // _DEBUG
	for (Component* component : m_Components)
	{
		if (component == nullptr) continue;

		component->Draw();
	}
}


