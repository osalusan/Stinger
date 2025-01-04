#include "bossEnemy.h"
#include "behaviorTree/behaviorTree.h"
#include <fstream>
#include <iostream>
#include <sstream>
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
	m_EnemySkillData.clear();

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

bool BossEnemy::UseStamina(const float& use)
{
	if (m_StaminaValue - use < 0.0f)
	{
		return false;
	}

	m_StaminaValue -= use;

	return true;
}

// �G�̊�{�f�[�^�̓ǂݍ��� / �r�w�C�r�A�c���[�̍쐬����ɌĂяo��
void BossEnemy::EnemyDataLoadCSV(const std::string& filePath)
{
	std::ifstream ifs(filePath);
	if (!ifs) return;

	std::string line;
	int loadLine = 0;
	int loadCount = 0;

	std::vector<std::string> baseStatas = {};

	// 1�s���ǂݍ���
	while (std::getline(ifs, line))
	{
		loadLine++;
		
		// 1�s�ڂ͓ǂݔ�΂�
		if (loadLine == 1) continue;

		loadCount++;
		// �K�v�Ȃ�����e��
		if (loadCount >= 2)
		{
			loadCount = 0;
			continue;
		}

		// ��s�̓X�L�b�v
		if (line.empty()) continue;

		// �J���}�ŋ�؂�
		std::stringstream ss(line);
		std::string column;
		std::vector<float> skillData = {};

		while (std::getline(ss, column, ','))
		{
			if (column.empty()) continue;

			// ��b�X�e�[�^�X
			if (loadLine == 2)
			{
				baseStatas.emplace_back(column);
			}
			else // �X�L�����i�[
			{
				skillData.emplace_back(std::stof(column));
			}
		}

		if (loadLine >= 3)
		{
			m_EnemySkillData.emplace_back(skillData);
		}
	}

	// TODO :�ǉ��\�� / �U����
	if (baseStatas.size() == 8)
	{
		m_MaxHealth = std::stoi(baseStatas[0]);
		m_MaxStamina = std::stof(baseStatas[1]);
		m_MoveSpeed = std::stof(baseStatas[2]);
		m_GravityValue = std::stof(baseStatas[3]);
		m_ShortRange = std::stof(baseStatas[4]);
		m_MiddleRange = std::stof(baseStatas[5]);
		m_LongRange = std::stof(baseStatas[6]);

		m_Health = m_MaxHealth;
		m_StaminaValue = m_MaxStamina;
	}
}