#pragma once
#include "behaviorNode.h"
#include <unordered_map>

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

	// �p�����[�^
	float m_DamageValue = 0.0f;
	float m_SpeedValue = 0.0f;
	float m_UseStaminaValue = 0.0f;

	// �A�j���[�V�������g�p����ۂ́A�K���Ă�
	void ReserveAnimation(const std::string& fileName, const std::string& animationName);
	// �Z�̏ꍇ�͕K���Ă�
	void InitSkillData(const std::string& skillName);
public:
	TaskNode() = delete;
	TaskNode(BossEnemy* boss, Player* player);
	virtual ~TaskNode()override;
	virtual void Init()override = 0;
	virtual NODE_STATE Update(const float& deltaTime) override;
};