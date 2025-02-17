#pragma once
#include "behaviorNode/taskNode.h"

// ‘O•ûéŒ¾
class LightningFall;

class LightningBattlecryTask final : public TaskNode
{
private:
	std::vector<LightningFall*> m_LightningFallCache = {};
	int m_UseLingtning = 0;
public:
	using TaskNode::TaskNode;
	~LightningBattlecryTask()override;
	virtual void Init()override;
	virtual NODE_STATE Update(const float& deltaTime) override;
};