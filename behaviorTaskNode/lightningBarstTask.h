#pragma once
#include "behaviorNode/taskNode.h"

// ‘O•ûéŒ¾
class LightningFall;

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