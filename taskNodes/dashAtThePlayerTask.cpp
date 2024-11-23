#include "dashAtThePlayerTask.h"
#include "character/player.h"
#include "character/mawJLaygo.h"
#include "manager/fbxModelManager.h"

void DashAtThePlayerTask::Init()
{
	// âΩÇ‡ê›íËÇ≥ÇÍÇƒÇ¢Ç»Ç¢éû
	if (m_Range == 0.0f)
	{
		m_Range = 10.0f;
	}
	if (MawJLaygo* maw = dynamic_cast<MawJLaygo*>(m_BossCache))
	{
		FbxModelManager::ReservAnimation(ANIMETION_MODEL::MAWJLAYGO, "asset\\model\\MawJ_UnarmedWalkForward60.fbx", "Running");
	}
	
}

NODE_STATUS DashAtThePlayerTask::Update(const float& deltaTime)
{
	if (m_BossCache == nullptr || m_PlayerCache == nullptr)
	{
		return NODE_STATUS::FAILURE;
	}

	// îÕàÕì‡Ç…ì¸Ç¡ÇƒÇ¢ÇΩÇÁ
	if (m_BossCache->GetCurrentRange() == RANGE::SHROT)
	{
		return NODE_STATUS::SUCCESS;
	}

	// à⁄ìÆèàóù
	const XMFLOAT3& playerPos = m_PlayerCache->GetPos();
	const XMFLOAT3& direction = m_BossCache->GetTargetDirection(playerPos);

	const float& moveSpeed = m_BossCache->GetMoveSpeed() * deltaTime;

	m_BossCache->AddVelocity({ direction.x * moveSpeed ,0.0f,direction.z * moveSpeed });

	m_BossCache->ChangeAnimation("Running");

	return NODE_STATUS::FAILURE;
}
