#pragma once
#include "behaviorNode/behaviorNode.h"

class BossEnemy;
class BehaviorTree
{
protected:
    BehaviorNode* m_Root = nullptr;

    // �r�w�C�r�A�c���[�̔z�u�̍Ō�ɒu��
    void CreateRoot(BehaviorNode* root);
public:
    ~BehaviorTree();
    virtual void CreateTree(BossEnemy* boss) = 0;

    void Update(const float& deltaTime);
};