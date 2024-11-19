#include "taskNode.h"

TaskNode::TaskNode(BossEnemy* boss)
{
	if (m_Boss == nullptr && boss != nullptr)
	{
		m_Boss = boss;
	}
}
