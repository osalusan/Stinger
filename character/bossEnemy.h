#pragma once
#include "character/character.h"

enum class RANGE
{
	NONE = 0, // NONEはそいつにとって移動以外できることがない
	SHROT,
	MIDDLE,
	LONG
};

class BehaviorTree;
class BehaviorNode;

class BossEnemy :public Character {
protected:
	BehaviorTree* m_Tree = nullptr;

	// ボスのパラメータ / 子クラスのInitで初期設定
	float m_MoveSpeed = 0.0f;
	float m_ShortRange = 0.0f;
	float m_MiddleRange = 0.0f;
	float m_LongRange = 0.0f;


	// ボスの現在の状態
	RANGE m_CurrentRange = RANGE::NONE;
	// 今実行中のノード
	BehaviorNode* m_RunningNodeCache = nullptr;


	virtual void CollisionControl()override = 0;
	virtual void CustomCollisionInfo()override = 0;
	virtual void AnimationControl()override = 0;
	// ビヘイビアツリーを作成
	virtual void CreateBehaviourTree() = 0;

	// 移動の所でビヘイビアツリーの制御
	virtual void MoveControl(const float& deltaTime)override final;
public:
	virtual ~BossEnemy()override;
	virtual void Init()override;

	void AddVelocity(const XMFLOAT3& vel)
	{
		m_Velocity.x += vel.x;
		m_Velocity.y += vel.y;
		m_Velocity.z += vel.z;
	}

	void AddRotation(const XMFLOAT3& rot)
	{
		m_Rotation.x += rot.x;
		m_Rotation.y += rot.y;
		m_Rotation.z += rot.z;
	}
	const BehaviorTree* GetBehaviourTree()const
	{
		return m_Tree;
	}	
	BehaviorNode* GetRunningNode()const
	{
		return m_RunningNodeCache;
	}

	// パラメータ系のGet
	const float& GetMoveSpeed()const
	{
		return m_MoveSpeed;
	}	
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

	// 現在の状態のGetとSet
	void SetCurrentRange(const RANGE& range)
	{
		m_CurrentRange = range;
	}
	const RANGE& GetCurrentRange()const
	{
		return m_CurrentRange;
	}
	void SetRunningNode(BehaviorNode* node)
	{
		m_RunningNodeCache = node;
	}
};