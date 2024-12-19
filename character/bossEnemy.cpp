#include "bossEnemy.h"
#include "behaviorTree/behaviorTree.h"
// TODO:�폜�\�� / �f�o�b�O�p
#include "manager/inputManager.h"
// ----------------------- public -----------------------
void BossEnemy::MoveControl(const float& deltaTime)
{
	if (m_Tree != nullptr)
	{
		m_Tree->Update(deltaTime);
	}

	// TODO :�폜�\�� / �f�o�b�O�p
	if (InputManager::GetKeyPress('0'))
	{
		TakeDamage(2000);
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

void BossEnemy::RotToTarget(GameObject* obj)
{
	if (obj == nullptr) return;
	const XMFLOAT3& enemyPos = GetPos();
	const XMFLOAT3& targetPos = obj->GetPos();

	// TODO :�C���\�� / ��]�����R��
	m_Rotation.y = atan2f(targetPos.x - enemyPos.x, targetPos.z - enemyPos.z);
}

