#pragma once
#include "character/character.h"

class BehaviourTree;

class BossEnemy :public Character {
protected:
	BehaviourTree* m_Tree = nullptr;

	// ‰Šú‰»‚ÅŒÄ‚Ô
	virtual void CreateTree(BehaviourTree* tree);
public:
	virtual ~BossEnemy();
};