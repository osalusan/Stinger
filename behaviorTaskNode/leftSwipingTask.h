#pragma once
#include "behaviorNode/taskNode.h"
// ---------------------------------------------------------
// タスクの説明：左手を振りかぶって攻撃
// 備考：攻撃判定は左肩～左手の左全体
// ---------------------------------------------------------
class LeftSwipingTask final : public TaskNode
{
private:
	virtual void InitTask(const float& deltaTime)override;
	virtual void RunningTask(const float& deltaTime)override;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
};