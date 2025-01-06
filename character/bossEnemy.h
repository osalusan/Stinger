#pragma once
#include "character/character.h"

enum class RANGE
{
	NONE = 0, // NONE�͂����ɂƂ��Ĉړ��ȊO�ł��邱�Ƃ��Ȃ�
	SHROT,
	MIDDLE,
	LONG
};


class BehaviorTree;
class BehaviorNode;

class BossEnemy :public Character {
protected:
	BehaviorTree* m_Tree = nullptr;
	std::vector<BoxCollisionComponent*> m_BoxCollisionCaches = {};

	std::vector<std::vector<float>> m_EnemySkillData = {};

	float m_StaminaValue = 0.0f;

	// �{�X�̃p�����[�^ / �q�N���X��Init�ŏ����ݒ�
	int   m_MaxHealth = 0;
	float m_MaxStamina = 0.0f;
	float m_MoveSpeed = 0.0f;
	float m_ShortRange = 0.0f;
	float m_MiddleRange = 0.0f;
	float m_LongRange = 0.0f;
	float m_AttackPower = 0.0f;
	float m_RotSpeed = 0.0f;
	float m_MinWaitTime = 0.0f;
	float m_MaxWaitTime = 0.0f;

	// �{�X�̌��݂̏��
	RANGE m_CurrentRange = RANGE::NONE;
	// �����s���̃m�[�h
	BehaviorNode* m_RunningNodeCache = nullptr;

	virtual void CollisionControl()override = 0;
	virtual void CustomCollisionInfo()override = 0;
	virtual void AnimationControl()override = 0;

	// �ړ��̏��Ńr�w�C�r�A�c���[�̐���
	virtual void MoveControl(const float& deltaTime)override final;
	virtual void ParameterControl(const float& deltaTime)override final;

	// �r�w�C�r�A�c���[���g�p����ꍇ�Ɏq�N���X�ŌĂ�
	void CreateTree();
	// CSV����G�l�~�[�̃f�[�^���擾
	void EnemyDataLoadCSV(const std::string& filePath);
public:
	BossEnemy() = delete;
	BossEnemy(BehaviorTree* tree, const XMFLOAT3& pos);
	BossEnemy(BehaviorTree* tree, const XMFLOAT3& pos,const XMFLOAT3& scale);
	BossEnemy(BehaviorTree* tree, const XMFLOAT3& pos,const XMFLOAT3& scale, const XMFLOAT3& rot);
	virtual ~BossEnemy()override;

	void AddVelocity(const XMFLOAT3& vel)
	{
		m_Velocity.x += vel.x;
		m_Velocity.y += vel.y;
		m_Velocity.z += vel.z;
	}

	void RotToTarget(GameObject* obj, const float& deltaTime);
	
	bool UseStamina(const float& use);

	const BehaviorTree* GetBehaviourTree()const
	{
		return m_Tree;
	}	
	BehaviorNode* GetRunningNode()const
	{
		return m_RunningNodeCache;
	}

	// �p�����[�^�n��Get
	const float& GetMoveSpeed()const
	{
		return m_MoveSpeed;
	}	
	const float& GetShortRange()const
	{
		return m_ShortRange;
	}
	const float& GetMiddelRange()const
	{
		return m_MiddleRange;
	}
	const float& GetLongRange()const
	{
		return m_LongRange;
	}
	const float& GetStamina()const
	{
		return m_StaminaValue;
	}
	const float& GetaMaxStamina()const
	{
		return m_MaxStamina;
	}
	const float& GetMinWaitTime()const
	{
		return m_MinWaitTime;
	}
	const float& GetMaxWaitTime()const
	{
		return m_MaxWaitTime;
	}

	// ���݂̏�Ԃ�Get��Set
	void SetCurrentRange(const RANGE& range)
	{
		m_CurrentRange = range;
	}
	const RANGE& GetCurrentRange()const
	{
		return m_CurrentRange;
	}
	void SetRunningNode(BehaviorNode* node)
	{
		m_RunningNodeCache = node;
	}

	const std::vector<float>& GetSkillData(const int& skillNo)const
	{
		return m_EnemySkillData[skillNo];
	}
};