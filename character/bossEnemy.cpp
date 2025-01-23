#include "bossEnemy.h"
#include "behaviorTree/behaviorTree.h"
#include <fstream>
#include <iostream>
#include <sstream>
// TODO :�������O���ɍ폜�\�� / �f�o�b�O�p
#include "manager/inputManager.h"
// ----------------------- protected -----------------------
void BossEnemy::MoveControl(const float& deltaTime)
{
	// �_���[�W�v�Z�p�Ƀ��Z�b�g
	m_CurrentAttackParts = ATTACK_PARTS::NONE;
	m_AttackDamage = 0.0f;
	m_ParryPossibleAtk = false;

	if (m_Tree != nullptr)
	{
		m_Tree->Update(deltaTime);
	}

	// TODO :�폜�\�� / �f�o�b�O�p
	if (InputManager::GetKeyPress('0'))
	{
		TakeDamage(2000);
	}
	if (InputManager::GetKeyPress('9'))
	{
		TakeDamage(1);
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

// BossEnemy�̃R���X�g���N�^���Ă΂�؂����ザ��Ȃ��ƁATask��dynamic_cast���������Ȃ�
void BossEnemy::CreateTree()
{
	if (m_Tree != nullptr)
	{
		m_Tree->CreateTree(this);
	}
}

// �G�̊�{�f�[�^�̓ǂݍ��� / �r�w�C�r�A�c���[�̍쐬����ɌĂяo��
void BossEnemy::EnemyDataLoadCSV(const std::string& filePath)
{
	std::ifstream ifs(filePath);
	if (!ifs) return;

	std::string line;
	int loadLine = 0;
	int loadCount = 0;
	int loadCellCnt = 0;
	std::string skillName = {};

	std::vector<std::string> baseStatas = {};
	std::vector<std::string> skillOptionNameData = {};

	// 1�s���ǂݍ���
	while (std::getline(ifs, line))
	{
		loadLine++;

		// 1�s�ڂ͓ǂݔ�΂�
		if (loadLine == 1) continue;

		loadCount++;
		loadCellCnt = 0;

		// ��s�̓X�L�b�v
		if (line.empty()) continue;

		// �J���}�ŋ�؂�
		std::stringstream ss(line);
		std::string column;
		std::vector<float> skillData = {};

		while (std::getline(ss, column, ','))
		{
			if (column.empty()) continue;

			loadCellCnt++;

			if (loadCount == 2)
			{
				if (loadCellCnt == 1)
				{
					skillName = column;
				}
				else if(loadCellCnt >= 2)
				{
					skillOptionNameData.emplace_back(column);
				}
				continue;
			}

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
			if (skillData.size() != 0)
			{
				int loop = 0;
				for (const std::string& skillOptionName : skillOptionNameData)
				{
					m_EnemySkillData[skillName].emplace(skillOptionName, skillData[loop]);
					loop++;
				}
			}
		}
		if (loadCount == 1)
		{
			skillOptionNameData.clear();
		}	
		else if (loadCount >= 2)
		{
			loadCount = 0;
		}
	}

	// �X�e�[�^�X�̗v�f��ǉ������������Aif���̐��l���ύX
	if (baseStatas.size() == 11)
	{
		m_MaxHealth = std::stof(baseStatas[0]);
		m_MaxStamina = std::stof(baseStatas[1]);
		m_MoveSpeed = std::stof(baseStatas[2]);
		m_GravityValue = std::stof(baseStatas[3]);
		m_ShortRange = std::stof(baseStatas[4]);
		m_MiddleRange = std::stof(baseStatas[5]);
		m_LongRange = std::stof(baseStatas[6]);
		m_Attack = std::stof(baseStatas[7]);
		m_RotSpeed = std::stof(baseStatas[8]);
		m_MinWaitTime = std::stof(baseStatas[9]);
		m_MaxWaitTime = std::stof(baseStatas[10]);

		m_Health = m_MaxHealth;
		m_StaminaValue = m_MaxStamina;
	}
}

// ----------------------- public -----------------------

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

void BossEnemy::TakeDamage(const float& atk)
{
	if (atk <= 0 || m_Health <= 0) return;

	m_Health -= atk;

	if (m_Health <= 0)
	{
		m_Health = 0;
	}
}

void BossEnemy::RotToTarget(GameObject* obj,const float& deltaTime)
{
	if (obj == nullptr) return;
	const XMFLOAT3& enemyPos = m_Position;
	const XMFLOAT3& targetPos = obj->GetPos();

	float currentAngle = m_Rotation.y;
	const float& targetAngle = atan2f(targetPos.x - enemyPos.x, targetPos.z - enemyPos.z);

	float angleDiff = targetAngle - currentAngle;
	while (angleDiff > XM_PI)
	{
		angleDiff -= XM_2PI;
	}
	while (angleDiff < -XM_PI)
	{
		angleDiff += XM_2PI;
	}

	// ���������𖄂߂�
	currentAngle += angleDiff * m_RotSpeed * deltaTime;

	m_Rotation.y = currentAngle;
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

void BossEnemy::SetRunningNode(BehaviorNode* node)
{
	m_RunningNodeCache = node;
	if (node == nullptr)
	{
		m_AttackAccept = false;
	}
}

