#pragma once
#include "behaviorNode/taskNode.h"
#include "main/main.h"

class JumpAttackTask final : public TaskNode
{
private:
	XMFLOAT3 m_MoveVector = {};
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATUS Update(const float& deltaTime) override;
};