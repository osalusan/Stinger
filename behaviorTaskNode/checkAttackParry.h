#pragma once
#include "behaviorNode/taskNode.h"

class CheckAttackParryTask final : public TaskNode
{
private:
	bool m_RunningChild = false;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;
};