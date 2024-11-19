#pragma once
#include "behaviorTree/taskNode.h"
// プレイヤーを狙ってダッシュ

class DashAtThePlayerNode final: public TaskNode
{
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATUS Update(const float& deltaTime) override;
};