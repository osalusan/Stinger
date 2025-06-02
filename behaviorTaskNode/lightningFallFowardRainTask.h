#pragma once
#include "behaviorNode/taskNode.h"

// 前方宣言
class LightningFall;
// ---------------------------------------------------------
// タスクの説明：体の前方に多くの雷を放つ
// 備考：雷の生成タイミングや数はcppで管理 / 名前がダサい
// ---------------------------------------------------------
class LightningFallFowardRainTask final : public TaskNode
{
private:
	std::vector<LightningFall*> m_LightningFallCache = {};
	bool m_SpawnLingtning = false;

	virtual void InitTask(const float& deltaTime)override;
	virtual void RunningTask(const float& deltaTime)override;
public:
	~LightningFallFowardRainTask();
	using TaskNode::TaskNode;
	virtual void Init()override;
};