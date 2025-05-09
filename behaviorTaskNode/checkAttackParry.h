#pragma once
#include "behaviorNode/taskNode.h"

// ---------------------------------------------------------
// タスクの説明：パリィされたか確認する為だけのタスク
// 備考：パリィを確認したら実行中のタスクを強制終了させる
// ---------------------------------------------------------
class CheckAttackParryTask final : public TaskNode
{
private:
	bool m_RunningChild = false;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;
};