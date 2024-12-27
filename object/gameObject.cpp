#include "gameObject.h"
#include "renderer/renderer.h"
#include "component/component.h"
#include "component/boxCollisionComponent.h"

// --------------------- public ---------------------

GameObject::~GameObject()
{
	for (Component* component : m_Components)
	{
		delete component;
		component = nullptr;
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
}

void GameObject::Update(const float& deltaTime)
{
	for (Component* component : m_Components)
	{
		if (component == nullptr) continue;

		component->Update();
	}
}

void GameObject::Draw()
{
	for (Component* component : m_Components)
	{
		if (component == nullptr) continue;

		component->Draw();
	}
}


