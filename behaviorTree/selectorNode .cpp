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

NODE_STATUS SelectorNode::Update(const float& deltaTime)
{
    for (BehaviorNode* child : m_Children)
    {
        if (child == nullptr) continue;

        NODE_STATUS status = child->Update(deltaTime);
        if (status != NODE_STATUS::FAILURE)
        {
            return status;
        }
    }
    return NODE_STATUS::FAILURE;
}
