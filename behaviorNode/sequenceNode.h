#pragma once
#include "taskNode.h"

// ���ׂĂ̎q�m�[�h�����������ꍇ�ɐ���
class SequenceNode: public BehaviorNode 
{
public:
    virtual ~SequenceNode();
    virtual void Init()override;
    virtual NODE_STATUS Update(const float& deltaTime) override;
};