#pragma once
#include "taskNode.h"

// Å‰‚É¬Œ÷‚µ‚½ƒm[ƒh‚ÅI—¹
class SelectorNode: public BehaviorNode 
{
protected:
    
public:
    virtual ~SelectorNode()override;
    virtual void Init()override;
    virtual NODE_STATUS Update(const float& deltaTime) override;
};