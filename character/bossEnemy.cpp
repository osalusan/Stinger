#include "bossEnemy.h"
#include "behaviorTree/behaviorTree.h"
#include <fstream>
#include <iostream>
#include <sstream>
// TODO:削除予定 / デバッグ用
#include "manager/inputManager.h"
// ----------------------- public -----------------------
void BossEnemy::MoveControl(const float& deltaTime)
{
	if (m_Tree != nullptr)
	{
		m_Tree->Update(deltaTime);
	}

	// TODO :削除予定 / デバッグ用
	if (InputManager::GetKeyPress('0'))
	{
		TakeDamage(2000);
	}
}

void BossEnemy::ParameterControl(const float& deltaTime)
{
	Character::ParameterControl(deltaTime);
	m_StaminaValue += deltaTime;

	if (m_StaminaValue > m_MaxStamina)
	{
		m_StaminaValue = m_MaxStamina;
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

	// BossEnemyのコンストラクタが呼ばれ切った後じゃないと、Taskのdynamic_castが反応しない
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

	// TODO :修正予定 / 回転を自然に
	m_Rotation.y = atan2f(targetPos.x - enemyPos.x, targetPos.z - enemyPos.z);
}

bool BossEnemy::UseStamina(const float& use)
{
	if (m_StaminaValue - use < 0.0f)
	{
		return false;
	}

	m_StaminaValue -= use;

	return true;
}

// 敵の基本データの読み込み
void BossEnemy::EnemyDataLoadCSV(const std::string& filePath)
{
	std::ifstream ifs(filePath);
	if (!ifs) return;

	std::string line;
	int noLoad = 0;

	// 1行ずつ読み込み
	while (std::getline(ifs, line))
	{
		// 1行目は読み飛ばす
		if (noLoad < 1)
		{
			noLoad++;
			continue;
		}

		// 空行はスキップ
		if (line.empty()) continue;

		// カンマで区切る
		std::stringstream ss(line);
		std::string column;
		std::vector<std::string> tokens;

		while (std::getline(ss, column, ','))
		{
			tokens.emplace_back(column);
		}

		m_MaxHealth = std::stoi(tokens[0]);
	}
}