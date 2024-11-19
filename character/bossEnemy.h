#pragma once
#include "character/character.h"

class BehaviourTree;

class BossEnemy :public Character {
protected:
	BehaviourTree* m_Tree = nullptr;

	// �{�X�̃p�����[�^
	float m_MoveSpeed = {};

	// �r�w�C�r�A�c���[���쐬
	virtual void CreateBehaviourTree() = 0;

	// �ړ��̏��Ńr�w�C�r�A�c���[�̐���
	virtual void MoveControl(const float& deltaTime)override final;
public:
	virtual ~BossEnemy()override;
	virtual void Init()override;

	void AddVelocity(const XMFLOAT3& vel)
	{
		m_Velocity.x += vel.x;
		m_Velocity.y += vel.y;
		m_Velocity.z += vel.z;
	}

	const float& GetMoveSpeed()const
	{
		return m_MoveSpeed;
	}

	const BehaviourTree* GetBehaviourTree()const
	{
		return m_Tree;
	}
};