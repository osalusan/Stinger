#pragma once
#include "behaviorNode.h"
#include <string>

class BossEnemy;
class Player;

class TaskNode : public BehaviorNode
{
protected:
	BossEnemy* m_BossCache = nullptr;
	Player* m_PlayerCache = nullptr;

	float m_CurrentTime = 0.0f;
	float m_MaxAnimTime = 0.0f;
	std::string m_AnimeName = "";

	// パラメータ
	float m_DamageValue = 0.0f;
	float m_SpeedValue = 0.0f;
	float m_UseStaminaValue = 0.0f;

	// アニメーションを使用する際は、必ず呼ぶ
	void ReserveAnimation(const std::string& fileName, const std::string& animationName);
	// 技の場合は必ず呼ぶ
	void InitSkillData(const int& layer);
public:
	TaskNode() = delete;
	TaskNode(BossEnemy* boss, Player* player);
	virtual ~TaskNode()override;
	virtual void Init()override = 0;
	virtual NODE_STATUS Update(const float& deltaTime) override;
};