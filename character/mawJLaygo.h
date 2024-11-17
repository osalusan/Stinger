#pragma once
#include "bossEnemy.h"

class MawJLaygo :public BossEnemy
{
private:
	virtual void MoveControl(const float& deltaTime)override;
	virtual void CustomCollisionInfo()override;
	virtual void CollisionControl()override;
public:
	MawJLaygo() = delete;
	MawJLaygo(const XMFLOAT3& pos);
	~MawJLaygo();
	virtual void Init()override;
};