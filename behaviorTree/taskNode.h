#pragma once
#include "behaviorNode.h"

class BossEnemy;
class Player;

class TaskNode : public BehaviorNode
{
private:
	BossEnemy* m_BossCache = nullptr;
	Player* m_PlayerCache = nullptr;
public:
	TaskNode() = delete;
	TaskNode(BossEnemy* boss, Player* player);
	virtual ~TaskNode()override;
	virtual void Init()override = 0;
	virtual NODE_STATUS Update(const float& deltaTime) override = 0;
};