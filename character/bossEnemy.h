#pragma once
#include "character/character.h"

class BehaviourTree;

class BossEnemy :public Character {
protected:
	BehaviourTree* m_Tree = nullptr;

	// ボスのパラメータ
	float m_MoveSpeed = {};

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

	const float& GetMoveSpeed()const
	{
		return m_MoveSpeed;
	}

	const BehaviourTree* GetBehaviourTree()const
	{
		return m_Tree;
	}
};