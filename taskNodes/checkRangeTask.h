#pragma once
#include "behaviorTree/taskNode.h"
// ”ÍˆÍ“à‚©‚Ç‚¤‚©”»’f

class CheckRangeTask final : public TaskNode
{
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATUS Update(const float& deltaTime) override;

};