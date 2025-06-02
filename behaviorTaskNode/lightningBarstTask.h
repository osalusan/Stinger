#pragma once
#include "behaviorNode/taskNode.h"

// 前方宣言
class LightningFall;
// ---------------------------------------------------------
// タスクの説明：体の周囲に規則的な雷を3回に分けて放つ
// 備考：雷の生成タイミングや数はcppで管理
// ---------------------------------------------------------
class LightningBarstTask final : public TaskNode
{
private:
	std::vector<LightningFall*> m_LightningFallCache = {};
	int m_UseLingtning = 0;

	virtual void InitTask(const float& deltaTime)override;
	virtual void RunningTask(const float& deltaTime)override;
public:
	using TaskNode::TaskNode;
	~LightningBarstTask()override;
	virtual void Init()override;
};