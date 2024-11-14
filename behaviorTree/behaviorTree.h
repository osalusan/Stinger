#pragma once
#include "behaviorNode.h"

class BehaviourTree
{
private:
     BehaviorNode* m_Root = nullptr;
public:
    BehaviourTree(BehaviorNode* rootNode);
    void Update();
};