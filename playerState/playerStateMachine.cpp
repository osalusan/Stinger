#include "playerStateMachine.h"
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

void PlayerStateMachine::Update(const float& deltaTime)
{
	if (m_CurrentPlayerState != nullptr)
	{
		m_CurrentPlayerState->Update(deltaTime);
	}
}

void PlayerStateMachine::Draw()
{
	// TODO : �v���C���[�̃A�j���[�V�����Ǘ�
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
