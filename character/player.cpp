#include "player.h"
#include "manager/fbxModelManager.h"
#include "manager/inputManager.h"
#include "manager/sceneManager.h"
#include "scene/gameScene.h"
#include "component/boxCollisionComponent.h"
#include "camera/playerCamera.h"
#include "playerState/playerStateMachine.h"

constexpr XMFLOAT3 PLAYER_DEFAULT_POS = { 0.03f,0.03f,0.03f };
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
	m_Scale = PLAYER_DEFAULT_POS;
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

		// Y‚¾‚¯+
		m_Velocity.y += m_PlayerStateMachine->GetVelocity().y;
	}
}

void Player::CollisionControl()
{
	if (m_BoxCollision == nullptr) return;

	UpdateBoxCollisionInfo();

	if (m_BoxCollision->CheckHitCollision(COLLISION_TAG::OBJECT))
	{

	}

	float groundHeight = 0.0f;

	if (m_Position.y <= groundHeight)
	{
		m_Position.y = 0.0f;
		m_Velocity.y = 0.0f;

		// ’n–Ê‚ÉG‚ê‚Ä‚¢‚é‚©‚Ç‚¤‚©‚ð“`‚¦‚é
		if (m_PlayerStateMachine != nullptr)
		{
			m_PlayerStateMachine->HitGround();
		}
	}


}
