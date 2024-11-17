#pragma once
#include "behaviorNode.h"

// 最初に成功したノードで終了
class SelectorNode : public BehaviorNode 
{
private:
    std::vector<BehaviorNode*> m_Children = {};
public:
    virtual ~SelectorNode();
    // TODO :子クラスを増やしていかない形にするのだったらfinalをつける
    virtual void Init()override;
    virtual NODE_STATUS Update(const float& deltaTime) override;

    template <typename T>
    void AddChild()
    {
        BehaviorNode* node = new T;
        if (node == nullptr) return;
        node->Init();
        m_Children.emplace_back(node);
    }
};