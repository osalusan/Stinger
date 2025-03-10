#include "playerStateHitAttack.h"
#include "manager/audioManager.h"
#include "manager/fbxModelManager.h"
#include "renderer/fbxModelRenderer.h"

void PlayerStateHitAttack::Init()
{
	m_CurrentTime = 0.0f;

	if (!m_Load)
	{
		// TODO :���ƃA�j���[�V������ύX�\�� / ���f��
		LoadAnimation("asset\\model\\player\\shieldParry_PaladinJNordstrom.fbx", "parryAttack");
		AudioManager::ReservAudio(AUDIO::HIT_ATTACK, "asset\\audio\\se\\parry.wav");

		m_Load = true;
	}

	AudioManager::Play(AUDIO::HIT_ATTACK);
}

void PlayerStateHitAttack::Unit()
{
	// �������z�֐��̈׉�����
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
