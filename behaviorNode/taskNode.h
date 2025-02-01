#pragma once
#include "behaviorNode.h"
#include <unordered_map>

// �O���錾
enum class ATTACK_PARTS;
class BossEnemy;
class Player;

class TaskNode : public BehaviorNode
{
private:
	float FindSkillData(const std::string& name);
	std::unordered_map<std::string, float> m_skillData = {};
protected:
	BossEnemy* m_BossCache = nullptr;
	Player* m_PlayerCache = nullptr;

	float m_CurrentTime = 0.0f;
	float m_MaxAnimTime = 0.0f;
	std::string m_AnimeName = "";

	bool m_UseDerivation = false;

	// �p�����[�^
	float m_DamageValue = 0.0f;
	float m_SpeedValue = 0.0f;
	float m_UseStaminaValue = 0.0f;
	float m_DerivationHealth = 0.0f;
	float m_AttackEnableTimeValue = 0.0f;
	float m_AttackDisableTimeValue = 1.0f;
	float m_BulletSpeed = 0.0f;
	bool m_ParryPossibleAtk = false;	// �p���B�����U���̏ꍇ�́ACSV�̕���0�ȊO�̐��l����͂���

	// �A�j���[�V�������g�p����ۂ́A�K���Ă�
	void ReserveAnimation(const std::string& fileName, const std::string& animationName);
	// �Z�̏ꍇ�͕K���Ă�
	void InitSkillData(const std::string& skillName);
	NODE_STATE UpdateChildren(const float& deltaTime);
	// ���Ȃ�������true���Ԃ�
	bool CheckRunningNode(BehaviorNode* currentNode);
	// �U�����g�p����ꍇ��Update�Ŏg�p
	void UseAttack(const ATTACK_PARTS& parts);
public:
	TaskNode() = delete;
	TaskNode(BossEnemy* boss, Player* player);
	virtual ~TaskNode()override;
	virtual void Init()override = 0;
	virtual NODE_STATE Update(const float& deltaTime) override;

	const float& GetMaxAnimTime()const
	{
		return m_MaxAnimTime;
	}

	// �p���B��_�E���ŋ����I��Task��ύX���鎞�Ɏg�p
	void CancelRunningTask();
};