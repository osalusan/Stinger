#include "player.h"
#include "manager/fbxModelManager.h"
#include "manager/inputManager.h"
#include "manager/sceneManager.h"
#include "renderer/fbxModelRenderer.h"
#include "scene/gameScene.h"
#include "component/boxCollisionComponent.h"
#include "component/shaderComponent.h"
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

	m_BoxCollCache = AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::PLAYER);
	// “–‚½‚è”»’è‚ÌŒã‚É’Ç‰Á
	AddComponent<ShaderComponent>(this, "cso\\skinningVS.cso", "cso\\skinningPS.cso");

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

		m_Rotation = m_PlayerStateMachine->GetRotation();
	}
}

void Player::CustomCollisionInfo()
{
	const XMFLOAT3& customScale = { m_Scale.x * 0.2f ,m_Scale.y ,m_Scale.z * 0.2f };
	if (m_BoxCollCache == nullptr) return;

	if (const FbxModelRenderer* model = FbxModelManager::GetAnimationModel(m_Model))
	{
		const XMFLOAT3& customScale = { m_Scale.x * 0.2f ,m_Scale.y ,m_Scale.z * 0.2f };
		m_BoxCollCache->SetCollisionInfo(m_Position, customScale, model->GetCenter(), model->GetScale(), GetRotationMatrix());
	}
}

void Player::CollisionControl()
{
	if (m_PlayerStateMachine == nullptr) return;

	if (m_BoxCollCache == nullptr) return;

	if (m_BoxCollCache->CheckHitObject(OBJECT::STATICMESH))
	{
		XMVECTOR mtv = m_BoxCollCache->GetMtv();

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