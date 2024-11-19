#pragma once
#include "behaviorNode.h"

class BossEnemy;

class TaskNode : public BehaviorNode
{
private:
	BossEnemy* m_Boss = nullptr;

public:
	TaskNode() = delete;
	TaskNode(BossEnemy* boss);
	virtual void Init()override = 0;
	virtual NODE_STATUS Update(const float& deltaTime) override = 0;
};