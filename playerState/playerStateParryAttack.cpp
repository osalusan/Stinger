#include "playerStateParryAttack.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "manager/fbxModelManager.h"
#include "renderer/fbxModelRenderer.h"
#include "scene/scene.h"
#include "character/bossEnemy.h"

void PlayerStateParryAttack::Init()
{
	LoadAnimation("asset\\model\\player\\shieldParry_PaladinJNordstrom.fbx", "parryAttack_Player");

	m_CurrentTime = 0.0f;

	if (m_PlayerCache == nullptr)
	{
		// Getのみ / 編集不可
		const Player* playerCache = m_PlayerMachine->GetPlayerCache();
		m_PlayerCache = playerCache;
	}

	if (m_PlayerCache != nullptr)
	{
		if (m_MinParryTime == 0.0f)
		{
			const std::unordered_map<std::string, float>& parryAttak = m_PlayerCache->GetStateData("パリィ攻撃");

			m_MinParryTime = FindStateData(parryAttak, "パリィ成功時間_最小");
			m_MaxParryTime = FindStateData(parryAttak, "パリィ成功時間_最大");
			m_SpeedAttenuateValue = FindStateData(parryAttak, "速度の減衰値");
		}
	}

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
	m_ParryAccept = false;
}

void PlayerStateParryAttack::Update(const float& deltaTime)
{
	PlayerState::Update(deltaTime);
	m_CurrentTime += deltaTime;

	if (m_MaxAnimTime == 0.0f)
	{
		if (FbxModelRenderer* model = FbxModelManager::GetAnimationModel(ANIMETION_MODEL::PLAYER))
		{
			m_MaxAnimTime = model->GetMaxAnimeTime(m_AnimName);
		}
	}

	if (m_PlayerMachine == nullptr) return;

	RotToCameraDirection(deltaTime);


	if (m_PlayerMachine->GetVelocity().x != 0.0f && m_PlayerMachine->GetVelocity().z != 0.0f)
	{
		m_PlayerMachine->SetVelocityX(m_PlayerMachine->GetVelocity().x / (m_SpeedAttenuateValue * deltaTime));
		m_PlayerMachine->SetVelocityZ(m_PlayerMachine->GetVelocity().z / (m_SpeedAttenuateValue * deltaTime));
	}
}

void PlayerStateParryAttack::ChangeStateControl()
{
	if (m_CurrentTime >= m_MaxAnimTime)
	{
		ChangePlayerState(PLAYER_STATE::IDLE);
		if (m_ObjManagerCache == nullptr) return;
		m_ObjManagerCache->SetSlowTime(0.0f);
	}
}

bool PlayerStateParryAttack::CheckParryAccept()
{
	if (!m_ParryAccept)
	{
		if (m_CurrentTime >= m_MinParryTime && m_CurrentTime <= m_MaxParryTime)
		{
			if (m_ObjManagerCache == nullptr || m_BossCache == nullptr)return false;
			m_PlayerMachine->InitVelocity();

			m_BossCache->SetParryRecoil(true);

			m_ObjManagerCache->SetSlowTime(0.8f);
			m_ObjManagerCache->SetSlowValue(0.3f);

			m_ParryAccept = true;
		}
	}
	return m_ParryAccept;
}
