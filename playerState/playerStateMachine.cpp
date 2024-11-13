#include "playerStateMachine.h"
#include "manager/inputManager.h"
#include "manager/sceneManager.h"
#include "camera/playerCamera.h"
#include "scene/gameScene.h"
#include "playerState.h"
#include "playerStateIdle.h"
#include "playerStateRun.h"
#include "playerStateJump.h"

PlayerStateMachine::~PlayerStateMachine()
{
	for (auto& PlayerStatePool : m_PlayerStatePool)
	{
		delete PlayerStatePool.second;
		PlayerStatePool.second = nullptr;
	}
	m_PlayerStatePool.clear();
}

void PlayerStateMachine::Init()
{
	if (m_PlayerStatePool.empty())
	{
		// 要素上限分リハッシュ
		m_PlayerStatePool.reserve(static_cast<int>(PLAYER_STATE::MAX) - 1);

		m_PlayerStatePool.emplace(PLAYER_STATE::IDLE, new PlayerStateIdle(this));
		m_PlayerStatePool.emplace(PLAYER_STATE::RUN, new PlayerStateRun(this));
		m_PlayerStatePool.emplace(PLAYER_STATE::JUMP, new PlayerStateJump(this));
	}
	// 初期化
	for (const std::pair<PLAYER_STATE, PlayerState*>& PlayerState : m_PlayerStatePool)
	{
		m_PlayerStatePool[PlayerState.first]->Init();
	}

	if (m_CurrentPlayerState == nullptr)
	{
		m_CurrentPlayerState = m_PlayerStatePool[PLAYER_STATE::IDLE];
	}
}

void PlayerStateMachine::Uninit()
{
	for (const auto& PlayerStatePool : m_PlayerStatePool)
	{
		// nullチェック
		if (PlayerStatePool.second != nullptr)
		{
			PlayerStatePool.second->Unit();
		}
	}
}
// 主に移動の処理
void PlayerStateMachine::Update(const float& deltaTime)
{
	if (m_CameraCache == nullptr)
	{
		GameScene* scene = SceneManager::GetScene<GameScene>();
		if (scene == nullptr) return;
		m_CameraCache = scene->GetCamera();

		if (m_CameraCache == nullptr) return;
	}

	// 入力の前にリセット
	m_RandL = MOVE_DIRECTION::NONE;
	m_FandB = MOVE_DIRECTION::NONE;
	m_IsJamp = false;

	if (InputManager::GetKeyPress('A'))
	{
		m_RandL = MOVE_DIRECTION::LEFT;

	}
	if (InputManager::GetKeyPress('D'))
	{
		m_RandL = MOVE_DIRECTION::RIGHT;
	}
	if (InputManager::GetKeyPress('W'))
	{
		m_FandB = MOVE_DIRECTION::FORWARD;
	}
	if (InputManager::GetKeyPress('S'))
	{
		m_FandB = MOVE_DIRECTION::BACKWARD;
	}

	//ジャンプ
	if (InputManager::GetKeyPress(VK_SPACE))
	{
		m_IsJamp = true;
		m_Velocity.y = 5.0f;
	}

	// 入力処理の後に
	if (m_CurrentPlayerState != nullptr)
	{
		m_CurrentPlayerState->Update(deltaTime);
		m_CurrentPlayerState->ChangeStateControl();
	}


	// 最後にリセット
	m_IsGround = false;
}

void PlayerStateMachine::Draw()
{
	// TODO : プレイヤーのアニメーション管理
}

void PlayerStateMachine::SetPlayerState(const PLAYER_STATE& state)
{
	// 同じステートだったらはじく
	if (m_CurrentPlayerState == m_PlayerStatePool[state]) return;

	// 終了処理
	if (m_CurrentPlayerState != nullptr)
	{
		m_CurrentPlayerState->Unit();
	}

	// nullチェック
	if (m_PlayerStatePool[state] != nullptr)
	{
		m_CurrentPlayerState = m_PlayerStatePool[state];
	}

	// 初期化
	if (m_CurrentPlayerState != nullptr)
	{
		m_CurrentPlayerState->Init();
	}
}

void PlayerStateMachine::InitVelocity()
{
	m_Velocity = {};
}

XMFLOAT3 PlayerStateMachine::GetCameraForward() const
{
	if (m_CameraCache != nullptr)
	{
		return m_CameraCache->GetForward();
	}
	return XMFLOAT3(0.0f,0.0f,0.0f);
}

XMFLOAT3 PlayerStateMachine::GetCameraRight() const
{
	if (m_CameraCache != nullptr)
	{
		return m_CameraCache->GetRight();
	}
	return XMFLOAT3(0.0f, 0.0f, 0.0f);
}
