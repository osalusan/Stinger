#pragma once
#include "behaviorNode.h"

class BehaviourTree
{
protected:
    BehaviorNode* m_Root = nullptr;

public:
    // ƒ‹[ƒg‚ğì¬
    void CreateRoot(BehaviorNode* root);
    void Update(const float& deltaTime);
};