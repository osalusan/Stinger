#pragma once
#include "behaviorTree/behaviorNode.h"
// �v���C���[��_���ă_�b�V��

class DashAtThePlayerNode : public BehaviorNode
{
public:
	virtual void Init()override final;
	virtual NODE_STATUS Update(const float& deltaTime) override final;
};