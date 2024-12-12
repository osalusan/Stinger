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

BossEnemy::BossEnemy(BehaviorTree* tree, const XMFLOAT3& pos, const XMFLOAT3& scale)
	:BossEnemy(tree,pos)
{
	m_Scale = scale;
}

BossEnemy::BossEnemy(BehaviorTree* tree, const XMFLOAT3& pos, const XMFLOAT3& scale, const XMFLOAT3& rot)
	:BossEnemy(tree,pos,scale)
{
	m_Rotation = rot;
}

BossEnemy::~BossEnemy()
{
	delete m_Tree;
	m_Tree = nullptr;

	m_RunningNodeCache = nullptr;
}

void BossEnemy::Init()
{
	Character::Init();

	// BossEnemy�̃R���X�g���N�^���Ă΂�؂����ザ��Ȃ��ƁATask��dynamic_cast���������Ȃ�
	if (m_Tree != nullptr)
	{
		m_Tree->CreateTree(this);
	}
}

