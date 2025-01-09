#include "sequenceNode.h"

SequenceNode::~SequenceNode()
{
    for (BehaviorNode* child : m_Children)
    {
        delete child;
        child = nullptr;
    }
}


void SequenceNode::Init()
{

}

NODE_STATE SequenceNode::Update(const float& deltaTime)
{
    for (BehaviorNode* child : m_Children)
    {
        if (child == nullptr) continue;

        NODE_STATE state = child->Update(deltaTime);
        child->SetCurrentState(state);
        m_CurrentState = state;

        if (state != NODE_STATE::SUCCESS)
        {
            return state;
        }
    }
    m_CurrentState = NODE_STATE::SUCCESS;
    return NODE_STATE::SUCCESS;
}