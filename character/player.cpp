#include "player.h"
#include "manager/fbxModelManager.h"
#include "manager/inputManager.h"
#include "manager/sceneManager.h"
#include "scene/gameScene.h"
#include "component/boxCollisionComponent.h"
#include "camera/playerCamera.h"
#include "playerState/playerStateMachine.h"

constexpr XMFLOAT3 DEFAULT_SCALE_PLAYER = { 0.03f,0.03f,0.03f };
constexpr float GRAVITY = 2200.0f;

constexpr int DEFAULT_HEALTH_PLAYER = 10;

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
	m_Scale = DEFAULT_SCALE_PLAYER;
	m_EnableGravity = true;
	m_GravityValue = GRAVITY;
	m_Health = DEFAULT_HEALTH_PLAYER;

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

// ------------------------------- private -------------------------------
void Player::MoveControl(const float& deltaTime)
{
	if (m_CameraCache == nullptr)
	{
		GameScene* scene = SceneManager::GetScene<GameScene>();
		if (scene == nullptr) return;
		ObjectManager* objManager = scene->GetObjectManager();
		if (objManager == nullptr) return;
		m_CameraCache = objManager->GetCamera();

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

		// ˆÊ’u‚ðMTV•ª‚¾‚¯ˆÚ“®
		XMVECTOR playerVectorPos = XMLoadFloat3(&m_Position);
		playerVectorPos = XMVectorAdd(playerVectorPos, mtv);
		XMStoreFloat3(&m_Position, playerVectorPos);

		// Šù‚É‰Ÿ‚µo‚µ‚Ä‚¢‚é‚©‚ç’n–Ê‚Ì‚‚³‚Å•â³‚µ‚È‚­‚Ä‚¢‚¢
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

		// ’n–Ê‚ÉG‚ê‚Ä‚¢‚é‚©‚Ç‚¤‚©‚ð“`‚¦‚é
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
