#pragma once
#include "behaviorNode/taskNode.h"
// ---------------------------------------------------------
// タスクの説明：敵がプレイヤーからどれくらい離れているかを判定
// 備考：これを呼ばないと、距離に応じて変化する攻撃が使えない
// ---------------------------------------------------------
class CheckRangeTask final : public TaskNode
{
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;

};