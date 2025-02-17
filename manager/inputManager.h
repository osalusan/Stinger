#pragma once
#include "main/main.h"

class InputManager final
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];
public:
	static void Init();
	static void Update();

	static bool GetKeyRelease(BYTE KeyCode);
	static bool GetKeyPress( BYTE KeyCode );
	static bool GetKeyTrigger( BYTE KeyCode );

	static bool GetMouseLeftPress();
	static bool GetMouseRightPress();
	
};
