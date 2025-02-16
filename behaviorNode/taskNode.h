#pragma once
#include "behaviorNode.h"
#include <unordered_map>

// 前方宣言
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

	bool m_UseDerivation = false;					// 派生技の初期設定をしたか
	bool m_EnableDerivation = false;				// 派生技が使えるか
	int m_UseDerivNumber = 0;                       // 使用する派生技の番号
	bool m_AttackTask = false;						// 攻撃に使用しているタスク

	// パラメータ
	float m_DamageValue = 0.0f;						// ダメージ倍率
	float m_SpeedValue = 0.0f;						// 速度倍率
	float m_UseStaminaValue = 0.0f;					// 使用スタミナ倍率
	float m_AttackEnableTimeValue = 0.0f;			// 攻撃判定開始時間
	float m_AttackDisableTimeValue = 1.0f;			// 攻撃判定終了時間
	float m_BulletSpeed = 0.0f;						// 弾速
	float m_AnimationSpeedValue = 1.0f;				// アニメーション速度倍率
	bool m_ParryPossibleAtk = false;	// パリィされる攻撃の場合は、CSVの方に0以外の数値を入力する

	// アニメーションを使用する際は、必ず呼ぶ
	void ReserveAnimation(const std::string& fileName, const std::string& animationName);
	// 技の場合は必ず呼ぶ
	void InitSkillData(const std::string& skillName);
	NODE_STATE UpdateChildren(const float& deltaTime);
	// 攻撃を使用する場合にUpdateで使用
	void UseAttack(const ATTACK_PARTS& parts);

	// エラー時に -1 を返す / 成功時に m_UseDerivNumber へ格納
	int DerivationChance();
	// 
	NODE_STATE UpdateUseDerivationTask(const float& deltaTime);

protected:
	virtual void InitTask(const float& deltaTime);
	virtual void RunningTask(const float& deltaTime);

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

	// パリィやダウンで強制的にTaskを変更する時に使用
	void CancelRunningTask();
};