#pragma once
#include "object/gameObject.h"
#include "component/collisionComponent.h"

// �O���錾
enum class ANIMETION_MODEL;

class Character :public GameObject
{
protected:
	XMFLOAT3 m_Velocity= {};
	XMFLOAT3 m_RecordPosition = {};					// �ߋ����W
	ANIMETION_MODEL m_Model;						// ���f���{�� / �R���X�g���N�^�ŏ�����

	// �d��
	bool m_EnableGravity = false;
	float m_GravityValue = 0.0f;

	int m_Health = 0;								// �̗�
	int m_Damage = 0;								// �^����_���[�W
	bool m_IsDead = false;							// ����ł���

	float m_AnimationTime = 0.0f;					// �A�j���[�V�����̃t���[��
	std::string m_AnimationName = "Idle";			// �A�j���[�V�����̖��O
	std::string m_NextAnimationName = "Idle";		// �u�����h�p�A���̃A�j���[�V�����̖��O
	float m_BlendRatio = 1.0f;						// �A�j���[�V�����u�����h�̐��l

	virtual void MoveControl(const float& deltaTime) = 0;
	virtual void CollisionControl() = 0;
	virtual void CustomCollisionInfo() = 0;

	void TakeDamage(const int& atk);
	void ReservModel(const ANIMETION_MODEL& animeModel, const std::string& path);
	
public:
	Character();
	virtual void Update(const float& deltaTime)override final;
	virtual void Draw()override;
	
	void ChangeAnimation(const std::string& anime);
};