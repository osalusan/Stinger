#pragma once
#include "taskNode.h"

// ���ׂĂ̎q�m�[�h�����������ꍇ�ɐ���
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