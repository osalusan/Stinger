#pragma once
#include "behaviorNode/taskNode.h"
// ---------------------------------------------------------
// タスクの説明：一定時間待機
// 備考：現在のスタミナに応じて待機時間が決まる
// ---------------------------------------------------------
class WaitTask final : public TaskNode
{
private:
	bool m_Wait = false;
	float m_WaitValue = 0.0f;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;
};