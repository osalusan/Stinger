#pragma once
#include "taskNode.h"

// ‚·‚×‚Ä‚Ìqƒm[ƒh‚ª¬Œ÷‚µ‚½ê‡‚É¬Œ÷
class SequenceNode: public BehaviorNode 
{
public:
    virtual ~SequenceNode();
    virtual void Init()override;
    virtual NODE_STATUS Update(const float& deltaTime) override;
};