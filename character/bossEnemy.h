#pragma once
#include "character/character.h"

enum class RANGE
{
	NONE = 0, // NONEはそいつにとって移動以外できることがない
	SHROT,
	MIDDLE,
	LONG
};

class BehaviourTree;

class BossEnemy :public Character {
protected:
	BehaviourTree* m_Tree = nullptr;

	// ボスのパラメータ / 子クラスのInitで初期設定
	float m_MoveSpeed = 0.0f;
	float m_ShortRange = 0.0f;
	float m_MiddleRange = 0.0f;
	float m_LongRange = 0.0f;


	// ボスの現在の状態
	RANGE m_CurrentRange = RANGE::NONE;

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
	const BehaviourTree* GetBehaviourTree()const
	{
		return m_Tree;
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
};