#pragma once
#include "taskNode.h"

// �ŏ��ɐ��������m�[�h�ŏI��
class SelectorNode: public BehaviorNode 
{
protected:
    
public:
    virtual ~SelectorNode()override;
    virtual void Init()override;
    virtual NODE_STATE Update(const float& deltaTime) override;
};