#pragma once
#include "behaviorTree.h"
// タイトル画面で使用するエネミーの頭脳
class MawJLaygoTitleTree final :public BehaviorTree
{
public:
	virtual void CreateTree(BossEnemy* boss);
};