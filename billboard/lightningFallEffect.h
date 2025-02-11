#pragma once
#include "billboard.h"

class LightningFallEffect :public BillBoard
{
private:
	const GameObject* m_FollowObject = nullptr;
public:
	LightningFallEffect() = delete;
	LightningFallEffect(const GameObject* followObj);
	virtual void Update(const float& deltaTime)override;
};