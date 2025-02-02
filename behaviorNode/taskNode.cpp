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

	// TODO :�ǉ��\�� / �G���ǉ����ꂽ�炻�̂��тɒǉ�
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

	m_DamageValue = FindSkillData("�U��_�{��");
	m_SpeedValue = FindSkillData("���x_�{��");
	m_UseStaminaValue = FindSkillData("����X�^�~�i_����");
	m_DerivationHealth = FindSkillData("�h���Z�̔����̗�_����");
	m_DerivationTimeValue = FindSkillData("�h���Z�ڍs����_����");
	m_DerivationChance = FindSkillData("�h���Z����_�m��");
	m_AttackEnableTimeValue = FindSkillData("�_���[�W�J�n����_����");
	m_AttackDisableTimeValue = FindSkillData("�_���[�W�I������_����");
	if (FindSkillData("�p���B�\_�U��") != 0.0f)
	{
		m_ParryPossibleAtk = true;
	}
	m_BulletSpeed = FindSkillData("�e��");
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

void TaskNode::UseAttack(const ATTACK_PARTS& parts)
{
	if (m_MaxAnimTime == 0.0f || m_BossCache == nullptr) return;
	// �_���[�W����
	if (m_CurrentTime >= m_MaxAnimTime * m_AttackEnableTimeValue && m_CurrentTime < m_MaxAnimTime * m_AttackDisableTimeValue)
	{
		m_BossCache->SetAttackParts(parts);
	}
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
	// �A�j���[�V�������Ȃ��^�X�N����ԍŏ��ɂ͂����悤��
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

void TaskNode::CancelRunningTask()
{
	m_CurrentTime = m_MaxAnimTime;
}
