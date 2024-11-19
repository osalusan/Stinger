#include "taskNode.h"

TaskNode::TaskNode(BossEnemy* boss, Player* player)
{
	if (m_BossCache == nullptr && boss != nullptr)
	{
		m_BossCache = boss;
	}
	if (m_PlayerCache == nullptr && player != nullptr)
	{
		m_PlayerCache = player;
	}
}

TaskNode::~TaskNode()
{
	m_BossCache = nullptr;
	m_PlayerCache = nullptr;
}
