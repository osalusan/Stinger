#pragma once
// TODO:デバッグ用削除予定
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