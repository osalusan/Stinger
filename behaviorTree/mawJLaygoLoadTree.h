#pragma once
#include "behaviorTree.h"
// ロード画面で使用するエネミーの頭脳
class MawJLaygoLoadTree final :public BehaviorTree
{
public:
	virtual void CreateTree(BossEnemy* boss);
};