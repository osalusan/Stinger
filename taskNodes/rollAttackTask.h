#pragma once
// TODO:�f�o�b�O�p�폜�\��
#include "behaviorTree/taskNode.h"

class RollAttackTask final : public TaskNode
{
private:
	float m_CurrentTime = 0.0f;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATUS Update(const float& deltaTime) override;

};