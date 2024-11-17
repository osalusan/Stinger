#include "bossEnemy.h"
#include "behaviorTree/behaviorTree.h"

// ----------------------- protected -----------------------
void BossEnemy::CreateTree(BehaviourTree* tree)
{
	if (m_Tree == nullptr && tree != nullptr)
	{
		m_Tree = tree;
	}
	if (m_Tree != nullptr)
	{
		m_Tree->Init();
	}
}

// ----------------------- public -----------------------

BossEnemy::~BossEnemy()
{
	delete m_Tree;
	m_Tree = nullptr;
}

