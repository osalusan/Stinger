#pragma once
#include "behaviorTree/taskNode.h"
// �v���C���[��_���ă_�b�V��

// �O���錾
class Player;

class DashAtThePlayerNode final: public TaskNode
{
private:
	Player* m_PlayerCache = nullptr;
public:
	using TaskNode::TaskNode;
	virtual void Init()override;
	virtual NODE_STATUS Update(const float& deltaTime) override;
};