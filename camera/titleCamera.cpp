#include "titleCamera.h"
#include "manager/inputManager.h"

void TitleCamera::Init()
{
	// �J�����̌���
	m_Target.z = 35.0f;
	m_Position.z = 0.0f;
	m_Position.y = 4.0f;
}

void TitleCamera::Uninit()
{
	// �������z�֐��̈׋����
}

void TitleCamera::Update()
{
	// TODO:�폜�\�� / �f�o�b�O�p
	if (InputManager::GetKeyPress('W'))
	{
		m_Position.z += 1.0f;
	}
	if (InputManager::GetKeyPress('S'))
	{
		m_Position.z -= 1.0f;
	}
	if (InputManager::GetKeyPress('F'))
	{
		m_Position.z = 0.0f;
		m_Position.y = 4.0f;
	}


	if (InputManager::GetKeyPress(VK_SPACE))
	{
		m_Position.y += 1.0f;
	}
	if (InputManager::GetKeyPress('Z'))
	{
		m_Position.y -= 1.0f;
	}
}
