#include "playerStateNormalAttack.h"
#include "playerStateMachine.h"
#include "manager/fbxModelManager.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "manager/audioManager.h"
#include "renderer/fbxModelRenderer.h"
#include "character/player.h"
#include "scene/scene.h"
#include "equipment/equipmentObject.h"

void PlayerStateNormalAttack::Init()
{
	if (m_PlayerMachine != nullptr)
	{
		m_PlayerMachine->InitVelocity();
		m_PlayerMachine->SetAnimeBlendTimeValue(m_BlendTime);

		if (m_PlayerCache == nullptr)
		{
			// Getのみ / 編集不可
			const Player* playerCache = m_PlayerMachine->GetPlayerCache();
			m_PlayerCache = playerCache;
		}
	}

	if (!m_Load && m_PlayerCache != nullptr)
	{
		FbxModelManager::ReservAnimation(ANIMETION_MODEL::PLAYER, "asset\\model\\player\\swordSlash1_PaladinJNordstrom.fbx", "normalAttack1");
		m_AnimName1 = "normalAttack1";
		FbxModelManager::ReservAnimation(ANIMETION_MODEL::PLAYER, "asset\\model\\player\\swordSlash2_PaladinJNordstrom.fbx", "normalAttack2");
		m_AnimName2 = "normalAttack2";
		FbxModelManager::ReservAnimation(ANIMETION_MODEL::PLAYER, "asset\\model\\player\\swordSlash3_PaladinJNordstrom.fbx", "normalAttack3");
		m_AnimName3 = "normalAttack3";

		AudioManager::ReservAudio(AUDIO::SLASH1_SE, "asset\\audio\\se\\slash1.wav");
		AudioManager::ReservAudio(AUDIO::SLASH2_SE, "asset\\audio\\se\\slash2.wav");

		const std::unordered_map<std::string, float>& normalAttak = m_PlayerCache->GetStateData("通常攻撃");

		m_AttackEnableTimeValue1 = FindStateData(normalAttak, "一段階目ダメージ開始時間_割合");
		m_AttackEnableTimeValue2 = FindStateData(normalAttak, "二段階目ダメージ開始時間_割合");
		m_AttackEnableTimeValue3 = FindStateData(normalAttak, "三段階目ダメージ開始時間_割合");

		m_DamageValue1 = FindStateData(normalAttak, "一段階目ダメージ_倍率");
		m_DamageValue2 = FindStateData(normalAttak, "二段階目ダメージ_倍率");
		m_DamageValue3 = FindStateData(normalAttak, "三段階目ダメージ_倍率");

		m_AttackCancleValue1 = FindStateData(normalAttak, "一段階目キャンセル可能時間_割合");
		m_AttackCancleValue2 = FindStateData(normalAttak, "二段階目キャンセル可能時間_割合");
		m_AttackCancleValue3 = FindStateData(normalAttak, "三段階目キャンセル可能時間_割合");

		m_BlendTime = FindStateData(normalAttak, "ブレンド速度");

		m_LoadAnimation = true;
		m_Load = true;
	}

	m_CurrentTime = 0.0f;
	m_AttackCancel = false;
	m_UseAttack = true;
	m_AttackAccept = true;
	m_MaxAnimTime = m_MaxAnimTime1;
	m_AttackComboNumber = 1;
	m_AnimSpeedValue = 1.0f;

	if (m_PlayerCache != nullptr)
	{
		m_AttackDamage = m_PlayerCache->GetAttack() * m_DamageValue1;
	}

	if (m_BossCache == nullptr)
	{
		Scene* scene = SceneManager::GetScene();
		if (scene == nullptr) return;
		ObjectManager* objManager = scene->GetObjectManager();
		m_BossCache = objManager->GetBossEnemy();
	}
}

void PlayerStateNormalAttack::Unit()
{
	m_CurrentTime = 0.0f;
	m_AttackDamage = 0.0f;
	m_AttackCancel = false;
	m_UseAttack = true;
	m_AttackAccept = true;
}

