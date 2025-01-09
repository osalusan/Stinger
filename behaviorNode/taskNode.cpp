#include "taskNode.h"
#include "character/bossEnemy.h"
#include "renderer/fbxModelRenderer.h"
#include "manager/fbxModelManager.h"
#include "character/mawJLaygo.h"

// ---------------------------- protected ----------------------------
void TaskNode::ReserveAnimation(const std::string& fileName, const std::string& animationName)
{
	m_AnimeName = animationName;

	// TODO :追加予定 / 敵が追加されたらそのたびに追加
	if (MawJLaygo* maw = dynamic_cast<MawJLaygo*>(m_BossCache))
	{
		FbxModelManager::ReservAnimation(ANIMETION_MODEL::MAWJLAYGO, fileName, m_AnimeName);
	}
}

void TaskNode::InitSkillData(const std::string& skillName)
{
	if (m_BossCache == nullptr) return;

	const std::vector<float>& skillData = m_BossCache->GetSkillData(skillName);
	if (skillData.empty())return;

	m_DamageValue = skillData[0];
	m_SpeedValue = skillData[1];
	m_UseStaminaValue = skillData[2];
}

// ---------------------------- public ----------------------------
TaskNode::TaskNode(BossEnemy* boss, Player* player)
{
	if (m_BossCache == nullptr && boss != nullptr)
	{
		m_BossCache = boss;
	}
	if (m_PlayerCache == nullptr && player != nullptr)
	{
		m_PlayerCache = player;
	}
}

TaskNode::~TaskNode()
{
	m_PlayerCache = nullptr;
	m_BossCache = nullptr;
}

NODE_STATE TaskNode::Update(const float& deltaTime)
{
	// アニメーションしないタスクを一番最初にはじくように
	if (m_AnimeName != "" && m_MaxAnimTime == 0.0f && m_BossCache != nullptr)
	{
		if (FbxModelRenderer* model = FbxModelManager::GetAnimationModel(m_BossCache->GetAnimeModel()))
		{
			m_MaxAnimTime = model->GetMaxAnimeTime(m_AnimeName);
			m_CurrentTime = m_MaxAnimTime;
		}
	}
	return NODE_STATE();
}
