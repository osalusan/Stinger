#include "playerStateHitAttack.h"
#include "manager/audioManager.h"
#include "manager/fbxModelManager.h"
#include "renderer/fbxModelRenderer.h"

void PlayerStateHitAttack::Init()
{
	m_CurrentTime = 0.0f;

	if (!m_Load)
	{
		LoadAnimation("asset\\model\\player\\impact_PaladinJNordstrom.fbx", "hitAttack");
		AudioManager::ReservAudio(AUDIO::HIT_ATTACK, "asset\\audio\\se\\hitAttack.wav");

		m_Load = true;
	}

	AudioManager::Play(AUDIO::HIT_ATTACK);

	if (m_PlayerMachine)
	{
		m_PlayerMachine->InitVelocity();
	}
}

void PlayerStateHitAttack::Unit()
{
	// ƒˆ‰¼‘zŠÖ”‚Ìˆ×‰¼ŽÀ‘•
}

void PlayerStateHitAttack::Update(const float& deltaTime)
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

void PlayerStateHitAttack::ChangeStateControl()
{
	if (m_CurrentTime < m_MaxAnimTime) return;

	ChangePlayerState(PLAYER_STATE::IDLE);
}
