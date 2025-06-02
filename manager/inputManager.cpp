#include "manager/inputManager.h"

BYTE InputManager::m_OldKeyState[256] = { "" };
BYTE InputManager::m_KeyState[256] = { ""};


void InputManager::Init()
{
	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );
}

void InputManager::Update()
{
	// キーボードを取得できない場合
	if (!GetKeyboardState(m_KeyState)) return;

	memcpy( m_OldKeyState, m_KeyState, 256 );
}
//放した時
bool InputManager::GetKeyRelease(BYTE KeyCode)
{
	return (!(m_KeyState[KeyCode] & 0x80) && (m_OldKeyState[KeyCode] & 0x80));
}

//押している間
bool InputManager::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}

bool InputManager::GetMouseLeftPress()
{
	return (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
}
bool InputManager::GetMouseRightPress()
{
	return (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
}
