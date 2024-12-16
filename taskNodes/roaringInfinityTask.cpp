#include "roaringInfinityTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"
#include "manager/fbxModelManager.h"
#include "character/mawJLaygo.h"

constexpr float ANIMATION_MAX_TIME = 0.22f;
constexpr float ANIMATION_MIN_TIME = 0.04f;
constexpr float ANIMATION_SPEED = 0.2f;
constexpr float DERAY_MAX_TIME = 0.02f;
void RoaringInfinityTask::Init()
{
	if (MawJLaygo* maw = dynamic_cast<MawJLaygo*>(m_BossCache))
	{
		FbxModelManager::ReservAnimation(ANIMETION_MODEL::MAWJLAYGO, "asset\\model\\MawJ_MutantRoaring60.fbx", "Roaring");
	}
}

NODE_STATUS RoaringInfinityTask::Update(const float& deltaTime)
{
	if (m_BossCache == nullptr)
	{
		return NODE_STATUS::FAILURE;
	}

	BehaviorNode* node = m_BossCache->GetRunningNode();
	if (node != nullptr && node != this)
	{
		return NODE_STATUS::FAILURE;
	}

	m_BossCache->ChangeAnimation("Roaring");

	if (m_AnimeTime >= ANIMATION_MAX_TIME)
	{
		m_Up = false;
		m_AnimeTime = ANIMATION_MAX_TIME - 0.001f;
		m_DerayTime = DERAY_MAX_TIME;
	}
	if (m_AnimeTime <= ANIMATION_MIN_TIME)
	{
		m_Up = true;
		m_AnimeTime = ANIMATION_MIN_TIME + 0.001f;
		m_DerayTime = DERAY_MAX_TIME;
	}

	if (m_DerayTime > 0.0f)
	{
		m_DerayTime -= deltaTime * ANIMATION_SPEED;
	}
	else
	{
		if (m_Up)
		{
			m_AnimeTime += deltaTime * ANIMATION_SPEED;
		}
		else
		{
			m_AnimeTime -= deltaTime * ANIMATION_SPEED;
		}
	}
	m_BossCache->SetAnimeTime(m_AnimeTime);

	m_BossCache->SetRunningNode(this);

	return NODE_STATUS::RUNNING;
}
