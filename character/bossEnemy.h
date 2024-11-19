#pragma once
#include "character/character.h"

class BehaviourTree;

class BossEnemy :public Character {
protected:
	BehaviourTree* m_Tree = nullptr;

	// ビヘイビアツリーを作成
	virtual void CreateBehaviourTree() = 0;

	// 移動の所でビヘイビアツリーの制御
	virtual void MoveControl(const float& deltaTime)override final;
public:
	virtual ~BossEnemy()override;
	virtual void Init()override;


	const BehaviourTree* GetBehaviourTree()const
	{
		return m_Tree;
	}
};