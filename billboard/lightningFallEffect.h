#pragma once
#include "billboard.h"
// 落雷のビルボードエフェクト
class LightningFallEffect :public BillBoard
{
private:
	const GameObject* m_FollowObject = nullptr;
	float m_TotalTime = 0.0f;
public:
	LightningFallEffect() = delete;
	LightningFallEffect(const GameObject* followObj);
	virtual void Update(const float& deltaTime)override;
	void Attack();
	virtual void End()override;
	bool Finish();
};