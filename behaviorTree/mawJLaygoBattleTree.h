#pragma once
#include "behaviorTree.h"

class MawJLaygoBattleTree final :public BehaviorTree
{
public:
	virtual void CreateTree(BossEnemy* boss);
};