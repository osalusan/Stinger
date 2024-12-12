#include "character.h"
#include "manager/fbxModelManager.h"
#include "renderer/fbxModelRenderer.h"
#include "component/collisionComponent.h"

constexpr float DEFAULT_BLEND_VALUE = 8.0f;

// ------------------------- protected -------------------------
void Character::TakeDamage(const int& atk)
{
	if (atk <= 0 || m_Health <= 0) return;

	m_Health -= atk;

	if (m_Health <= 0)
	{
		m_Health = 0;
		m_IsDead = true;
	}

}

void Character::ReservModel(const ANIMETION_MODEL& animeModel, const std::string& path)
{
	FbxModelManager::ReservModel(animeModel, path);
	m_Model = animeModel;
}

// ------------------------- public -------------------------
Character::Character()
{
	m_Model = ANIMETION_MODEL::MAX;
	m_BlendTimeValue = DEFAULT_BLEND_VALUE;
	LoadShader("cso\\skinningVS.cso", "cso\\skinningPS.cso");
}

void Character::Update(const float& deltaTime)
{
	GameObject::Update(deltaTime);

	m_Velocity.x = 0.0f;
	m_Velocity.z = 0.0f;

	if (m_EnableGravity)
	{
		m_Velocity.y -= m_GravityValue * deltaTime;
	}
	else
	{
		m_Velocity.y = 0.0f;
	}
	// �ړ�����
	MoveControl(deltaTime);
	// �A�j���[�V��������
	AnimationControl();

	// �A�j���[�V�����̃u�����h����
	m_AnimationTime += deltaTime;
	if (m_AnimationName != m_NextAnimationName && m_BlendRatio < 1.0f) 
	{ 
		m_BlendRatio += deltaTime * m_BlendTimeValue;
	}
	else if(m_AnimationName != m_NextAnimationName && m_BlendRatio > 1.0f)
	{
		m_AnimationName = m_NextAnimationName;
		m_AnimationTime = 0.0f;
		m_BlendRatio = 0.0f;
	}
	if (m_AnimationName == m_NextAnimationName && m_BlendRatio > 0.0f)
	{ 
		m_BlendRatio -= deltaTime * m_BlendTimeValue;
	}
	else if (m_AnimationName == m_NextAnimationName && m_BlendRatio < 0.0f)
	{
		m_BlendRatio = 0.0f;
	}

	// ���x����
	m_Velocity.x *= deltaTime;
	m_Velocity.y *= deltaTime;
	m_Velocity.z *= deltaTime;

	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;

	// �����蔻�菈���̑O�� / ����̂�
	if (m_ModelCenter.x == 0 && m_ModelCenter.y == 0 && m_ModelCenter.z == 0)
	{
		if (const FbxModelRenderer* model = FbxModelManager::GetAnimationModel(m_Model))
		{
			m_ModelCenter = model->GetCenter();
			m_ModelScale = model->GetScale();
		}
	}
	// �����蔻��p�ϐ��̐ݒ� / �ړ���
	CustomCollisionInfo();
	// �����蔻�菈��
	CollisionControl();
	// �����蔻��p�ϐ��̐ݒ� / �����蔻���
	CustomCollisionInfo();
	// �ߋ����W�̕ۑ�
	m_RecordPosition = m_Position;
}

void Character::Draw()
{
	GameObject::Draw();

	if (FbxModelRenderer* model = FbxModelManager::GetAnimationModel(m_Model))
	{
		if (m_AnimationName == m_NextAnimationName)
		{
			model->Update(m_AnimationName.c_str(), m_AnimationTime);
		}
		else if(m_AnimationName != m_NextAnimationName)
		{
			model->Update(m_AnimationName.c_str(), m_AnimationTime, m_NextAnimationName.c_str(), 0.0f, m_BlendRatio);
		}
		
		model->Draw();
	}
}