#include "selectorNode .h"

SelectorNode::~SelectorNode()
{
    for (TaskNode* child : m_Children)
    {
        delete child;
        child = nullptr;
    }
}
