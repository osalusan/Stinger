#include "player.h"
#include "manager/fbxModelManager.h"
#include "manager/inputManager.h"
#include "manager/sceneManager.h"
#include "scene/gameScene.h"
#include "component/boxCollisionComponent.h"
#include "camera/playerCamera.h"
#include "playerState/playerStateMachine.h"

constexpr XMFLOAT3 PLAYER_DEFAULT_POS = { 0.03f,0.03f,0.03f };
constexpr float GRAVITY = 500.0f;

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
		m_Velocity = m_PlayerStateMachine->GetVelocity();
	}

	// TODO :デバッグ用仮処理 / PlayerState作成後に以下の処理を移動
	

	//ジャンプ
	if (InputManager::GetKeyPress(VK_SPACE))
	{
		m_Velocity.y = 5.0f;
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
	}
}
