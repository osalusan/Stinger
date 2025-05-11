#pragma once
#include "behaviorTree.h"
// バトルで使用するエネミーの頭脳
class MawJLaygoBattleTree final :public BehaviorTree
{
public:
	virtual void CreateTree(BossEnemy* boss);
};