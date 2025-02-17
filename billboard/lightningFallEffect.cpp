#include "lightningFallEffect.h"
#include "manager/textureManager.h"

LightningFallEffect::LightningFallEffect(const GameObject* followObj)
	:BillBoard(XMFLOAT3(0.0f, 0.0f, 5.0f), XMFLOAT3(3.0f, 15.0f, 3.0f), TEXTURE::LIGHTNING_FALL, L"asset\\texture\\billboard\\lightningFall.png", 0.05f, XMINT2(6, 1))
{
	m_FollowObject = followObj;
	m_Enable = false;
	m_Loop = false;
	m_BillboardY = true;
}

void LightningFallEffect::Update(const float& deltaTime)
{
	BillBoard::Update(deltaTime);
	if (m_FollowObject == nullptr) return;

	const XMFLOAT3& followObjPos = m_FollowObject->GetPos();
	const XMFLOAT3& followObjScale = m_FollowObject->GetScale();

	m_Position.x = followObjPos.x;
	m_Position.z = followObjPos.z;
	m_Position.y = followObjPos.y + (followObjScale.y);
	m_TotalTime += deltaTime;
}

void LightningFallEffect::Attack()
{
	m_Enable = true;
	m_CurrentTime = 0.0f;
	m_AnimCount = 0;
	m_TotalTime = 0.0f;
}

void LightningFallEffect::End()
{
	BillBoard::End();
	m_TotalTime = 0.0f;
}

bool LightningFallEffect::Finish()
{
	if (m_TotalTime > m_MaxTime)
	{
		End();
		return true;
	}
	return false;
}
