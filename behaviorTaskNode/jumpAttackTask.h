#pragma once
#include "behaviorNode/taskNode.h"
#include "main/main.h"

class JumpAttackTask final : public TaskNode
{
private:
	XMFLOAT3 m_MoveVector = {};
	float m_Accel = 0.0f;

	virtual void InitTask(const float& deltaTime)override;
	virtual void RunningTask(const float& deltaTime)override;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
};