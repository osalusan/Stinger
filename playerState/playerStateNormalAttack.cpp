#include "playerStateNormalAttack.h"
#include "playerStateMachine.h"
#include "manager/fbxModelManager.h"
#include "manager/sceneManager.h"
#include "manager/objectManager.h"
#include "renderer/fbxModelRenderer.h"
#include "character/player.h"
#include "scene/scene.h"
#include "equipment/equipmentObject.h"

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
	m_UseAttack = true;
	m_AttackAccept = true;
	m_MaxAnimTime = m_MaxAnimTime1;

	if (m_PlayerMachine != nullptr)
	{
		m_PlayerMachine->InitVelocity();
		m_PlayerMachine->SetAnimeBlendTimeValue(m_BlendTime);

		if (m_PlayerCache == nullptr)
		{
			// Get�̂� / �ҏW�s��
			const Player* playerCache = m_PlayerMachine->GetPlayerCache();
			m_PlayerCache = playerCache;
		}
	}
	if (m_PlayerCache != nullptr)
	{
		if (m_AttackEnableTimeValue1 == 0.0f)
		{
			const std::unordered_map<std::string, float>& normalAttak = m_PlayerCache->GetStateData("�ʏ�U��");
			m_AttackEnableTimeValue1 = FindStateData(normalAttak, "��i�K�ڃ_���[�W�����J�n���Ԋ���");
			m_AttackEnableTimeValue2 = FindStateData(normalAttak, "��i�K�ڃ_���[�W�����J�n���Ԋ���");
			m_AttackEnableTimeValue3 = FindStateData(normalAttak, "�O�i�K�ڃ_���[�W�����J�n���Ԋ���");

			m_DamageValue1 = FindStateData(normalAttak, "��i�K�ڃ_���[�W�{��");
			m_DamageValue2 = FindStateData(normalAttak, "��i�K�ڃ_���[�W�{��");
			m_DamageValue3 = FindStateData(normalAttak, "�O�i�K�ڃ_���[�W�{��");

			m_AttackCancleValue1 = FindStateData(normalAttak, "��i�K�ڃL�����Z���\���Ԋ���");
			m_AttackCancleValue2 = FindStateData(normalAttak, "��i�K�ڃL�����Z���\���Ԋ���");
			m_AttackCancleValue3 = FindStateData(normalAttak, "�O�i�K�ڃL�����Z���\���Ԋ���");

			m_BlendTime = FindStateData(normalAttak, "�u�����h���x");
		}
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
	m_CurrentTime += deltaTime;
	if (m_PlayerMachine == nullptr) return;

	// ����̂�
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

	// �A���U���̃A�j���[�V��������
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

	// �ҋ@���[�V�����̍U���L�����Z��
	const float& maxAnimTime1and2 = m_MaxAnimTime1 + m_MaxAnimTime2;
	const bool& isAttackButton = m_PlayerMachine->GetIsNormalAttackButton();
	if (m_CurrentTime < m_MaxAnimTime1)
	{
		// �_���[�W�����J�n
		if (m_CurrentTime >= m_MaxAnimTime1 * m_AttackEnableTimeValue1)
		{
			if (m_UseAttack)
			{
				m_AttackAccept = false;
			}
		}
		// �A�j���[�V�����̓r���I���\�ݒ�
		if (m_CurrentTime >= m_MaxAnimTime1 * m_AttackCancleValue1)
		{
			m_AttackCancel = true;
			if (isAttackButton)
			{
				m_CurrentTime = m_MaxAnimTime1;
				m_MaxAnimTime = maxAnimTime1and2;
				m_UseAttack = true;
				m_AttackCancel = false;
				m_AttackDamage = m_PlayerCache->GetAttack() * m_DamageValue2;
			}
		}

	}
	else if (m_CurrentTime < maxAnimTime1and2)
	{
		// �_���[�W�����J�n
		if (m_CurrentTime >= m_MaxAnimTime1 + (m_MaxAnimTime2 * m_AttackEnableTimeValue2))
		{
			if (m_UseAttack)
			{
				m_AttackAccept = false;
			}
		}
		// �A�j���[�V�����̓r���I���\�ݒ�
		if (m_CurrentTime >= m_MaxAnimTime1 + (m_MaxAnimTime2 * m_AttackCancleValue2))
		{
			m_AttackCancel = true;
			if (isAttackButton)
			{
				m_CurrentTime = maxAnimTime1and2;
				m_MaxAnimTime = maxAnimTime1and2 + m_MaxAnimTime3;
				m_UseAttack = true;
				m_AttackCancel = false;
				m_AttackDamage = m_PlayerCache->GetAttack() * m_DamageValue3;
			}
		}

	}
	else if (m_CurrentTime < maxAnimTime1and2 + m_MaxAnimTime3)
	{
		// �_���[�W�����J�n
		if (m_CurrentTime >= maxAnimTime1and2 + (m_MaxAnimTime3 * m_AttackEnableTimeValue3))
		{
			if (m_UseAttack)
			{
				m_AttackAccept = false;
			}
		}
		// �A�j���[�V�����̓r���I���\�ݒ�
		if (m_CurrentTime >= maxAnimTime1and2 + (m_MaxAnimTime3 * m_AttackCancleValue3))
		{
			m_AttackCancel = true;
		}
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

	// �D�揇�ʏ�

	// TODO :�A�j���[�V�����̎��Ԃ�ݒ�
	if (m_CurrentTime >= m_MaxAnimTime)
	{
		ChangePlayerState(PLAYER_STATE::IDLE);
	}
	else if (m_AttackCancel)
	{
		// �p���B�U���������ꂽ��
		if (m_PlayerMachine->GetIsParryAttackButton())
		{
			ChangePlayerState(PLAYER_STATE::ATTACK_PARRY);
		}
		// �ʏ�U���������ꂽ��
		else if (m_PlayerMachine->GetIsNormalAttackButton())
		{
			ChangePlayerState(PLAYER_STATE::ATTACK_NORMAL);
		}
		// ����������ꂽ��
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
	}
	return m_AttackAccept;
}

