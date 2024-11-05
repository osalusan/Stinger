#include "character.h"
#include "manager/animationModelManager.h"
#include "renderer/animationModelRenderer.h"

Character::Character(const ANIMETION_MODEL& model)
{
	m_Model = model;
}

void Character::Update(const float& deltaTime)
{
	// ˆÚ“®ˆ—
	MoveControl(deltaTime);

	m_Velocity.y *= deltaTime;
	m_Velocity.x *= deltaTime;

	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;

	// “–‚½‚è”»’èˆ—
	CollisionControl();

	// ‰ß‹ŽÀ•W‚Ì•Û‘¶
	m_RecordPosition = m_Position;
}

void Character::Draw()
{
	GameObject::Draw();

	if (AnimationModelRenderer* Model = AnimationModelManager::GetAnimationModel(m_Model))
	{
		//Model->Update(m_AnimationName.c_str(), m_AnimationFrame, m_NextanimationName.c_str(), m_AnimationFrame, m_BlendRatio);
		Model->Draw();
	}
}


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

