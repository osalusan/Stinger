#pragma once
#include "behaviorNode/taskNode.h"
// ---------------------------------------------------------
// タスクの説明：パリィ時のよろけ
// 備考：
// ---------------------------------------------------------
class ParryRecoilTask final : public TaskNode
{
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;
};
