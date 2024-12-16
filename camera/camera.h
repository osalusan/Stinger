#pragma once
#include "main/main.h"

class Camera
{
protected:
	XMFLOAT3 m_Position = {};
	XMFLOAT3 m_Rotation = {};
	XMFLOAT3 m_Target = {};
	float m_Length = 20.0f;
	XMFLOAT4X4 m_ViewMatrix = {};

public:
	virtual ~Camera() {};
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw();
	
	void SetTarget(const XMFLOAT3& position)
	{
		m_Target = position;
	}

	XMMATRIX GetViewMatrix() 
	{
		return XMLoadFloat4x4(&m_ViewMatrix);
	}

	const XMFLOAT3& GetPos()const
	{
		return m_Position;
	}

	//�O���x�N�g���̎擾
	XMFLOAT3 GetForward()
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 forward;
		XMStoreFloat3(&forward, rotationMatrix.r[2]);
		return forward;
	}
	//�E�����x�N�g���̎擾
	XMFLOAT3 GetRight()
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 right;
		XMStoreFloat3(&right, rotationMatrix.r[0]);
		return right;
	}

	//������x�N�g���̎擾
	XMFLOAT3 GetUp()
	{
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw(
			m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMFLOAT3 up;
		XMStoreFloat3(&up, rotationMatrix.r[1]);
		return up;
	}
};