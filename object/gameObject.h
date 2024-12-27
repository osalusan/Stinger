#pragma once
#include "main/main.h"
#include <string>

// �O���錾
class BoxCollisionComponent;
class Component;
enum class COLLISION_TAG;

// �����蔻��p
struct CollisionData
{
	BoxCollisionComponent* BoxCollisions = nullptr;
	XMFLOAT3 ColliPosition = {};
	XMFLOAT3 ColliRotation = {};
	XMFLOAT3 ColliScale = { 1.0f, 1.0f, 1.0f };

	//�����蔻��̉�]�}�g���b�N�X���擾
	XMMATRIX GetColliRotationMatrix()const
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			ColliRotation.x, ColliRotation.y, ColliRotation.z);

		return rotationMatrix;
	}
};

class GameObject 
{
protected:
	XMFLOAT3 m_Position = {};
	XMFLOAT3 m_Scale = { 1.0f, 1.0f, 1.0f };
	XMFLOAT3 m_Rotation = {};

	// Model�֌W
	XMFLOAT3 m_ModelCenter = {};
	XMFLOAT3 m_ModelScale = {};

	bool m_Enable = true;		// �L���A����

	std::vector<Component*> m_Components = {};
	std::vector<CollisionData*> m_BoxCollisions = {};

	// �R���W������ǉ����������ɌĂ�
	void AddBoxCollisionComponent(const COLLISION_TAG& tag);
	// Update�ɏ����Ȃ���ModelCenter�Ȃǂ��擾�ł��Ȃ�
	void UpdateBoxCollisionInfo();

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

	void SetEnable(const float& flag)
	{
		m_Enable = flag;
	}

	std::vector<CollisionData*> GetBoxCollisions()
	{
		return m_BoxCollisions;
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

	const XMFLOAT3& GetModelCenter()const
	{
		return m_ModelCenter;
	}
	const XMFLOAT3& GetModelScale()const
	{
		return m_ModelScale;
	}

	//�O���x�N�g���̎擾
	XMFLOAT3 GetForward()const
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 forward;
		XMStoreFloat3(&forward, rotationMatrix.r[2]);
		return forward;
	}
	//�E�����x�N�g���̎擾
	XMFLOAT3 GetRight()const
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 right;
		XMStoreFloat3(&right, rotationMatrix.r[0]);
		return right;
	}

	//������x�N�g���̎擾
	XMFLOAT3 GetUp()const
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 up;
		XMStoreFloat3(&up, rotationMatrix.r[1]);
		return up;
	}

	// ��]�}�g���b�N�X���擾
	XMMATRIX GetRotationMatrix()const
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rotation.x, m_Rotation.y, m_Rotation.z);

		return rotationMatrix;
	}

	// �^�[�Q�b�g�ւ̃x�N�g�����擾
	XMFLOAT3 GetTargetDirection(const XMFLOAT3& targetpos) {
		
		XMVECTOR vector = XMVector3Normalize(XMVectorSubtract(XMLoadFloat3(&targetpos),XMLoadFloat3(&m_Position)));

		XMFLOAT3 direction;
		XMStoreFloat3(&direction, vector);
		return direction;
	}

};