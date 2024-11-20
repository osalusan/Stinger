#pragma once
#include "behaviorTree/taskNode.h"
// プレイヤーを狙ってダッシュ

class DashAtThePlayerTask final: public TaskNode
{
private:
	float m_Range = 0.0f;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATUS Update(const float& deltaTime) override;

	void SetRange(const float& range)
	{
		m_Range = range;
	}
};