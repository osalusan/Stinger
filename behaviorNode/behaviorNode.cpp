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

bool BehaviorNode::CheckRunningNode(BehaviorNode* currentNode)
{
	if (currentNode == this || currentNode == nullptr)
	{
		return true;
	}

	for (BehaviorNode* child : m_Children)
	{
		if (child == nullptr) continue;

		if (child->CheckRunningNode(currentNode))
		{
			return true;
		}
	}
	return false;
}

int BehaviorNode::GetTotalDerivChance() const
{
	int totalValue = 0;

	for (int childValue : m_ChildDerivChance)
	{
		totalValue += childValue;
	}

	return totalValue;
}
