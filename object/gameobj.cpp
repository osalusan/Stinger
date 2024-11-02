#include "gameobj.h"

void GameObject::GetForward(XMFLOAT3& forward)
{
	XMMATRIX rotationMatrix;
	rotationMatrix = XMMatrixRotationRollPitchYaw(
		m_Rotation.x, m_Rotation.y, m_Rotation.z);

	XMFLOAT3 forward;
	XMStoreFloat3(&forward, rotationMatrix.r[2]);
}

void GameObject::GetRight(XMFLOAT3& forward)
{
	XMMATRIX rotationMatrix;
	rotationMatrix = XMMatrixRotationRollPitchYaw(
		m_Rotation.x, m_Rotation.y, m_Rotation.z);

	XMFLOAT3 forward;
	XMStoreFloat3(&forward, rotationMatrix.r[0]);
}
