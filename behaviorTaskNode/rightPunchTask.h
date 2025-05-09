#pragma once
#include "behaviorNode/taskNode.h"
// ---------------------------------------------------------
// タスクの説明：右フック
// 備考：右肩～右手までの右側が攻撃判定
// ---------------------------------------------------------
class RightPunchTask final : public TaskNode
{
private:
	virtual void RunningTask(const float& deltaTime)override;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
};
