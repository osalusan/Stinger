#include "roaringTask.h"
#include "character/player.h"
#include "character/bossEnemy.h"
#include "manager/fbxModelManager.h"
#include "character/mawJLaygo.h"

constexpr float MAX_TIME_ANIMEATTACK = 3.0f;

void RoaringTask::Init()
{
	if (MawJLaygo* maw = dynamic_cast<MawJLaygo*>(m_BossCache))
	{
		FbxModelManager::ReservAnimation(ANIMETION_MODEL::MAWJLAYGO, "asset\\model\\MawJ_MutantRoaring60.fbx","Roaring");
	}
	m_CurrentTime = MAX_TIME_ANIMEATTACK;
}

NODE_STATUS RoaringTask::Update(const float& deltaTime)
{
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATUS::FAILURE;
	}

	BehaviorNode* node = m_BossCache->GetRunningNode();
	if (node != nullptr && node != this)
	{
		return NODE_STATUS::FAILURE;
	}

	// ”ÍˆÍ“à‚É“ü‚Á‚Ä‚¢‚½‚ç
	if (m_CurrentTime >= MAX_TIME_ANIMEATTACK && m_BossCache->GetCurrentRange() == RANGE::MIDDLE)
	{
		if(node == nullptr)
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
