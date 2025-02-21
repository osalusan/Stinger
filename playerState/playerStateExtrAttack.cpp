#include "playerStateExtrAttack.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "manager/fbxModelManager.h"
#include "manager/audioManager.h"
#include "renderer/fbxModelRenderer.h"
#include "scene/scene.h"
#include "character/bossEnemy.h"
#include "camera/playerCamera.h"

void PlayerStateExtrAttack::Init()
{
	m_CurrentTime = 0.0f;
	m_AttackAccept = false;

	if (m_PlayerMachine == nullptr) return;

	m_PlayerMachine->UseExtrAttack();

	if (m_PlayerCache == nullptr)
	{
		// Getのみ / 編集不可
		const Player* playerCache = m_PlayerMachine->GetPlayerCache();
		m_PlayerCache = playerCache;
	}

	if (!m_Load && m_PlayerCache != nullptr)
	{
		LoadAnimation("asset\\model\\player\\swordAndShieldExtrAttack_PaladinJNordstrom.fbx", "extrAttack");

		// TODO :変更予定 / 仮素材
		AudioManager::ReservAudio(AUDIO::SLASH2_SE, "asset\\audio\\se\\slash2.wav");

		const std::unordered_map<std::string, float>& parryAttak = m_PlayerCache->GetStateData("エクストラ攻撃");

		m_DamageValue = FindStateData(parryAttak, "ダメージ_倍率");

		m_Load = true;
	}

	if (m_ObjManagerCache == nullptr)
	{
		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;
		m_ObjManagerCache = scene->GetObjectManager();
	}

	if (m_BossCache == nullptr)
	{
		if (m_ObjManagerCache != nullptr)
		{
			m_BossCache = m_ObjManagerCache->GetBossEnemy();
		}
	}

	if (m_CameraCache == nullptr)
	{
		if (m_ObjManagerCache != nullptr)
		{
			m_CameraCache = dynamic_cast<PlayerCamera*>(m_ObjManagerCache->GetCamera());
		}
	}	
	if (m_CameraCache != nullptr)
	{
		m_CameraCache->StartCutIn();
	}

	if (m_ObjManagerCache != nullptr && m_PlayerMachine->GetIsExtrAttack())
	{
		// TODO :エネミーの動きのみを止める処理

		//m_ObjManagerCache->SetSlowTime(1.5f);
		//m_ObjManagerCache->SetSlowValue(0.0f);
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
		if (m_CameraCache != nullptr)
		{
			m_CameraCache->EndCutIn();
		}
		ChangePlayerState(PLAYER_STATE::IDLE);
	}
}

bool PlayerStateExtrAttack::CheckAttackAccept()
{
	if (!m_AttackAccept)
	{
		if (m_BossCache == nullptr || m_PlayerCache == nullptr)
		{
			return false;
		}

		m_BossCache->TakeDamage(m_PlayerCache->GetAttack() * m_DamageValue);
		m_AttackAccept = true;

		// TODO :音の変更予定 / 借り素材
		AudioManager::Play(AUDIO::SLASH2_SE, false, 0.85f);
	}
	return m_AttackAccept;
}
