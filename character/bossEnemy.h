#pragma once
#include "character/character.h"
#include <unordered_map>
// プレイヤーとの距離に応じた範囲 / エネミーごとに有効距離が異なる
enum class RANGE
{
	NONE = 0, // NONEはそいつにとって移動以外できることがない
	SHORT,
	MIDDLE,
	LONG
};
// 攻撃判定部位
enum class ATTACK_PARTS
{
	NONE = 0,
	BODY,
	ARM,
	RIGHT_ARM,
	LEFT_ARM,
	RIGHT_LEG,
	LEFT_LEG,
	ALL,
	MAX
};

class BehaviorTree;
class BehaviorNode;
class Player;
class Polygon2D;
// ボスエネミー用の基底クラス
class BossEnemy :public Character {
protected:
	BehaviorTree* m_Tree = nullptr;
	std::vector<BoxCollisionComponent*> m_BoxCollisionCaches = {};
	std::unordered_map<std::string, ATTACK_PARTS> m_PartsCategory = {};
	Player* m_PlayerCache = nullptr;		// ダメージ用
	Polygon2D* m_EnemyHpCache = nullptr;

	std::unordered_map<std::string, std::unordered_map<std::string, float>> m_EnemySkillData = {};

	ATTACK_PARTS m_CurrentAttackParts = ATTACK_PARTS::NONE;		// 現在有効な当たり判定の攻撃場所

	// パリィのパラメータ
	bool m_ParryRecoil = false;
	bool m_ParryPossibleAtk = false;

	// 攻撃のパラメータ
	float m_AttackDamage = 0.0f;								// 各タスクノードからダメージを格納してもらう
	bool m_AttackAccept = false;								// プレイヤーに攻撃した後、意図しない連続ヒットが無くなるように

	// ボスのパラメータ
	float m_CurStaminaValue = 0.0f;								// 現在のスタミナ / 技のバランスの為に実装

	// ボスのパラメータ / CSVからの読み込みで数値を反映
	float m_MaxStamina = 0.0f;									// スタミナの最大値
	float m_ShortRange = 0.0f;									// 近距離の範囲
	float m_MiddleRange = 0.0f;									// 中距離の範囲
	float m_LongRange = 0.0f;									// 遠距離の範囲
	float m_MinWaitTime = 0.0f;									// 待機時の最小硬直時間
	float m_MaxWaitTime = 0.0f;									// 待機時の最大硬直時間

	// ボスの現在の状態
	RANGE m_CurrentRange = RANGE::NONE;
	// 今実行中のノード
	BehaviorNode* m_RunningNodeCache = nullptr;

	virtual void CollisionControl()override = 0;		
	virtual void CustomCollisionInfo()override = 0;		
	virtual void AnimationControl()override = 0;		

	// 移動の所でビヘイビアツリーの更新を行う
	virtual void MoveControl(const float& deltaTime)override final;
	// ボスの変動パラメータを管理
	virtual void ParameterControl(const float& deltaTime)override final;

	// ビヘイビアツリーを使用する場合に子クラスで呼ぶ
	void CreateTree();
	// CSVからエネミーのデータを取得
	void EnemyDataLoadCSV(const std::string& filePath);
public:
	BossEnemy() = delete;
	BossEnemy(BehaviorTree* tree, const XMFLOAT3& pos);
	BossEnemy(BehaviorTree* tree, const XMFLOAT3& pos,const XMFLOAT3& scale);
	BossEnemy(BehaviorTree* tree, const XMFLOAT3& pos,const XMFLOAT3& scale, const XMFLOAT3& rot);
	virtual ~BossEnemy()override;
	virtual void TakeDamage(const float& atk)override;

	void AddVelocity(const XMFLOAT3& vel)
	{
		m_Velocity.x += vel.x;
		m_Velocity.y += vel.y;
		m_Velocity.z += vel.z;
	}

	// ターゲットの方へゆっくり向く
	void RotToTarget(GameObject* obj, const float& deltaTime);
	// ターゲットの方へ即時に向く
	void RotToTarget(GameObject* obj);
	// スタミナを消費する場合に使用
	bool UseStamina(const float& use);

	const BehaviorTree* GetBehaviourTree()const
	{
		return m_Tree;
	}	
	BehaviorNode* GetRunningNode()const
	{
		return m_RunningNodeCache;
	}
	const bool& GetParryRecoil()const
	{
		return m_ParryRecoil;
	}

	// パラメータ系のGet
	const float& GetShortRange()const
	{
		return m_ShortRange;
	}
	const float& GetMiddelRange()const
	{
		return m_MiddleRange;
	}
	const float& GetLongRange()const
	{
		return m_LongRange;
	}
	const float& GetStamina()const
	{
		return m_CurStaminaValue;
	}
	const float& GetMaxStamina()const
	{
		return m_MaxStamina;
	}
	const float& GetMinWaitTime()const
	{
		return m_MinWaitTime;
	}
	const float& GetMaxWaitTime()const
	{
		return m_MaxWaitTime;
	}
	const ATTACK_PARTS& GetCurrentAttackParts()const
	{
		return m_CurrentAttackParts;
	}

	// 現在の状態のGetとSet
	void SetCurrentRange(const RANGE& range)
	{
		m_CurrentRange = range;
	}
	const RANGE& GetCurrentRange()const
	{
		return m_CurrentRange;
	}
	void SetRunningNode(BehaviorNode* node);

	void SetParryRecoil(const bool& parry)
	{
		m_ParryRecoil = parry;
	}
	void SetAttackParts(const ATTACK_PARTS& parts)
	{
		m_CurrentAttackParts = parts;
	}
	void SetAttackDamage(const float& attack)
	{
		m_AttackDamage = m_Attack * attack;
	}
	void SetParryPossibleAtk(const bool& possible)
	{
		m_ParryPossibleAtk = possible;
	}
	void SetAttackAccept(const bool& accept)
	{
		m_AttackAccept = accept;
	}

	const std::unordered_map<std::string, float>& GetSkillData(const std::string& skillName)const
	{
		if (m_EnemySkillData.count(skillName) >= 1)
		{
			return m_EnemySkillData.at(skillName);
		}
		// テンポラリ警告用
		static const std::unordered_map<std::string, float> emptyMap = {};
		return emptyMap;
	}
};