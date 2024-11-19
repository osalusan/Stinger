#pragma once
#include "character/character.h"

class BehaviourTree;

class BossEnemy :public Character {
protected:
	BehaviourTree* m_Tree = nullptr;

	// �r�w�C�r�A�c���[���쐬
	virtual void CreateBehaviourTree() = 0;

	// �ړ��̏��Ńr�w�C�r�A�c���[�̐���
	virtual void MoveControl(const float& deltaTime)override final;
public:
	virtual ~BossEnemy()override;
	virtual void Init()override;


	const BehaviourTree* GetBehaviourTree()const
	{
		return m_Tree;
	}
};