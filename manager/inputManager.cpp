#include "manager/inputManager.h"


BYTE InputManager::m_OldKeyState[256];
BYTE InputManager::m_KeyState[256];


void InputManager::Init()
{

	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );

}

void InputManager::Uninit()
{


}

void InputManager::Update()
{

	memcpy( m_OldKeyState, m_KeyState, 256 );

	//GetKeyboardState( m_KeyState );

}
//•ú‚µ‚½
bool InputManager::GetKeyRelease(BYTE KeyCode)
{
	return (!(m_KeyState[KeyCode] & 0x80) && (m_OldKeyState[KeyCode] & 0x80));
}

//‰Ÿ‚µ‚Ä‚¢‚éŠÔ
bool InputManager::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}
//‰Ÿ‚µ‚½••ú‚µ‚½uŠÔ
bool InputManager::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}
