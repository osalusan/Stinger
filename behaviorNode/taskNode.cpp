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
	m_AnimName = animationName;

	// TODO :追加予定 / 敵が追加されたらそのたびに追加
	if (MawJLaygo* maw = dynamic_cast<MawJLaygo*>(m_BossCache))
	{
		FbxModelManager::ReservAnimation(ANIMETION_MODEL::MAWJLAYGO, fileName, m_AnimName);
	}
}

void TaskNode::InitSkillData(const std::string& skillName)
{
	if (m_BossCache == nullptr) return;

	m_skillData = m_BossCache->GetSkillData(skillName);
	if (m_skillData.empty())return;

	m_DamageValue = FindSkillData("攻撃_倍率");
	m_SpeedValue = FindSkillData("速度_倍率");
	m_UseStaminaValue = FindSkillData("消費スタミナ_割合");
	m_AttackEnableTimeValue = FindSkillData("ダメージ開始時間_割合");
	m_AttackDisableTimeValue = FindSkillData("ダメージ終了時間_割合");
	if (FindSkillData("パリィ可能_攻撃") != 0.0f)
	{
		m_ParryPossibleAtk = true;
	}
	m_BulletSpeed = FindSkillData("弾速");
	m_AnimationSpeedValue = FindSkillData("アニメーション速度_倍率");
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
	// ダメージ発生
	if (m_CurrentTime >= m_MaxAnimTime * m_AttackEnableTimeValue && m_CurrentTime < m_MaxAnimTime * m_AttackDisableTimeValue)
	{
		m_BossCache->SetAttackParts(parts);
	}
}

int TaskNode::DerivationChance()
{
	if (m_Children.size() == 0)
	{
		return -1;
	}

	int totalChance = 0;
	for (const int& chance : m_ChildDerivChance)
	{
		totalChance += chance;
	}

	if (totalChance == 0)
	{
		return 0;
	}

	const int& randValue = rand() % totalChance;
	totalChance = 0;
	int loop = 0;
	for (const int& chance : m_ChildDerivChance)
	{
		totalChance += chance;
		if (totalChance >= randValue)
		{
			m_UseDerivNumber = loop;
			return loop;
		}
		loop++;
	}

	return -1;
}

NODE_STATE TaskNode::UpdateUseDerivationTask(const float& deltaTime)
{
	BehaviorNode* child = m_Children[m_UseDerivNumber];
	if (child != nullptr)
	{
		m_CurrentState = child->Update(deltaTime);
		child->SetCurrentState(m_CurrentState);
		return m_CurrentState;
	}
	return NODE_STATE::FAILURE;
}

void TaskNode::InitTask(const float& deltaTime)
{
	return;
}

void TaskNode::RunningTask(const float& deltaTime)
{
	return;
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
		return NODE_STATE::FAILURE;
	}
	// アニメーションしないタスクを一番最初にはじくように
	if (m_AnimName != "" && m_MaxAnimTime == 0.0f)
	{
		if (FbxModelRenderer* model = FbxModelManager::GetAnimationModel(m_BossCache->GetAnimeModel()))
		{
			m_MaxAnimTime = model->GetMaxAnimeTime(m_AnimName);
			m_CurrentTime = m_MaxAnimTime;
		}
	}
	m_BossCache->SetAttackDamage(m_DamageValue);
	m_BossCache->SetParryPossibleAtk(m_ParryPossibleAtk);

	// 攻撃タスクじゃなかったら
	if (!m_AttackTask)
	{
		return NODE_STATE::FAILURE;
	}

	BehaviorNode* node = m_BossCache->GetRunningNode();
	if (!CheckRunningNode(node))
	{
		return NODE_STATE::FAILURE;
	}

	// 初期化
	if (node == nullptr)
	{
		if (m_MaxAnimTime == 0.0f)
		{
			return NODE_STATE::FAILURE;
		}

		if (m_CurrentTime >= m_MaxAnimTime)
		{
			const float& maxStamina = m_BossCache->GetaMaxStamina();
			if (m_BossCache->UseStamina(maxStamina * m_UseStaminaValue))
			{
				InitTask(deltaTime);

				if (m_ChildDerivChance.size() > 0)
				{
					if (GetDerivationData(m_UseDerivNumber).Chance == 0 || GetDerivationData(m_UseDerivNumber).Chance == 100)
					{
						m_EnableDerivation = true;
					}
					// 確率
					if (rand() % 100 < GetDerivationData(m_UseDerivNumber).Chance)
					{
						m_EnableDerivation = true;
					}
				}
			}
		}
	}

	// 派生技の発生確認
	if (GetDerivationData().size() > 0)
	{
		if (m_EnableDerivation && m_CurrentTime > m_MaxAnimTime * GetDerivationData(m_UseDerivNumber).TransTimeValue)
		{
			if (m_BossCache->GetHealth() <= m_BossCache->GetMaxHealth() * GetDerivationData(m_UseDerivNumber).Health)
			{
				m_UseDerivation = true;
				m_EnableDerivation = false;
				m_CurrentTime = m_MaxAnimTime;
				m_BossCache->SetRunningNode(nullptr);
			}
		}
	}

	// 派生技の発生確認後に配置
	if (m_UseDerivation)
	{
		return UpdateUseDerivationTask(deltaTime);
	}

	if (m_CurrentTime < m_MaxAnimTime)
	{
		m_CurrentTime += deltaTime;
		m_BossCache->ChangeAnimation(m_AnimName);
		// 状態を保存
		m_BossCache->SetRunningNode(this);

		RunningTask(deltaTime);

		return NODE_STATE::RUNNING;
	}
	else
	{
		if (node == this)
		{
			// 状態を削除
			m_BossCache->SetRunningNode(nullptr);
			return NODE_STATE::SUCCESS;
		}
	}

	return NODE_STATE::FAILURE;
}

void TaskNode::CancelRunningTask()
{
	m_CurrentTime = m_MaxAnimTime;
}
