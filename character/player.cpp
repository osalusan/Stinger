#include "player.h"
#include "manager/fbxModelManager.h"
#include "manager/inputManager.h"
#include "manager/sceneManager.h"
#include "scene/gameScene.h"
#include "component/boxCollisionComponent.h"
#include "camera/playerCamera.h"
#include "playerState/playerStateMachine.h"

constexpr XMFLOAT3 PLAYER_DEFAULT_SCALE = { 0.03f,0.03f,0.03f };
constexpr float GRAVITY = 2200.0f;

Player::Player()
{
	ReservModel(ANIMETION_MODEL::PLAYER, "asset\\model\\Akai.fbx");
}

Player::~Player()
{
	delete m_PlayerStateMachine;
	m_PlayerStateMachine = nullptr;
}

void Player::Init()
{
	GameObject::Init();
	m_Scale = PLAYER_DEFAULT_SCALE;
	m_EnableGravity = true;
	m_GravityValue = GRAVITY;

	AddBoxCollisionComponent(COLLISION_TAG::PLAYER);

	if (m_PlayerStateMachine == nullptr)
	{
		m_PlayerStateMachine = new PlayerStateMachine;
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

void Player::Draw()
{
	Character::Draw();
	if (m_PlayerStateMachine != nullptr)
	{
		m_PlayerStateMachine->Draw();
	}
}

// ------------------------------- private -------------------------------
void Player::MoveControl(const float& deltaTime)
{
	if (m_CameraCache == nullptr)
	{
		GameScene* scene = SceneManager::GetScene<GameScene>();
		if (scene == nullptr) return;
		m_CameraCache = scene->GetCamera();

		if (m_CameraCache == nullptr) return;
	}

	if (m_PlayerStateMachine != nullptr)
	{
		m_PlayerStateMachine->Update(deltaTime);
		m_Velocity.x = m_PlayerStateMachine->GetVelocity().x;
		m_Velocity.z = m_PlayerStateMachine->GetVelocity().z;

		// Y����+
		m_Velocity.y += m_PlayerStateMachine->GetVelocity().y;
	}
}

void Player::CustomCollisionInfo()
{
	m_ColliPosition = m_Position;
	m_ColliRotation = m_Rotation;
	m_ColliScale.x = m_Scale.x * 0.5f;
	m_ColliScale.y = m_Scale.y;
	m_ColliScale.z = m_Scale.z * 0.5f;
}

void Player::CollisionControl()
{
	if (m_BoxCollision == nullptr || m_PlayerStateMachine == nullptr) return;

	UpdateBoxCollisionInfo();

	if (m_BoxCollision->CheckHitCollision(COLLISION_TAG::OBJECT))
	{
		XMVECTOR mtv = m_BoxCollision->GetMtv();

		// �ʒu��MTV�������ړ�
		XMVECTOR playerVectorPos = XMLoadFloat3(&m_Position);
		playerVectorPos = XMVectorAdd(playerVectorPos, mtv);
		XMStoreFloat3(&m_Position, playerVectorPos);

		// ���ɉ����o���Ă��邩��|�W�V�����͕␳���Ȃ��Ă���
		if (mtv.m128_f32[1] > 0.0f)
		{
			m_Velocity.y = 0.0f;
			m_PlayerStateMachine->HitGround();
		}
	}

	float groundHeight = 0.0f;

	if (m_Position.y <= groundHeight)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;

		// �n�ʂɐG��Ă��邩�ǂ�����`����
		m_PlayerStateMachine->HitGround();
		
	}
}
