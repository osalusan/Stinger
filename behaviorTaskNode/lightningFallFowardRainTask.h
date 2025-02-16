#pragma once
#include "behaviorNode/taskNode.h"

// ‘O•ûéŒ¾
class LightningFall;

class LightningFallFowardRainTask final : public TaskNode
{
private:
	std::vector<LightningFall*> m_LightningFallCache = {};
	bool m_SpawnLingtning = false;
public:
	~LightningFallFowardRainTask();
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;
};