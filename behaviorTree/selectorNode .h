#pragma once
#include "behaviorNode.h"

// Å‰‚É¬Œ÷‚µ‚½ƒm[ƒh‚ÅI—¹
class SelectorNode final: public BehaviorNode 
{
private:
    std::vector<BehaviorNode*> m_Children = {};
public:
    virtual ~SelectorNode()override;
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