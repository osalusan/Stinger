#include "roaringInfinityTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"
#include "manager/fbxModelManager.h"
#include "character/mawJLaygo.h"

constexpr float MAX_TIME_ANIMEATTACK = 3.0f;

void RoaringInfinityTask::Init()
{
	if (MawJLaygo* maw = dynamic_cast<MawJLaygo*>(m_BossCache))
	{
		FbxModelManager::ReservAnimation(ANIMETION_MODEL::MAWJLAYGO, "asset\\model\\MawJ_MutantRoaring60.fbx", "Roaring");
	}
	m_CurrentTime = MAX_TIME_ANIMEATTACK;
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

	if (m_CurrentTime >= MAX_TIME_ANIMEATTACK)
	{
		if (node == nullptr)
		{
			m_CurrentTime = 0.0f;
		}
	}

	if (m_CurrentTime < MAX_TIME_ANIMEATTACK)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation("Roaring");

		// UŒ‚ó‘Ô‚ð•Û‘¶
		m_BossCache->SetRunningNode(this);

		return NODE_STATUS::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// UŒ‚ó‘Ô‚ðíœ
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATUS::SUCCESS;
		}
	}

	return NODE_STATUS::FAILURE;
}
