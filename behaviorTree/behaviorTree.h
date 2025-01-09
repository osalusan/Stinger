#pragma once

// 前方宣言
class BehaviorNode;
class BossEnemy;
class BehaviorTree
{
protected:
    BehaviorNode* m_Root = nullptr;

    // ビヘイビアツリーの配置の最後に置く
    void CreateRoot(BehaviorNode* root);
    void ClearNodeState(BehaviorNode* root);
public:
    ~BehaviorTree();
    virtual void CreateTree(BossEnemy* boss) = 0;

    void Update(const float& deltaTime);

    // 変更不可
    const BehaviorNode* GetRootNode()const
    {
        return m_Root;
    }
};