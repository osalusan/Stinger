#pragma once
#include "behaviorNode/taskNode.h"
// ---------------------------------------------------------
// タスクの説明：中距離で使用したい攻撃タスクを管理
// 備考：派生条件や派生確率を使用するために作成
// ---------------------------------------------------------
class MiddleRangeAttackTask final : public TaskNode
{
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;
};