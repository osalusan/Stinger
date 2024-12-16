#pragma once
#include "taskNode.h"

// ç≈èâÇ…ê¨å˜ÇµÇΩÉmÅ[ÉhÇ≈èIóπ
class SelectorNode: public BehaviorNode 
{
protected:
    std::vector<TaskNode*> m_Children = {};
public:
    virtual ~SelectorNode()override;
    virtual void Init()override;
    virtual NODE_STATUS Update(const float& deltaTime) override;

    template <typename T, typename... Arg>
    void AddTaskChild(Arg&&...args)
    {
        TaskNode* node = new T(std::forward<Arg>(args)...);
        if (node == nullptr) return;
        node->Init();
        m_Children.emplace_back(node);
    }
    template <typename T>
    void AddNodeChild()
    {
        BehaviorNode* node = new T;
        if (node == nullptr) return;
        node->Init();
        m_Children.emplace_back(node);
    }
};