#pragma once
#include "behaviorNode/taskNode.h"
// ---------------------------------------------------------
// タスクの説明：プレイヤーに向かって走る
// 備考：SUCCESSの設定が無いので、最初に置くと無限に歩き続ける
// ---------------------------------------------------------
class DashAtThePlayerTask final: public TaskNode
{
private:
	float m_Range = 0.0f;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;

	void SetRange(const float& range)
	{
		m_Range = range;
	}
};