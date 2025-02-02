#pragma once
#include "billboard.h"

class LightningBallEffect :public BillBoard
{
private:
	const GameObject* m_FollowObject = nullptr;
public:
	LightningBallEffect() = delete;
	LightningBallEffect(const GameObject* followObj);
	virtual void Update(const float& deltaTime)override;
};