#include "lightningFallEffect.h"
#include "manager/textureManager.h"

LightningFallEffect::LightningFallEffect(const GameObject* followObj)
	:BillBoard(XMFLOAT3(0.0f, 0.0f, 5.0f), XMFLOAT3(3.0f, 5.0f, 3.0f), TEXTURE::LIGHTNING_FALL, L"asset\\texture\\billboard\\lightningFall.png", 0.05f, XMINT2(6, 1))
{
	m_FollowObject = followObj;
	m_Enable = true;
	m_Loop = true;
	m_LoopStart = 0;
	m_LoopEnd = 6;
}

void LightningFallEffect::Update(const float& deltaTime)
{
	BillBoard::Update(deltaTime);
	if (m_FollowObject == nullptr) return;

	const XMFLOAT3& followObjPos = m_FollowObject->GetPos();
	const XMFLOAT3& followObjScale = m_FollowObject->GetScale();

	m_Position.x = followObjPos.x;
	m_Position.z = followObjPos.z;
	m_Position.y = followObjPos.y + (followObjScale.y * 0.5f);
}
