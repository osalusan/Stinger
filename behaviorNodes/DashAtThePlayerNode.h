#pragma once
#include "behaviorTree/behaviorNode.h"
// �v���C���[��_���ă_�b�V��

// �O���錾
class Player;

class DashAtThePlayerNode final: public BehaviorNode
{
private:
	Player* m_PlayerCache = nullptr;
public:
	virtual void Init()override;
	virtual NODE_STATUS Update(const float& deltaTime) override;
};