#include "playerStateHitDamage.h"
#include "manager/audioManager.h"
#include "manager/fbxModelManager.h"
#include "renderer/fbxModelRenderer.h"

void PlayerStateHitDamage::Init()
{
	m_CurrentTime = 0.0f;

	if (!m_IsLoad)
	{
		LoadAnimation("asset\\model\\player\\impact_PaladinJNordstrom.fbx", "hitAttack");
		AudioManager::ReservAudio(AUDIO::PLAYER_HIT_DAMAGE, "asset\\audio\\se\\hitAttack.wav");

		m_IsLoad = true;
	}
	else
	{
		AudioManager::Play(AUDIO::PLAYER_HIT_DAMAGE);
	}

	if (m_PlayerMachine)
	{
		m_PlayerMachine->InitVelocity();
	}
}

void PlayerStateHitDamage::Unit()
{
	// ƒˆ‰¼‘zŠÖ”‚Ìˆ×‰¼ŽÀ‘•
}

void PlayerStateHitDamage::Update(const float& deltaTime)
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

void PlayerStateHitDamage::ChangeStateControl()
{
	if (m_CurrentTime < m_MaxAnimTime) return;

	ChangePlayerState(PLAYER_STATE::IDLE);
}
