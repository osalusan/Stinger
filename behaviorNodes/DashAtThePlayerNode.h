#pragma once
#include "behaviorTree/behaviorNode.h"
// プレイヤーを狙ってダッシュ

// 前方宣言
class Player;

class DashAtThePlayerNode final: public BehaviorNode
{
private:
	Player* m_PlayerCache = nullptr;
public:
	virtual void Init()override;
	virtual NODE_STATUS Update(const float& deltaTime) override;
};