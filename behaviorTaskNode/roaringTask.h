#pragma once
#include "behaviorNode/taskNode.h"

class RoaringTask final : public TaskNode
{
private:
	bool m_UseRendition = false;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;

};