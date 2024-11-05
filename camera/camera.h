#pragma once
#include "main/main.h"

class Camera
{
protected:
	XMFLOAT3 m_Position = {};
	XMFLOAT3 m_Rotation = {};
	XMFLOAT3 m_Target = {};
	float m_length = 20.0f;
	XMFLOAT4X4 m_ViewMatrix = {};

public:
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw();
	
	void SetTarget(const XMFLOAT3& position)
	{
		m_Target = position;
	}

	const XMMATRIX& GetViewMatrix() 
	{
		return XMLoadFloat4x4(&m_ViewMatrix);
	}

	const XMFLOAT3& GetPos()const
	{
		return m_Position;
	}
};