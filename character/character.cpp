#include "character.h"
#include "manager/fbxModelManager.h"
#include "renderer/fbxModelRenderer.h"
#include "component/collisionComponent.h"

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
}

Character::~Character()
{
	delete m_Collision;
	m_Collision = nullptr;
}

void Character::Update(const float& deltaTime)
{

	m_Velocity.x = 0.0f;
	m_Velocity.z = 0.0f;

	if (m_EnableGravity)
	{
		m_Velocity.y += m_GravityValue;
	}
	else
	{
		m_Velocity.y = 0.0f;
	}
	// ˆÚ“®ˆ—
	MoveControl(deltaTime);

	m_Velocity.y *= deltaTime;
	m_Velocity.x *= deltaTime;
	m_Velocity.z *= deltaTime;

	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;

	// “–‚½‚è”»’èˆ—‚Ì‘O‚É / ‰‰ñ‚Ì‚Ý
	if (m_ModelCenter.x == 0 && m_ModelCenter.y == 0 && m_ModelCenter.z == 0)
	{
		if (FbxModelRenderer* model = FbxModelManager::GetAnimationModel(m_Model))
		{
			m_ModelCenter = model->GetCenter();
			m_ModelScale = model->GetScale();
		}
	}

	if (m_Collision != nullptr)
	{
		m_Collision->Update();
	}

	// “–‚½‚è”»’èˆ—
	CollisionControl();

	// ‰ß‹ŽÀ•W‚Ì•Û‘¶
	m_RecordPosition = m_Position;
}

void Character::Draw()
{
	GameObject::Draw();

	if (FbxModelRenderer* model = FbxModelManager::GetAnimationModel(m_Model))
	{
		//Model->Update(m_AnimationName.c_str(), m_AnimationFrame, m_NextanimationName.c_str(), m_AnimationFrame, m_BlendRatio);
		model->Draw();
	}

#if _DEBUG
	if (m_Collision != nullptr)
	{
		m_Collision->Draw();
	}
#endif // _DEBUG
}




