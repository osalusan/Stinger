#pragma once
#include "object/gameObject.h"

// �O���錾
enum class ANIMETION_MODEL;

class Character :public GameObject
{
protected:
	XMFLOAT3 m_Velocity= {};
	XMFLOAT3 m_RecordPosition = {};					// �ߋ����W
	ANIMETION_MODEL m_Model;						// ���f���{�� / �R���X�g���N�^�ŏ�����

	int m_Health = 0;								// �̗�
	int m_Damage = 0;								// �^����_���[�W
	bool m_IsDead = false;							// ����ł���

	unsigned int m_AnimationFrame = 0;				// �A�j���[�V�����̃t���[��
	std::string m_AnimationName = "Idle";			// �A�j���[�V�����̖��O
	std::string m_NextanimationName = "Idle";		// �u�����h�p�A���̃A�j���[�V�����̖��O
	float m_BlendRatio = 1.0f;						// �A�j���[�V�����u�����h�̐��l

	virtual void MoveControl(const float& deltaTime) = 0;
	virtual void CollisionControl() = 0;

	void TakeDamage(const int& atk);
	
public:
	Character() = delete;
	Character(const ANIMETION_MODEL& model);
	virtual void Update(const float& deltaTime)override final;
	virtual void Draw()override final;
};