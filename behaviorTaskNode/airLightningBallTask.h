#pragma once
#include "behaviorNode/taskNode.h"

// �O���錾
class LightningBall;

class AirLightningBallTask final : public TaskNode
{
private:
	LightningBall* m_LightningBallCache = nullptr;
	bool m_SpawnBall = false;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;
};