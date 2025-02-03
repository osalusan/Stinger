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

DERIVATION_DATA BehaviorNode::GetDerivationData(const int& num) const
{
	if (m_DerivationData.size() > num)
	{
		return m_DerivationData[num];
	}
	return DERIVATION_DATA();
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

int BehaviorNode::GetDerivChance(const int& num) const
{
	if (m_ChildDerivChance.size() > num)
	{
		return m_ChildDerivChance[num];
	}
	return -1;
}
