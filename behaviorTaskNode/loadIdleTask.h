#pragma once
#include "behaviorNode/taskNode.h"

class LoadIdleTask final : public TaskNode
{
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATUS Update(const float& deltaTime) override;
};