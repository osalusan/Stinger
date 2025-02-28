#pragma once
#include "behaviorNode/taskNode.h"

// ‘O•ûéŒ¾
class LightningFall;

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