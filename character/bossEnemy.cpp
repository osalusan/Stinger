#include "bossEnemy.h"
#include "behaviorTree/behaviorTree.h"

// ----------------------- public -----------------------

void BossEnemy::MoveControl(const float& deltaTime)
{
	if (m_Tree != nullptr)
	{
		m_Tree->Update(deltaTime);
	}
}

BossEnemy::~BossEnemy()
{
	delete m_Tree;
	m_Tree = nullptr;
}

void BossEnemy::Init()
{
	GameObject::Init();

	if (m_Tree == nullptr)
	{
		m_Tree = new BehaviourTree;
	}

	CreateBehaviourTree();
}

