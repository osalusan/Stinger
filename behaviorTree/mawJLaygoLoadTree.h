#pragma once
#include "behaviorTree.h"

class MawJLaygoLoadTree final :public BehaviorTree
{
public:
	virtual void CreateTree(BossEnemy* boss);
};