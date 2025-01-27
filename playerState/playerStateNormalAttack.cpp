#include "playerStateNormalAttack.h"
#include "playerStateMachine.h"
#include "manager/fbxModelManager.h"
#include "renderer/fbxModelRenderer.h"

constexpr float BLEND_VALUE_NORMAL_ATTACK = 10.0f;
void PlayerStateNormalAttack::Init()
{
	if (!m_LoadAnimation)
	{
		FbxModelManager::ReservAnimation(ANIMETION_MODEL::PLAYER, "asset\\model\\player\\swordSlash1_PaladinJNordstrom.fbx", "normalAttack1_Player");
		m_AnimName1 = "normalAttack1_Player";
		FbxModelManager::ReservAnimation(ANIMETION_MODEL::PLAYER, "asset\\model\\player\\swordSlash2_PaladinJNordstrom.fbx", "normalAttack2_Player");
		m_AnimName2 = "normalAttack2_Player";
		FbxModelManager::ReservAnimation(ANIMETION_MODEL::PLAYER, "asset\\model\\player\\swordSlash3_PaladinJNordstrom.fbx", "normalAttack3_Player");
		m_AnimName3 = "normalAttack3_Player";

		m_LoadAnimation = true;
	}

	m_CurrentTime = 0.0f;
	m_AttackCancel = false;
	m_MaxAnimTime = m_MaxAnimTime1;
	if (m_PlayerMachine != nullptr)
	{
		m_PlayerMachine->InitVelocity();
		m_PlayerMachine->SetAnimeBlendTimeValue(BLEND_VALUE_NORMAL_ATTACK);
	}
}

void PlayerStateNormalAttack::Unit()
{
	m_CurrentTime = 0.0f;
	m_AttackCancel = false;
}

void PlayerStateNormalAttack::Update(const float& deltaTime)
{
	PlayerState::Update(deltaTime);
	m_CurrentTime += deltaTime;
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
	if (m_CurrentTime >= m_MaxAnimTime1 * m_Attack1CancleValue && m_CurrentTime < m_MaxAnimTime1)
	{
		m_AttackCancel = true;
		if (isAttackButton)
		{
			m_CurrentTime = m_MaxAnimTime1;
			m_MaxAnimTime = maxAnimTime1and2;
			m_AttackCancel = false;
		}
	}
	else if (m_CurrentTime >= m_MaxAnimTime1 + (m_MaxAnimTime2 * m_Attack2CancleValue) && m_CurrentTime < maxAnimTime1and2)
	{
		m_AttackCancel = true;
		if (isAttackButton)
		{
			m_CurrentTime = maxAnimTime1and2;
			m_MaxAnimTime = maxAnimTime1and2 + m_MaxAnimTime3;
			m_AttackCancel = false;
		}
	}
	else if (m_CurrentTime >= maxAnimTime1and2 + (m_MaxAnimTime3 * m_Attack3CancleValue) && m_CurrentTime < maxAnimTime1and2 + m_MaxAnimTime3)
	{
		m_AttackCancel = true;
	}



	RotToCameraDirection(deltaTime);

	if (m_PlayerMachine != nullptr)
	{
		m_PlayerMachine->InitVelocity();
	}
}

void PlayerStateNormalAttack::ChangeStateControl()
{
	if (m_PlayerMachine == nullptr) return;

	// 優先順位順

	// TODO :アニメーションの時間を設定
	if (m_CurrentTime >= m_MaxAnimTime)
	{
		ChangePlayerState(PLAYER_STATE::IDLE);
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

