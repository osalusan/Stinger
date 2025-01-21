#include "playerStateHoldWeapon.h"
#include "character/player.h"

void PlayerStateHoldWeapon::Init()
{
	m_CurrentTime = 0.0f;
	// Get�̂� / �ҏW�s��
	const Player* playerCache = m_PlayerMachine->GetPlayerCache();
	if (playerCache == nullptr) return;
	m_MinParryTime = playerCache->GetMinParryTime();
	m_MaxParryTime = playerCache->GetMaxParryTime();
}

void PlayerStateHoldWeapon::Unit()
{
	m_CurrentTime = 0.0f;
}

void PlayerStateHoldWeapon::Update(const float& deltaTime)
{
	PlayerState::Update(deltaTime);
	m_CurrentTime += deltaTime;
}

void PlayerStateHoldWeapon::ChangeStateControl()
{
	if (m_PlayerMachine == nullptr) return;

	// �D�揇�ʏ�

	if (m_PlayerMachine->GetIsHitAttacked())
	{
		if (m_CurrentTime >= m_MinParryTime && m_CurrentTime <= m_MaxParryTime)
		{
			ChangePlayerState(PLAYER_STATE::PARRY);
		}
		// TODO :�ǉ��\�� / ��_�����̃X�e�[�g��
	}
	else if (!m_PlayerMachine->GetIsHold())
	{
		ChangePlayerState(PLAYER_STATE::IDLE);
	}


}
