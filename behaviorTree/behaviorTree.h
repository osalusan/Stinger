#pragma once
#include "behaviorNode/behaviorNode.h"

class BossEnemy;
class BehaviorTree
{
protected:
    BehaviorNode* m_Root = nullptr;

    // ビヘイビアツリーの配置の最後に置く
    void CreateRoot(BehaviorNode* root);
public:
    ~BehaviorTree();
    virtual void CreateTree(BossEnemy* boss) = 0;

    void Update(const float& deltaTime);
};