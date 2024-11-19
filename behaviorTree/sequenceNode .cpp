#include "sequenceNode .h"

SequenceNode::~SequenceNode()
{
    for (TaskNode* child : m_Children)
    {
        delete child;
        child = nullptr;
    }
}
