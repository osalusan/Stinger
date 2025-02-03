#include "lightningBallEffect.h"
#include "manager/textureManager.h"


LightningBallEffect::LightningBallEffect(const GameObject* followObj)
	:BillBoard(XMFLOAT3(0.0f, 0.0f, 5.0f), XMFLOAT3(5.0f, 5.0f, 5.0f), TEXTURE::LIGHTNING_BALL, L"asset\\texture\\billboard\\lightningBall.png", 0.05f, XMINT2(5, 4))
{
	m_FollowObject = followObj;
	m_Enable = false;
	m_Loop = true;
	m_LoopStart = 5;
	m_LoopEnd = 10;
	m_AnimCount = m_LoopStart;
}

void LightningBallEffect::Update(const float& deltaTime)
{
	BillBoard::Update(deltaTime);
	if (m_FollowObject == nullptr) return;

	const XMFLOAT3& followObjPos = m_FollowObject->GetPos();
	const XMFLOAT3& followObjScale = m_FollowObject->GetScale();

	m_Position.x = followObjPos.x;
	m_Position.z = followObjPos.z;
	m_Position.y = followObjPos.y + (followObjScale.y * 0.5f);
}
