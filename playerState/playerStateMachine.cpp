#include "playerStateMachine.h"
#include "manager/inputManager.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "camera/playerCamera.h"
#include "scene/gameScene.h"
#include "playerState.h"
#include "playerStateIdle.h"
#include "playerStateRun.h"
#include "playerStateJump.h"
#include "character/player.h"

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
		// �v�f��������n�b�V��
		m_PlayerStatePool.reserve(static_cast<int>(PLAYER_STATE::MAX) - 1);

		m_PlayerStatePool.emplace(PLAYER_STATE::IDLE, new PlayerStateIdle(this));
		m_PlayerStatePool.emplace(PLAYER_STATE::RUN, new PlayerStateRun(this));
		m_PlayerStatePool.emplace(PLAYER_STATE::JUMP, new PlayerStateJump(this));
	}
	// ������
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
		// null�`�F�b�N
		if (PlayerStatePool.second != nullptr)
		{
			PlayerStatePool.second->Unit();
		}
	}
}
// ��Ɉړ��̏���
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

	// ���͂̑O�Ƀ��Z�b�g
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

	//�W�����v
	if (m_IsGround && InputManager::GetKeyPress(VK_SPACE))
	{
		m_IsJamp = true;
	}

	// ���͏����̌��
	if (m_CurrentPlayerState != nullptr)
	{
		m_CurrentPlayerState->Update(deltaTime);
		m_CurrentPlayerState->ChangeStateControl();
	}

	// �X�e�[�g�̍X�V�̌�ɌĂ�
	if (m_PlayerCache != nullptr)
	{
		m_PlayerCache->SetVelocityX(m_Velocity.x);
		m_PlayerCache->SetVelocityZ(m_Velocity.z);

		// Y����+
		m_PlayerCache->SetVelocityY(m_PlayerCache->GetVelocity().y + m_Velocity.y);

		m_PlayerCache->SetRotationY(m_Rotation.y);
	}

	// �Ō�Ƀ��Z�b�g
	m_IsGround = false;
}

void PlayerStateMachine::SetPlayerState(const PLAYER_STATE& state)
{
	// �����X�e�[�g��������͂���
	if (m_CurrentPlayerState == m_PlayerStatePool[state]) return;

	// �I������
	if (m_CurrentPlayerState != nullptr)
	{
		m_CurrentPlayerState->Unit();
	}

	// null�`�F�b�N
	if (m_PlayerStatePool[state] != nullptr)
	{
		m_CurrentPlayerState = m_PlayerStatePool[state];
	}

	// ������
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

void PlayerStateMachine::HitGround()
{
	m_IsGround = true;
}
