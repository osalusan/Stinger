#pragma once
#include "bossEnemy.h"

class MawJLaygo final:public BossEnemy
{
private:
	virtual void CustomCollisionInfo()override;
	virtual void CollisionControl()override;

	virtual void CreateBehaviourTree()override final;
public:
	MawJLaygo() = delete;
	MawJLaygo(const XMFLOAT3& pos);
	virtual ~MawJLaygo()override;
	virtual void Init()override;
};