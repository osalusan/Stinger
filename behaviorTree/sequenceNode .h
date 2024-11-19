#pragma once
#include "taskNode.h"

// ‚·‚×‚Ä‚Ìqƒm[ƒh‚ª¬Œ÷‚µ‚½ê‡‚É¬Œ÷
class SequenceNode final: public BehaviorNode 
{
private:
    std::vector<TaskNode*> m_Children = {};
public:
    ~SequenceNode();
    virtual void Init()override;
    virtual NODE_STATUS Update(const float& deltaTime) override;

    template <typename T, typename... Arg>
    void AddChild(Arg&&...args)
    {
        TaskNode* node = new T(std::forward<Arg>(args)...);
        if (node == nullptr) return;
        node->Init();
        m_Children.emplace_back(node);
    }
};