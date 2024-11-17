#pragma once
#include "behaviorNode.h"

class BehaviourTree
{
protected:
    BehaviorNode* m_Root = nullptr;

    // ���[�g���쐬
    void CreateRoot(BehaviorNode* root);
public:

    virtual void Init() = 0;
    void Update();
};