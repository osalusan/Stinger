#include "titleCamera.h"
#include "manager/inputManager.h"

void TitleCamera::Init()
{
	GameObject::Init();
	// ƒJƒƒ‰‚ÌŒü‚«
	m_Target.z = 35.0f;
	m_Position.z = 0.0f;
	m_Position.y = 4.0f;
}
