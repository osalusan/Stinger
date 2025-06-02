#pragma once
#include "behaviorNode/taskNode.h"
// ---------------------------------------------------------
// タスクの説明：敵の体力を判別する
// 備考：体力がどれくらいか判断し、生きているか死んでいるかを判別
// ---------------------------------------------------------
class CheckHealthTask final : public TaskNode
{
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;
};