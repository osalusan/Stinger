#include "behaviorTree/selectorNode .h"

class MoveMawJLaygoSelector final: public SelectorNode
{
private:
    float m_CurrentTime = 0.0f;

public:
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