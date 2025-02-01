#pragma once
#include "behaviorNode/taskNode.h"
#include "main/main.h"

class JumpAttackTask final : public TaskNode
{
private:
	XMFLOAT3 m_MoveVector = {};
	float m_Accel = 0.0f;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;
};