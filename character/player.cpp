#include "player.h"
#include "manager/fbxModelManager.h"
#include "manager/inputManager.h"
#include "manager/sceneManager.h"
#include "renderer/fbxModelRenderer.h"
#include "component/boxCollisionComponent.h"
#include "component/shaderComponent.h"
#include "camera/playerCamera.h"
#include "playerState/playerStateMachine.h"
#include <fstream>
#include <iostream>
#include <sstream>

constexpr XMFLOAT3 DEFAULT_SCALE_PLAYER = { 0.03f,0.03f,0.03f };

Player::Player(const XMFLOAT3& pos)
{
	ReservModel(ANIMETION_MODEL::PLAYER, "asset\\model\\player\\akai.fbx");
	m_Position = pos;
}

Player::~Player()
{
	delete m_PlayerStateMachine;
	m_PlayerStateMachine = nullptr;
}

void Player::Init()
{
	Character::Init();
	// ��ԍŏ��ɌĂ�
	PlayerDataLoadCSV("asset\\csv\\player.csv");

	m_Scale = DEFAULT_SCALE_PLAYER;
	m_EnableGravity = true;

	m_BoxCollCache = AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::PLAYER);
	// �����蔻��̌�ɒǉ�
	AddComponent<ShaderComponent>(this, "cso\\skinningVS.cso", "cso\\skinningPS.cso");

	if (m_PlayerStateMachine == nullptr)
	{
		m_PlayerStateMachine = new PlayerStateMachine(this);
	}
	if (m_PlayerStateMachine != nullptr)
	{
		m_PlayerStateMachine->Init();
	}
}

void Player::Uninit()
{
	if (m_PlayerStateMachine != nullptr)
	{
		m_PlayerStateMachine->Uninit();
	}
	Character::Uninit();
}

void Player::TakeDamage(const float& atk)
{
	if (atk <= 0 || m_Health <= 0) return;

	m_Health -= atk;

	if (m_PlayerStateMachine != nullptr)
	{
		m_PlayerStateMachine->SetIsHitDamage(true);
	}

	if (m_Health <= 0)
	{
		m_Health = 0;
	}
}

// ------------------------------- private -------------------------------
void Player::MoveControl(const float& deltaTime)
{
	if (m_PlayerStateMachine != nullptr)
	{
		m_PlayerStateMachine->Update(deltaTime);
	}
}

void Player::CustomCollisionInfo()
{
	const XMFLOAT3& customScale = { m_Scale.x * 0.4f ,m_Scale.y ,m_Scale.z * 1.0f };
	if (m_BoxCollCache == nullptr) return;

	if (const FbxModelRenderer* model = FbxModelManager::GetAnimationModel(m_Model))
	{
		m_BoxCollCache->SetCollisionInfo(m_Position, customScale, model->GetCenter(), model->GetScale(), GetRotationMatrix());
	}
}

void Player::CollisionControl()
{
	if (m_PlayerStateMachine == nullptr) return;

	if (m_BoxCollCache == nullptr) return;

	const int& maxCount = 5;
	int count = 0;
	XMFLOAT3 recPos = {};
	while(m_BoxCollCache->CheckHitAllObject())
	{
		count++;
		if (m_BoxCollCache->GetHitGameObject<GameObject>() == nullptr) break;

		XMVECTOR mtv = m_BoxCollCache->GetMtv();

		// �ʒu��MTV�������ړ�
		XMVECTOR playerVectorPos = XMLoadFloat3(&m_Position);
		playerVectorPos = XMVectorAdd(playerVectorPos, mtv);
		XMStoreFloat3(&m_Position, playerVectorPos);

		// �p�̖ڂ荞�݃o�O�h�~
		std::vector<GameObject*> objects = {};
		m_BoxCollCache->GetHitGameObjects(objects);
		if (objects.size() >= 2)
		{
			if (m_Position.x == recPos.x && m_Position.z == recPos.z)
			{
				m_Position.x = m_RecordPosition.x;
				m_Position.z = m_RecordPosition.z;
				count = maxCount;
			}
			else
			{
				recPos = m_Position;
			}
		}

		m_BoxCollCache->SetPos(m_Position);

		// ���ɉ����o���Ă��邩��n�ʂ̍����ŕ␳���Ȃ��Ă���
		if (mtv.m128_f32[1] > 0.0f)
		{
			m_Velocity.y = 0.0f;
			m_PlayerStateMachine->HitGround();
		}
		if (count >= maxCount) break;	// �������[�v���Ȃ��悤��
	}

	CheckWorldWallPos();

	float groundHeight = 0.0f;

	if (m_Position.y <= groundHeight)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;

		// �n�ʂɐG��Ă��邩�ǂ�����`����
		m_PlayerStateMachine->HitGround();
	}
}

void Player::AnimationControl()
{
	if (m_PlayerStateMachine != nullptr)
	{
		ChangeAnimation(m_PlayerStateMachine->GetAnimation());
	}
}

// �G�̊�{�f�[�^�̓ǂݍ��� / �r�w�C�r�A�c���[�̍쐬����ɌĂяo��
void Player::PlayerDataLoadCSV(const std::string& filePath)
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

		// ��s�̓X�L�b�v
		if (line.empty()) continue;

		// �J���}�ŋ�؂�
		std::stringstream ss(line);
		std::string column;
		std::vector<float> skillData = {};

		while (std::getline(ss, column, ','))
		{
			if (column.empty()) continue;

			// ��b�X�e�[�^�X�i�[
			if (loadLine == 2)
			{
				baseStatas.emplace_back(column);
			}

			if (loadLine >= 4) break;
		}
	}

	// �X�e�[�^�X�̗v�f��ǉ������������Aif���̐��l���ύX
	if (baseStatas.size() == 7)
	{
		m_MaxHealth = std::stof(baseStatas[0]);
		m_Attack = std::stof(baseStatas[1]);
		m_MoveSpeed = std::stof(baseStatas[2]);
		m_GravityValue = std::stof(baseStatas[3]);
		m_RotSpeed = std::stof(baseStatas[4]);
		m_MinParryTime = std::stof(baseStatas[5]);
		m_MaxParryTime = std::stof(baseStatas[6]);

		m_Health = m_MaxHealth;
	}
}
