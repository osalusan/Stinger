#pragma once
#include "behaviorNode/taskNode.h"

// ‘O•ûéŒ¾
class LightningBall;

class AirLightningBallTask final : public TaskNode
{
private:
	LightningBall* m_LightningBallCache = nullptr;
	bool m_Attack = false;

	virtual void InitTask(const float& deltaTime)override;
	virtual void RunningTask(const float& deltaTime)override;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
};