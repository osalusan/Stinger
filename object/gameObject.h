#pragma once
#include "main/main.h"
#include <string>

// 前方宣言
class BoxCollisionComponent;
class Component;
enum class COLLISION_TAG;

class GameObject 
{
protected:
	XMFLOAT3 m_Position = {};
	XMFLOAT3 m_Scale = { 1.0f, 1.0f, 1.0f };
	XMFLOAT3 m_Rotation = {};

	bool m_Enable = true;		// 有効、無効

	std::vector<Component*> m_Components = {};

public:
	virtual ~GameObject();
	virtual void Init();
	virtual void Uninit();
	virtual void Update(const float& deltaTime);
	virtual void Draw();

	template <typename T, typename... Arg>
	T* AddComponent(Arg&&...args)
	{
		T* component = new T(std::forward<Arg>(args)...);
		if (component == nullptr) return nullptr;

		component->Init();
		m_Components.emplace_back(component);

		return component;
	}

	template <typename T>
	bool GetComponents(std::vector<T*>& components)
	{
		for (Component* component : m_Components)
		{
			if (component == nullptr) continue;

			if (T* tComponent = dynamic_cast<T*>(component))
			{
				components.emplace_back(tComponent);
			}
		}
		return !components.empty();
	}

	void SetEnable(const float& flag)
	{
		m_Enable = flag;
	}

	const bool& GetEnable()const
	{
		return m_Enable;
	}
	const XMFLOAT3& GetPos()const
	{
		return m_Position;
	}
	const XMFLOAT3& GetScale()const
	{
		return m_Scale;
	}
	const XMFLOAT3& GetRot()const
	{
		return m_Rotation;
	}

	//前方ベクトルの取得
	XMFLOAT3 GetForward()const
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 forward;
		XMStoreFloat3(&forward, rotationMatrix.r[2]);
		return forward;
	}
	//右方向ベクトルの取得
	XMFLOAT3 GetRight()const
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 right;
		XMStoreFloat3(&right, rotationMatrix.r[0]);
		return right;
	}

	//上方向ベクトルの取得
	XMFLOAT3 GetUp()const
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 up;
		XMStoreFloat3(&up, rotationMatrix.r[1]);
		return up;
	}

	// 回転マトリックスを取得
	XMMATRIX GetRotationMatrix()const
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rotation.x, m_Rotation.y, m_Rotation.z);

		return rotationMatrix;
	}

	// ターゲットへのベクトルを取得
	XMFLOAT3 GetTargetDirection(const XMFLOAT3& targetpos)const 
	{
		XMVECTOR vector = XMVector3Normalize(XMVectorSubtract(XMLoadFloat3(&targetpos),XMLoadFloat3(&m_Position)));

		XMFLOAT3 direction;
		XMStoreFloat3(&direction, vector);
		return direction;
	}

};