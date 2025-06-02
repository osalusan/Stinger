#pragma once
#include "behaviorNode/taskNode.h"

// 前方宣言
class LightningBall;

// ---------------------------------------------------------
// タスクの説明：体の正面に雷の弾を発射する
// 備考：空中で使うと、宙にとどまる
// ---------------------------------------------------------
class AirLightningBallTask final : public TaskNode
{
private:
	LightningBall* m_LightningBallCache = nullptr;
	bool m_Attack = false;

	virtual void InitTask(const float& deltaTime)override;
	virtual void RunningTask(const float& deltaTime)override;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
};