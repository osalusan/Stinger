#pragma once
#include "behaviorNode.h"

class BehaviorTree
{
protected:
    BehaviorNode* m_Root = nullptr;

public:
    ~BehaviorTree();
    // ƒ‹[ƒg‚ğì¬
    void CreateRoot(BehaviorNode* root);
    void Update(const float& deltaTime);
};