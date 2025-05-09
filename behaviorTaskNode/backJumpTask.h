#pragma once
#include "behaviorNode/taskNode.h"

// ---------------------------------------------------------
// タスクの説明：ジャンプしながら後ろに下がる
// 備考：
// ---------------------------------------------------------
class BackJumpTask final : public TaskNode
{
private:
	virtual void RunningTask(const float& deltaTime)override;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
};