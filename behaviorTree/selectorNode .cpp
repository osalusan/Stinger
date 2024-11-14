#include "selectorNode .h"

SelectorNode::~SelectorNode()
{
    for (BehaviorNode* child : m_Children)
    {
        delete child;
        child = nullptr;
    }
}

void SelectorNode::Init()
{

}

NODE_STATUS SelectorNode::Update()
{
    for (BehaviorNode* child : m_Children)
    {
        NODE_STATUS status = child->Update();
        if (status != NODE_STATUS::FAILURE)
        {
            return status;
        }
    }
    return NODE_STATUS::FAILURE;
}
