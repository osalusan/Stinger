#include "playerStateMachine.h"
#include "manager/inputManager.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "camera/playerCamera.h"
#include "scene/gameScene.h"
#include "character/player.h"
#include "playerState.h"
#include "playerStateIdle.h"
#include "playerStateRun.h"
#include "playerStateJump.h"
#include "playerStateParryAttack.h"
#include "playerStateNormalAttack.h"
#include "playerState/playerStateRolling.h"

constexpr int EXTRATTACK_ACCEPT_PARRY_MAX = 3;

PlayerStateMachine::PlayerStateMachine(Player* player)
{
	if (player == nullptr || m_PlayerCache != nullptr) return;
	m_PlayerCache = player;
}

PlayerStateMachine::~PlayerStateMachine()
{
	for (auto& PlayerStatePool : m_PlayerStatePool)
	{
		delete PlayerStatePool.second;
		PlayerStatePool.second = nullptr;
	}
	m_PlayerStatePool.clear();
	m_PlayerCache = nullptr;
}

void PlayerStateMachine::Init()
{
	if (m_PlayerStatePool.empty())
	{
		m_ParryCache = new PlayerStateParryAttack(this);
		m_RollingCache = new PlayerStateRolling(this);
		m_NormalAttackCache = new PlayerStateNormalAttack(this);
		// 要素上限分リハッシュ
		m_PlayerStatePool.reserve(static_cast<int>(PLAYER_STATE::MAX) - 1);

		m_PlayerStatePool.emplace(PLAYER_STATE::IDLE, new PlayerStateIdle(this));
		m_PlayerStatePool.emplace(PLAYER_STATE::RUN, new PlayerStateRun(this));
		m_PlayerStatePool.emplace(PLAYER_STATE::ATTACK_PARRY, m_ParryCache);
		m_PlayerStatePool.emplace(PLAYER_STATE::ATTACK_NORMAL, m_NormalAttackCache);
		m_PlayerStatePool.emplace(PLAYER_STATE::ROLLING, m_RollingCache);
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
		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;
		ObjectManager* objManager = scene->GetObjectManager();
		if (objManager == nullptr) return;
		m_CameraCache = objManager->GetCamera();

		if (m_CameraCache == nullptr) return;
	}

	// 入力の前にリセット
	m_RandL = MOVE_DIRECTION::NONE;
	m_FandB = MOVE_DIRECTION::NONE;
	m_IsJamp = false;
	m_IsParryAttackButton = false;
	m_IsNormalAttackButton = false;
	m_IsRollingButton = false;
	m_IsExtrAttack = false;

	// 攻撃
	if (InputManager::GetMouseRightPress())
	{
		m_IsParryAttackButton = true;
	}
	if (InputManager::GetMouseLeftPress())
	{
		m_IsNormalAttackButton = true;

		if (m_ParryCount >= EXTRATTACK_ACCEPT_PARRY_MAX)
		{
			m_IsExtrAttack = true;
		}
	}
	// 回避
	if (InputManager::GetKeyPress(VK_SPACE))
	{
		m_IsRollingButton = true;
	}
	// 移動
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
	if (m_IsGround && InputManager::GetKeyPress(VK_SPACE))
	{
		m_IsJamp = true;
	}

	// 入力処理の後に
	if (m_CurrentPlayerState != nullptr)
	{
		m_CurrentPlayerState->Update(deltaTime);
		m_CurrentPlayerState->ChangeStateControl();
	}

	// ステートの更新の後に呼ぶ
	if (m_PlayerCache != nullptr)
	{
		m_PlayerCache->SetVelocityX(m_Velocity.x);
		m_PlayerCache->SetVelocityZ(m_Velocity.z);

		// Yだけ+
		m_PlayerCache->SetVelocityY(m_PlayerCache->GetVelocity().y + m_Velocity.y);

		m_PlayerCache->SetRotationY(m_Rotation.y);

		if (m_AnimeBlendTimeValue != 0.0f)
		{
			m_PlayerCache->SetBlendTimeValue(m_AnimeBlendTimeValue);
		}
	}

	// 最後にリセット
	m_IsGround = false;
	m_IsHitAttacked = false;
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

	m_CurrentState = state;
}

void PlayerStateMachine::InitVelocity()
{
	m_Velocity = {};
}

bool PlayerStateMachine::CheckParry()
{
	if (m_CurrentPlayerState != m_PlayerStatePool[PLAYER_STATE::ATTACK_PARRY]) return false;

	if (m_ParryCache == nullptr) return false;
	
	return m_ParryCache->CheckParryAccept();
}

bool PlayerStateMachine::CheckRolling()
{
	if (m_CurrentPlayerState != m_PlayerStatePool[PLAYER_STATE::ROLLING]) return false;

	if (m_RollingCache == nullptr) return false;

	return m_RollingCache->CheckRollingAccept();
}

bool PlayerStateMachine::CheckAttack()
{
	if (m_CurrentPlayerState != m_PlayerStatePool[PLAYER_STATE::ATTACK_NORMAL]) return false;

	if (m_NormalAttackCache == nullptr) return false;

	return m_NormalAttackCache->CheckAttackAccept();

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

void PlayerStateMachine::HitGround()
{
	m_IsGround = true;
}
