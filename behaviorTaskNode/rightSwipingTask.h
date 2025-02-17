#pragma once
#include "behaviorNode/taskNode.h"

class RightSwipingTask final : public TaskNode
{
private:
	virtual void InitTask(const float& deltaTime)override;
	virtual void RunningTask(const float& deltaTime)override;

public:
	using TaskNode::TaskNode;
	virtual void Init()override;
};