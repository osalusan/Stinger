#pragma once
#include "behaviorTree.h"

class MawJLaygoTitleTree final :public BehaviorTree
{
public:
	virtual void CreateTree(BossEnemy* boss);
};