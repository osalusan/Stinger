#include "playerStateParryAttack.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "scene/scene.h"
#include "character/bossEnemy.h"

constexpr float SPEED_ATTENUATE_VALUE = 67.0f;	// ���x�̌����l
void PlayerStateParryAttack::Init()
{
	m_CurrentTime = 0.0f;
	// Get�̂� / �ҏW�s��
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

void PlayerStateParryAttack::Unit()
{
	m_CurrentTime = 0.0f;
}

void PlayerStateParryAttack::Update(const float& deltaTime)
{
	PlayerState::Update(deltaTime);
	m_CurrentTime += deltaTime;

	if (m_PlayerMachine == nullptr) return;

	RotToCameraDirection(deltaTime);

	if (m_PlayerMachine->GetIsHitAttacked() && !m_ParryAccept)
	{
		if (m_CurrentTime >= m_MinParryTime && m_CurrentTime <= m_MaxParryTime)
		{
			if (m_ObjManagerCache == nullptr || m_BossCache == nullptr)return;
			m_PlayerMachine->InitVelocity();

			m_BossCache->SetParryRecoil(true);

			m_ObjManagerCache->SetSlowTime(0.9f);
			m_ObjManagerCache->SetSlowValue(0.0f);

			m_ParryAccept = true;
		}
	}

	// ����������̏����̌��
	if (m_PlayerMachine->GetVelocity().x != 0.0f && m_PlayerMachine->GetVelocity().z != 0.0f)
	{
		m_PlayerMachine->SetVelocityX(m_PlayerMachine->GetVelocity().x / (SPEED_ATTENUATE_VALUE * deltaTime));
		m_PlayerMachine->SetVelocityZ(m_PlayerMachine->GetVelocity().z / (SPEED_ATTENUATE_VALUE * deltaTime));
	}
}

void PlayerStateParryAttack::ChangeStateControl()
{
	// TODO :�A�j���[�V�����̎��Ԃ�ݒ�
	if (m_CurrentTime >= 1.0f)
	{
		ChangePlayerState(PLAYER_STATE::IDLE);
		if (m_ObjManagerCache == nullptr) return;
		m_ObjManagerCache->SetSlowTime(0.0f);
	}
}
