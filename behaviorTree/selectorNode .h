#pragma once
#include "behaviorNode.h"

// Å‰‚É¬Œ÷‚µ‚½ƒm[ƒh‚ÅI—¹
class SelectorNode : public BehaviorNode 
{
private:
    std::vector<BehaviorNode*> m_Children = {};
public:
    virtual ~SelectorNode();
    virtual void Init()override;
    virtual NODE_STATUS Update() override;

    template <typename T>
    void AddChild(T child)
    {
        BehaviorNode* node = new T;
        if (node == nullptr) return;
        node->Init();
        m_Children.emplace_back(child);
    }
};