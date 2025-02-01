#include "moveRandSizeBox.h"
#include "manager/objModelManager.h"
#include "component/boxCollisionComponent.h"
#include "component/shaderComponent.h"

constexpr int RNAD_RANGE = 50;
constexpr float SPEED_VALUE = 0.02f;
constexpr float UP_DOWN_SPEED_VALUE = 0.027f;
constexpr float ROTATE_VALUE = 0.001f;

// -------------------------- private --------------------------
void MoveRandSizeBox::MoveControl(const float& deltaTime)
{
	if (m_Position.z < m_ResetPosZ)
	{
		m_Position.x = static_cast<float>(GetRandom() % (RNAD_RANGE / 2));
		m_Position.z = m_DefaultPosZ;
	}

	// ˆÚ“®§Œä
	m_Position.z -= m_Speed;

	const float& upDownSpeed = (m_Speed * m_UpDownRange) * UP_DOWN_SPEED_VALUE;
	if (m_Up)
	{
		m_Position.y += upDownSpeed;
	}
	else
	{
		m_Position.y -= upDownSpeed;
	}

	const float& max = m_RandCenterPosY + m_UpDownRange;
	const float& min = m_RandCenterPosY - m_UpDownRange;

	if (m_Position.y >= max)
	{
		m_Up = false;
	}
	else if (m_Position.y <= min)
	{
		m_Up = true;
	}

	// ‰ñ“]§Œä
	m_Rotation.x += m_RotValue.x;
	m_Rotation.y += m_RotValue.y;
	m_Rotation.z += m_RotValue.z;

	m_Color.w = 0.01f;
}

// -------------------------- public --------------------------
MoveRandSizeBox::MoveRandSizeBox() : StaticMeshObject(STATICMESH_MODEL::BOX_CENTER)
{
	ObjModelManager::ReservModel(m_Model, "asset\\model\\object\\boxCenter.obj");
	AddComponent<ShaderComponent>(this, "cso\\unlitColorModelVS.cso", "cso\\unlitColorModelPS.cso");

	m_Speed = static_cast<float>(rand() % RNAD_RANGE) * SPEED_VALUE;
	m_Position.x = static_cast<float>(GetRandom() % (RNAD_RANGE / 2));
	m_UpDownRange = static_cast<float>(rand() % (RNAD_RANGE/5));
	m_RandCenterPosY = static_cast<float>(rand() % (RNAD_RANGE / 4)) + 5.0f;
	m_RotValue.x = static_cast<float>(GetRandom() % RNAD_RANGE) * ROTATE_VALUE;
	m_RotValue.y = static_cast<float>(GetRandom() % RNAD_RANGE) * ROTATE_VALUE;
	m_RotValue.z = static_cast<float>(GetRandom() % RNAD_RANGE) * ROTATE_VALUE;

	m_Position.y = m_RandCenterPosY;
}

MoveRandSizeBox::MoveRandSizeBox(const float& resetPosZ,const float& posZ)
	:MoveRandSizeBox()
{
	m_ResetPosZ = resetPosZ;
	m_DefaultPosZ = posZ;
}
