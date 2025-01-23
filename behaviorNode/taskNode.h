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

	bool m_UseDerivation = false;

	// パラメータ
	float m_DamageValue = 0.0f;
	float m_SpeedValue = 0.0f;
	float m_UseStaminaValue = 0.0f;
	float m_DerivationHealth = 0.0f;
	bool m_ParryPossibleAtk = false;	// パリィされる攻撃の場合は、CSVの方に0以外の数値を入力する

	// アニメーションを使用する際は、必ず呼ぶ
	void ReserveAnimation(const std::string& fileName, const std::string& animationName);
	// 技の場合は必ず呼ぶ
	void InitSkillData(const std::string& skillName);
	NODE_STATE UpdateChildren(const float& deltaTime);
	// 問題なかったらtrueが返る
	bool CheckRunningNode(BehaviorNode* currentNode);
public:
	TaskNode() = delete;
	TaskNode(BossEnemy* boss, Player* player);
	virtual ~TaskNode()override;
	virtual void Init()override = 0;
	virtual NODE_STATE Update(const float& deltaTime) override;
};