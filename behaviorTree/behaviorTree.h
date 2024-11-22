#pragma once
#include "behaviorNode.h"

class BehaviorTree
{
protected:
    BehaviorNode* m_Root = nullptr;

public:
    ~BehaviorTree();
    // ���[�g���쐬
    void CreateRoot(BehaviorNode* root);
    void Update(const float& deltaTime);
};