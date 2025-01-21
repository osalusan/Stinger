#include "playerStateHoldWeapon.h"
#include "character/player.h"

void PlayerStateHoldWeapon::Init()
{
	m_CurrentTime = 0.0f;
	// Getのみ / 編集不可
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

	// 優先順位順

	if (m_PlayerMachine->GetIsHitAttacked())
	{
		if (m_CurrentTime >= m_MinParryTime && m_CurrentTime <= m_MaxParryTime)
		{
			ChangePlayerState(PLAYER_STATE::PARRY);
		}
		// TODO :追加予定 / 被ダメ時のステートへ
	}
	else if (!m_PlayerMachine->GetIsHold())
	{
		ChangePlayerState(PLAYER_STATE::IDLE);
	}


}
