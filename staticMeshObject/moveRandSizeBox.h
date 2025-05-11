#pragma once
#include "staticMeshObject.h"

class MoveRandSizeBox final: public StaticMeshObject
{
private:
	float m_Speed = 0.0f;
	float m_ResetPosZ = 0.0f;
	float m_UpDownRange = 0.0f;
	float m_DefaultPosZ = 0.0f;
	float m_RandCenterPosY = 0.0f;

	XMFLOAT3 m_RotValue = {};

	bool m_Up = false;

	virtual void MoveControl(const float& deltaTime)override;
public:
	MoveRandSizeBox();
	MoveRandSizeBox(const float& resetPosZ, const float& posZ);
};