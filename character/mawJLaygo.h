#pragma once
#include "bossEnemy.h"

class MawJLaygo :public BossEnemy
{
private:
	virtual void CustomCollisionInfo()override;
	virtual void CollisionControl()override;

	virtual void CreateBehaviourTree()override final;
public:
	MawJLaygo() = delete;
	MawJLaygo(const XMFLOAT3& pos);
	~MawJLaygo();
	virtual void Init()override;
};