#include "playerStateParry.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "scene/scene.h"
#include "character/bossEnemy.h"

void PlayerStateParry::Init()
{
	m_CurrentTime = 0.0f;
	// Getのみ / 編集不可
	const Player* playerCache = m_PlayerMachine->GetPlayerCache();
	if (playerCache == nullptr) return;
	m_MinParryTime = playerCache->GetMinParryTime();
	m_MaxParryTime = playerCache->GetMaxParryTime();

	if (m_BossCache == nullptr)
	{
		if (m_ObjManagerCache != nullptr)
		{
			m_BossCache = m_ObjManagerCache->GetBossEnemy();
		}
	}
	if (m_ObjManagerCache == nullptr)
	{
		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;
		m_ObjManagerCache = scene->GetObjectManager();
	}
}

void PlayerStateParry::Unit()
{
	m_CurrentTime = 0.0f;
}

void PlayerStateParry::Update(const float& deltaTime)
{
	PlayerState::Update(deltaTime);
	m_CurrentTime += deltaTime;
	RotToCameraDirection(deltaTime);

	if (m_PlayerMachine->GetIsHitAttacked() && !m_ParryAccept)
	{
		if (m_CurrentTime >= m_MinParryTime && m_CurrentTime <= m_MaxParryTime)
		{
			if (m_ObjManagerCache == nullptr || m_BossCache == nullptr)return;

			m_BossCache->SetParryRecoil(true);

			m_ObjManagerCache->SetSlowTime(0.9f);
			m_ObjManagerCache->SetSlowValue(0.0f);

			m_ParryAccept = true;
		}
	}
}

void PlayerStateParry::ChangeStateControl()
{
	// TODO :アニメーションの時間を設定
	if (m_CurrentTime >= 1.0f)
	{
		ChangePlayerState(PLAYER_STATE::IDLE);
		if (m_ObjManagerCache == nullptr) return;
		m_ObjManagerCache->SetSlowTime(0.0f);
	}
}
