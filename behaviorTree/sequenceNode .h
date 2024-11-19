#pragma once
#include "behaviorNode.h"

// ‚·‚×‚Ä‚Ìqƒm[ƒh‚ª¬Œ÷‚µ‚½ê‡‚É¬Œ÷
class SequenceNode final: public BehaviorNode 
{
private:
    std::vector<BehaviorNode*> m_Children = {};
public:
    ~SequenceNode();
    virtual void Init()override;
    virtual NODE_STATUS Update(const float& deltaTime) override;

    template <typename T>
    void AddChild(T child)
    {
        BehaviorNode* node = new T;
        if (node == nullptr) return;
        node->Init();
        m_Children.emplace_back(child);
    }   
};