#include "taskNode.h"
#include "character/bossEnemy.h"
#include "renderer/fbxModelRenderer.h"
#include "manager/fbxModelManager.h"
#include "character/mawJLaygo.h"

// ---------------------------- private ----------------------------
float TaskNode::FindSkillData(const std::string& name)
{
	if (m_skillData.count(name) >= 1)
	{
		return m_skillData.at(name);
	}
	else
	{
		return 0.0f;
	}
}

// ---------------------------- protected ----------------------------
void TaskNode::ReserveAnimation(const std::string& fileName, const std::string& animationName)
{
	m_AnimeName = animationName;

	// TODO :’Ç‰Á—\’è / “G‚ª’Ç‰Á‚³‚ê‚½‚ç‚»‚Ì‚½‚Ñ‚É’Ç‰Á
	if (MawJLaygo* maw = dynamic_cast<MawJLaygo*>(m_BossCache))
	{
		FbxModelManager::ReservAnimation(ANIMETION_MODEL::MAWJLAYGO, fileName, m_AnimeName);
	}
}

void TaskNode::InitSkillData(const std::string& skillName)
{
	if (m_BossCache == nullptr) return;

	m_skillData = m_BossCache->GetSkillData(skillName);
	if (m_skillData.empty())return;

	m_DamageValue = FindSkillData("UŒ‚”{—¦");
	m_SpeedValue = FindSkillData("‘¬“x”{—¦");
	m_UseStaminaValue = FindSkillData("Á”ïƒXƒ^ƒ~ƒiŠ„‡");
	m_DerivationHealth = FindSkillData("”h¶‹Z‚Ì”­¶‘Ì—ÍŠ„‡");
	if (FindSkillData("ƒpƒŠƒB‰Â”\UŒ‚") != 0.0f)
	{
		m_ParryPossibleAtk = true;
	}
}

NODE_STATE TaskNode::UpdateChildren(const float& deltaTime)
{
	if (m_BossCache == nullptr) return NODE_STATE::FAILURE;

	for (BehaviorNode* child : m_Children)
	{
		if (child == nullptr) continue;

		NODE_STATE state = child->Update(deltaTime);
		child->SetCurrentState(state);
		m_CurrentState = state;
		return state;
	}
	return NODE_STATE::FAILURE;
}

bool TaskNode::CheckRunningNode(BehaviorNode* currentNode)
{
	bool hit = true;
	if (currentNode != nullptr && currentNode != this)
	{
		hit = false;
		for (BehaviorNode* child : m_Children)
		{
			if (child == currentNode)
			{
				hit = true;
			}
		}
	}
	return hit;
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
	if (m_BossCache == nullptr)
	{
		return NODE_STATE();
	}
	// ƒAƒjƒ[ƒVƒ‡ƒ“‚µ‚È‚¢ƒ^ƒXƒN‚ðˆê”ÔÅ‰‚É‚Í‚¶‚­‚æ‚¤‚É
	if (m_AnimeName != "" && m_MaxAnimTime == 0.0f)
	{
		if (FbxModelRenderer* model = FbxModelManager::GetAnimationModel(m_BossCache->GetAnimeModel()))
		{
			m_MaxAnimTime = model->GetMaxAnimeTime(m_AnimeName);
			m_CurrentTime = m_MaxAnimTime;
		}
	}

	m_BossCache->SetAttackDamage(m_DamageValue);
	m_BossCache->SetParryPossibleAtk(m_ParryPossibleAtk);
	return NODE_STATE();
}
