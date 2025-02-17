#pragma once
#include "behaviorNode/taskNode.h"

class RightPunchTask final : public TaskNode
{
private:
	virtual void RunningTask(const float& deltaTime)override;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
};
