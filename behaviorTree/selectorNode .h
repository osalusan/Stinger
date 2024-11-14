#pragma once
#include "behaviorNode.h"

// �ŏ��ɐ��������m�[�h�ŏI��
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