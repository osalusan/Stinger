#include "playerStateExtrAttack.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "manager/fbxModelManager.h"
#include "manager/audioManager.h"
#include "renderer/fbxModelRenderer.h"
#include "scene/scene.h"
#include "character/bossEnemy.h"

void PlayerStateExtrAttack::Init()
{
	m_CurrentTime = 0.0f;
	if (m_PlayerMachine == nullptr) return;

	if (m_PlayerCache == nullptr)
	{
		// Getのみ / 編集不可
		const Player* playerCache = m_PlayerMachine->GetPlayerCache();
		m_PlayerCache = playerCache;
	}

	if (!m_Load && m_PlayerCache != nullptr)
	{
		LoadAnimation("asset\\model\\player\\swordAndShieldExtrAttack_PaladinJNordstrom.fbx", "extrAttack");

		// TODO :音をつける
		// AudioManager::ReservAudio(AUDIO::PARRY_SE, "asset\\audio\\se\\parry.wav");

		const std::unordered_map<std::string, float>& parryAttak = m_PlayerCache->GetStateData("エクストラ攻撃");

		m_DamageValue = FindStateData(parryAttak, "ダメージ_倍率");

		m_Load = true;
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

	if (m_ObjManagerCache != nullptr && m_PlayerMachine->GetIsExtrAttack())
	{
		m_ObjManagerCache->SetSlowTime(1.5f);
		m_ObjManagerCache->SetSlowValue(0.0f);
	}
}

void PlayerStateExtrAttack::Unit()
{
	m_CurrentTime = 0.0f;
}

void PlayerStateExtrAttack::Update(const float& deltaTime)
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
}

void PlayerStateExtrAttack::ChangeStateControl()
{
	if (m_CurrentTime >= m_MaxAnimTime)
	{
		ChangePlayerState(PLAYER_STATE::IDLE);
	}
}
