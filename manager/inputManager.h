#pragma once
#include "main/main.h"
// ゲーム内すべてのf入力を管理するクラス
class InputManager final
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];
public:
	static void Init();
	static void Update();

	// 簡易的な構造の為、同時に2つのキー入力が行われたら、Releseが正常に動作しない
	static bool GetKeyRelease(BYTE KeyCode);
	static bool GetKeyPress(BYTE KeyCode );

	static bool GetMouseLeftPress();
	static bool GetMouseRightPress();
	
};
