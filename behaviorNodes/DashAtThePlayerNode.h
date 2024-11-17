#pragma once
#include "behaviorTree/behaviorNode.h"
// プレイヤーを狙ってダッシュ

class DashAtThePlayerNode : public BehaviorNode
{
public:
	virtual void Init()override final;
	virtual NODE_STATUS Update(const float& deltaTime) override final;
};