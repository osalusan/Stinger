#include "selectorNode .h"

SelectorNode::~SelectorNode()
{
    for (TaskNode* child : m_Children)
    {
        delete child;
        child = nullptr;
    }
}

void SelectorNode::Init()
{

}

NODE_STATUS SelectorNode::Update(const float& deltaTime)
{
    for (TaskNode* child : m_Children)
    {
        if (child == nullptr) continue;

        NODE_STATUS status = child->Update(deltaTime);
        if (status != NODE_STATUS::SUCCESS)
        {
            return status;
        }
    }
    return NODE_STATUS::SUCCESS;
}
