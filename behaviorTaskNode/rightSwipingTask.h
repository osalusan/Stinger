#pragma once
#include "behaviorNode/taskNode.h"
// ---------------------------------------------------------
// タスクの説明：右手を振りかぶって攻撃
// 備考：攻撃判定は右肩～右手の右全体
// ---------------------------------------------------------
class RightSwipingTask final : public TaskNode
{
private:
	virtual void InitTask(const float& deltaTime)override;
	virtual void RunningTask(const float& deltaTime)override;

public:
	using TaskNode::TaskNode;
	virtual void Init()override;
};