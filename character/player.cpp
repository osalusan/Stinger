#include "player.h"
#include "manager/fbxModelManager.h"
#include "manager/inputManager.h"
#include "manager/sceneManager.h"
#include "scene/gameScene.h"
#include "component/boxCollisionComponent.h"
#include "camera/playerCamera.h"
#include "playerState/playerStateMachine.h"

constexpr XMFLOAT3 PLAYER_DEFAULT_POS = { 0.03f,0.03f,0.03f };
constexpr float MOVE_SPEED = 2000.0f;
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
	}

	// TODO :デバッグ用仮処理 / PlayerState作成後に以下の処理を移動

	m_Velocity.x = { 0.0f };
	m_Velocity.z = { 0.0f };
	const XMFLOAT3& forwardVector = m_CameraCache->GetForward();
	const XMFLOAT3& rightVector = m_CameraCache->GetRight();

	// 回転制御用変数
	float cRot = 6.28f / 4.0f;
	float rotation = 0.0f;

	XMFLOAT3 moveVector = {};

	if (InputManager::GetKeyPress('A'))
	{
		moveVector.x += (-rightVector.x);
		moveVector.z += (-rightVector.z);

	}
	if (InputManager::GetKeyPress('D'))
	{
		moveVector.x += rightVector.x;
		moveVector.z += rightVector.z;
	}
	if (InputManager::GetKeyPress('W'))
	{
		moveVector.x += forwardVector.x;
		moveVector.z += forwardVector.z;
	}
	if (InputManager::GetKeyPress('S'))
	{
		moveVector.x += (-forwardVector.x);
		moveVector.z += (-forwardVector.z);
	}

	if (InputManager::GetKeyRelease(VK_RBUTTON) || InputManager::GetKeyRelease(VK_LSHIFT))
	{
		
	}
	//ジャンプ
	if (InputManager::GetKeyPress(VK_SPACE))
	{
		m_Velocity.y = 5.0f;
	}

	//斜めでも速度が変わらないように
	XMVECTOR velocityVec = XMLoadFloat3(&moveVector);
	XMVECTOR normalizedVelocityVec = XMVector3Normalize(velocityVec);
	XMFLOAT3 normalizedVelocity = {};
	XMStoreFloat3(&normalizedVelocity, normalizedVelocityVec);

	m_Velocity.x = normalizedVelocity.x * MOVE_SPEED * deltaTime;
	m_Velocity.z = normalizedVelocity.z * MOVE_SPEED * deltaTime;
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