void PlayerStateNormalAttack::Update(const float& deltaTime)
{
	PlayerState::Update(deltaTime);
	m_CurrentTime += deltaTime * m_AnimSpeedValue;
	if (m_PlayerMachine == nullptr) return;

	// 初回のみ
	if (m_MaxAnimTime1 == 0.0f)
	{
		if (FbxModelRenderer* model = FbxModelManager::GetAnimationModel(ANIMETION_MODEL::PLAYER))
		{
			m_MaxAnimTime1 = model->GetMaxAnimeTime(m_AnimName1);
		}
	}
	if (m_MaxAnimTime2 == 0.0f)
	{
		if (FbxModelRenderer* model = FbxModelManager::GetAnimationModel(ANIMETION_MODEL::PLAYER))
		{
			m_MaxAnimTime2 = model->GetMaxAnimeTime(m_AnimName2);
		}
	}
	if (m_MaxAnimTime3 == 0.0f)
	{
		if (FbxModelRenderer* model = FbxModelManager::GetAnimationModel(ANIMETION_MODEL::PLAYER))
		{
			m_MaxAnimTime3 = model->GetMaxAnimeTime(m_AnimName3);
		}
	}
	if (m_MaxAnimTime == 0.0f)
	{
		m_MaxAnimTime = m_MaxAnimTime1;
	}

	// 連続攻撃のアニメーション制御
	if (m_CurrentTime < m_MaxAnimTime1)
	{
		m_PlayerMachine->SetAnimation(m_AnimName1);
	}
	else if (m_CurrentTime < m_MaxAnimTime1 + m_MaxAnimTime2)
	{
		m_PlayerMachine->SetAnimation(m_AnimName2);
	}
	else if (m_CurrentTime < m_MaxAnimTime1 + m_MaxAnimTime2 + m_MaxAnimTime3)
	{
		m_PlayerMachine->SetAnimation(m_AnimName3);
	}

	// 待機モーションの攻撃キャンセル
	const float& maxAnimTime1and2 = m_MaxAnimTime1 + m_MaxAnimTime2;
	const bool& isAttackButton = m_PlayerMachine->GetIsNormalAttackButton();
	if (m_CurrentTime < m_MaxAnimTime1)
	{
		// ダメージ発生開始
		if (m_CurrentTime >= m_MaxAnimTime1 * m_AttackEnableTimeValue1)
		{
			if (m_UseAttack)
			{
				m_AttackAccept = false;
			}
		}
		else
		{
			RotToCameraDirection(deltaTime);
			m_AttackAccept = true;
		}
		// アニメーションの途中終了可能設定
		if (m_CurrentTime >= m_MaxAnimTime1 * m_AttackCancleValue1)
		{
			m_AttackCancel = true;
			if (isAttackButton)
			{
				m_CurrentTime = m_MaxAnimTime1;
				m_MaxAnimTime = maxAnimTime1and2;
				m_UseAttack = true;
				m_AttackCancel = false;
				m_AttackComboNumber = 2;
				m_AttackDamage = m_PlayerCache->GetAttack() * m_DamageValue2;
				m_AnimSpeedValue = 1.15f;
				m_PlayerMachine->SetAnimationSpeedValue(m_AnimSpeedValue);
			}
		}

	}
	else if (m_CurrentTime < maxAnimTime1and2)
	{
		// ダメージ発生開始
		if (m_CurrentTime >= m_MaxAnimTime1 + (m_MaxAnimTime2 * m_AttackEnableTimeValue2))
		{
			if (m_UseAttack)
			{
				m_AttackAccept = false;
			}
		}
		else
		{
			RotToInputKeyDirection(deltaTime);
			m_AttackAccept = true;
		}
		// アニメーションの途中終了可能設定
		if (m_CurrentTime >= m_MaxAnimTime1 + (m_MaxAnimTime2 * m_AttackCancleValue2))
		{
			m_AttackCancel = true;
			if (isAttackButton)
			{
				m_CurrentTime = maxAnimTime1and2;
				m_MaxAnimTime = maxAnimTime1and2 + m_MaxAnimTime3;
				m_UseAttack = true;
				m_AttackCancel = false;
				m_AttackComboNumber = 3;
				m_AttackDamage = m_PlayerCache->GetAttack() * m_DamageValue3;

				m_AnimSpeedValue = 1.25f;
				m_PlayerMachine->SetAnimationSpeedValue(m_AnimSpeedValue);
			}
		}

	}
	else if (m_CurrentTime < maxAnimTime1and2 + m_MaxAnimTime3)
	{
		// ダメージ発生開始
		if (m_CurrentTime >= maxAnimTime1and2 + (m_MaxAnimTime3 * m_AttackEnableTimeValue3))
		{
			if (m_UseAttack)
			{
				m_AttackAccept = false;
			}
		}
		else
		{
			RotToInputKeyDirection(deltaTime);
			m_AttackAccept = true;
		}
		// アニメーションの途中終了可能設定
		if (m_CurrentTime >= maxAnimTime1and2 + (m_MaxAnimTime3 * m_AttackCancleValue3))
		{
			m_AttackCancel = true;
			m_AnimSpeedValue = 1.0f;
			m_PlayerMachine->SetAnimationSpeedValue(m_AnimSpeedValue);
		}
	}

	if (m_PlayerMachine != nullptr)
	{
		m_PlayerMachine->InitVelocity();
	}
}

void PlayerStateNormalAttack::ChangeStateControl()
{
	if (m_PlayerMachine == nullptr) return;

	// 優先順位順

	if (m_CurrentTime >= m_MaxAnimTime)
	{
		ChangePlayerState(PLAYER_STATE::IDLE);
	}
	else if (m_PlayerMachine->GetIsExtrAttack())
	{
		ChangePlayerState(PLAYER_STATE::ATTACK_EXTR);
	}
	else if (m_AttackCancel)
	{
		// パリィ攻撃が押されたら
		if (m_PlayerMachine->GetIsParryAttackButton())
		{
			ChangePlayerState(PLAYER_STATE::ATTACK_PARRY);
		}
		// 通常攻撃が押されたら
		else if (m_PlayerMachine->GetIsNormalAttackButton())
		{
			ChangePlayerState(PLAYER_STATE::ATTACK_NORMAL);
		}
		// 回避が押されたら
		else if (m_PlayerMachine->GetIsRollingButton())
		{
			ChangePlayerState(PLAYER_STATE::ROLLING);
		}
	}
}

bool PlayerStateNormalAttack::CheckAttackAccept()
{
	if (!m_AttackCancel && !m_AttackAccept)
	{
		if (m_BossCache == nullptr) return false;

		m_BossCache->TakeDamage(m_AttackDamage);
		m_UseAttack = false;
		m_AttackAccept = true;

		if (m_AttackComboNumber == 1 || m_AttackComboNumber == 2)
		{
			AudioManager::Play(AUDIO::SLASH1_SE,false,0.8f);
		}
		else if (m_AttackComboNumber == 3)
		{
			AudioManager::Play(AUDIO::SLASH2_SE,false,0.85f);
		}
	}
	return m_AttackAccept;
}

