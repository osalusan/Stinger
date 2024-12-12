#pragma once

#include "behaviorTree/taskNode.h"

class RoaringInfinityTask final : public TaskNode
{
private:
	float m_CurrentTime = 0.0f;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATUS Update(const float& deltaTime) override;

};