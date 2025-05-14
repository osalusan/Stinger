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
	if (m_Enable)
	{
		for (Component* component : m_Components)
		{
			if (component == nullptr) continue;

			component->Update();
		}
	}
}

void GameObject::Draw()
{
	if (m_Enable)
	{
		for (Component* component : m_Components)
		{
			if (component == nullptr) continue;

			component->Draw();
		}
	}
}

XMFLOAT3 GameObject::GetForward()const
{
	XMMATRIX rotationMatrix;
	rotationMatrix = XMMatrixRotationRollPitchYaw(
		m_Rotation.x, m_Rotation.y, m_Rotation.z);

	XMFLOAT3 forward;
	XMStoreFloat3(&forward, rotationMatrix.r[2]);
	return forward;
}

XMFLOAT3 GameObject::GetRight() const
{
	XMMATRIX rotationMatrix;
	rotationMatrix = XMMatrixRotationRollPitchYaw(
		m_Rotation.x, m_Rotation.y, m_Rotation.z);

	XMFLOAT3 right;
	XMStoreFloat3(&right, rotationMatrix.r[0]);
	return right;
}

XMFLOAT3 GameObject::GetUp() const
{
	XMMATRIX rotationMatrix;
	rotationMatrix = XMMatrixRotationRollPitchYaw(
		m_Rotation.x, m_Rotation.y, m_Rotation.z);

	XMFLOAT3 up;
	XMStoreFloat3(&up, rotationMatrix.r[1]);
	return up;
}

XMMATRIX GameObject::GetRotationMatrix() const
{
	XMMATRIX rotationMatrix;
	rotationMatrix = XMMatrixRotationRollPitchYaw(
		m_Rotation.x, m_Rotation.y, m_Rotation.z);

	return rotationMatrix;
}

XMFLOAT3 GameObject::GetTargetDirection(const XMFLOAT3& targetpos) const
{
	XMVECTOR vector = XMVector3Normalize(XMVectorSubtract(XMLoadFloat3(&targetpos), XMLoadFloat3(&m_Position)));

	XMFLOAT3 direction;
	XMStoreFloat3(&direction, vector);
	return direction;
}


