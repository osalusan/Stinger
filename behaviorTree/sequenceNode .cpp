#include "sequenceNode .h"

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

NODE_STATUS SequenceNode::Update()
{
    for (BehaviorNode* child : m_Children)
    {
        NODE_STATUS status = child->Update();
        if (status != NODE_STATUS::SUCCESS)
        {
            return status;
        }
    }
    return NODE_STATUS::SUCCESS;
}
