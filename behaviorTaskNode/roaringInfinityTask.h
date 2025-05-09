#pragma once
#include "behaviorNode/taskNode.h"
// ---------------------------------------------------------
// タスクの説明：無限に咆哮を放つ
// 備考：SUCCESSが無い
// ---------------------------------------------------------
class RoaringInfinityTask final : public TaskNode
{
private:
	bool m_Up = false;
	float m_AnimeTime = 0.0f;
	float m_DerayTime = 0.0f;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;

};