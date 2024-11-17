#pragma once
#include "behaviorNode.h"

class BehaviourTree
{
protected:
    BehaviorNode* m_Root = nullptr;

public:
    // ���[�g���쐬
    void CreateRoot(BehaviorNode* root);
    void Update(const float& deltaTime);
};