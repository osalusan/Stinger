#pragma once
#include "behaviorNode/taskNode.h"
// ---------------------------------------------------------
// タスクの説明：死亡タスク
// 備考：アニメーションの管理やスローなどの演出を管理
// ---------------------------------------------------------
class DeadTask final : public TaskNode
{
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;
};