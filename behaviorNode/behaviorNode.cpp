#include "behaviorNode.h"

BehaviorNode::~BehaviorNode()
{
	for (BehaviorNode* child : m_Children)
	{
		delete child;
		child = nullptr;
	}
	m_Children.clear();
}
