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

BossEnemy::BossEnemy(BehaviorTree* tree,const XMFLOAT3& pos)
{
	if (m_Tree == nullptr)
	{
		m_Tree = tree;
	}

	m_Position = pos;
}

BossEnemy::~BossEnemy()
{
	delete m_Tree;
	m_Tree = nullptr;

	m_RunningNodeCache = nullptr;
}

void BossEnemy::Init()
{
	GameObject::Init();

	// BossEnemy�̃R���X�g���N�^���Ă΂�؂����ザ��Ȃ��ƁATask��dynamic_cast���������Ȃ�
	if (m_Tree != nullptr)
	{
		m_Tree->CreateTree(this);
	}
}

