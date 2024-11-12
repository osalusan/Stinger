#include "player.h"
#include "manager/fbxModelManager.h"
#include "manager/inputManager.h"
#include "manager/sceneManager.h"
#include "scene/gameScene.h"
#include "component/boxCollisionComponent.h"
#include "camera/playerCamera.h"

constexpr XMFLOAT3 PLAYER_DEFAULT_POS = { 0.03f,0.03f,0.03f };
constexpr float MOVE_SPEED = 2000.0f;

Player::Player()
{
	ReservModel(ANIMETION_MODEL::PLAYER, "asset\\model\\Akai.fbx");
}

void Player::Init()
{
	GameObject::Init();
	m_Scale = PLAYER_DEFAULT_POS;

	AddBoxCollisionComponent(COLLISION_TAG::PLAYER);
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

	m_Velocity.x = { 0.0f };
	m_Velocity.z = { 0.0f };
	const XMFLOAT3& forwardVector = m_CameraCache->GetForward();
	const XMFLOAT3& rightVector = m_CameraCache->GetRight();

	// 回転制御用変数
	float cRot = 6.28f / 4.0f;
	float rotation = 0.0f;

	if (InputManager::GetKeyPress('A'))
	{
		m_Velocity.x += (-rightVector.x);
		m_Velocity.z += (-rightVector.z);

	}
	if (InputManager::GetKeyPress('D'))
	{
		m_Velocity.x += rightVector.x;
		m_Velocity.z += rightVector.z;
	}
	if (InputManager::GetKeyPress('W'))
	{
		m_Velocity.z += forwardVector.z;
		m_Velocity.x += forwardVector.x;
	}
	if (InputManager::GetKeyPress('S'))
	{
		m_Velocity.z += (-forwardVector.z);
		m_Velocity.x += (-forwardVector.x);
	}

	if (InputManager::GetKeyRelease(VK_RBUTTON) || InputManager::GetKeyRelease(VK_LSHIFT))
	{
		
	}
	//ジャンプ
	if (InputManager::GetKeyPress(VK_SPACE))
	{
		//m_Velocity.y = 1.0f;
	}

	//斜めでも速度が変わらないように
	XMVECTOR velocityVec = XMLoadFloat3(&m_Velocity);
	XMVECTOR normalizedVelocityVec = XMVector3Normalize(velocityVec);
	XMFLOAT3 normalizedVelocity;
	XMStoreFloat3(&normalizedVelocity, normalizedVelocityVec);

	m_Velocity.x = normalizedVelocity.x * MOVE_SPEED * deltaTime;
	m_Velocity.z = normalizedVelocity.z * MOVE_SPEED * deltaTime;

	// 重力加速度
	m_Velocity.y += -2.5f * deltaTime;
}

void Player::CollisionControl()
{
	if (m_BoxCollision == nullptr) return;

	UpdateBoxCollisionInfo();

	if (m_BoxCollision->CheckHitCollision(COLLISION_TAG::OBJECT))
	{

	}
}
