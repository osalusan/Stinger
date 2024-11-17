#pragma once
#include "behaviorNode.h"

// �ŏ��ɐ��������m�[�h�ŏI��
class SelectorNode : public BehaviorNode 
{
private:
    std::vector<BehaviorNode*> m_Children = {};
public:
    virtual ~SelectorNode();
    // TODO :�q�N���X�𑝂₵�Ă����Ȃ��`�ɂ���̂�������final������
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