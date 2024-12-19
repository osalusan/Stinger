#pragma once
#include "object/gameObject.h"
#include "component/collisionComponent.h"

// �O���錾
enum class ANIMETION_MODEL;

class MeshFiled;
class Character :public GameObject
{
protected:
	XMFLOAT3 m_Velocity= {};
	XMFLOAT3 m_RecordPosition = {};					// �ߋ����W
	ANIMETION_MODEL m_Model;						// ���f���{�� / �R���X�g���N�^�ŏ�����

	MeshFiled* m_MeshFiled = nullptr;

	// �d��
	bool m_EnableGravity = false;
	float m_GravityValue = 0.0f;

	int m_Health = 0;								// �̗�
	int m_Damage = 0;								// �^����_���[�W
	bool m_IsDead = false;							// ����ł���

	// ���o�I���t���O
	bool m_FinishPerformance = false;

	// �A�j���[�V�����p
	float m_AnimationTime = 0.0f;					// �A�j���[�V�����̃t���[��
	std::string m_AnimationName = "";				// �A�j���[�V�����̖��O
	std::string m_NextAnimationName = "";			// �u�����h�p�A���̃A�j���[�V�����̖��O
	float m_BlendRatio = 0.0f;						// �A�j���[�V�����u�����h�̐��l
	float m_BlendTimeValue = 0.0f;					// �ǂ̂��炢�u�����h�𑁂��s����
	float m_MaxAnimeTime = 0.0f;					// ���݂̃A�j���[�V�����̍ő厞��

	virtual void MoveControl(const float& deltaTime) = 0;
	virtual void CollisionControl() = 0;
	virtual void CustomCollisionInfo() = 0;
	virtual void AnimationControl() = 0;

	virtual void ParameterControl(const float& deltaTime);

	void TakeDamage(const int& atk);
	void ReservModel(const ANIMETION_MODEL& animeModel, const std::string& path);
	
public:
	Character();
	virtual void Update(const float& deltaTime)override final;
	virtual void Draw()override;
	
	void ChangeAnimation(const std::string& anime)
	{
		m_NextAnimationName = anime;
	}

	void SetAnimeTime(const float& time)
	{
		m_AnimationTime = time;
	}

	const ANIMETION_MODEL& GetAnimeModel()const
	{
		return m_Model;
	}
	const std::string& GetAnimeName()const
	{
		return m_AnimationName;
	}
	const std::string& GetNextAnimeName()const
	{
		return m_NextAnimationName;
	}
	const float& GetAnimeTime()const
	{
		return m_AnimationTime;
	}
	const float& GetBlendRatio()const
	{
		return m_BlendRatio;
	}
	const float& GetMaxAnimeTime()const
	{
		return m_MaxAnimeTime;
	}
	const int& GetHealth()const
	{
		return m_Health;
	}
};