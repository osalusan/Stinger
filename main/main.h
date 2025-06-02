#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NOMINMAX
// Windows.h内でwinsockを呼んでしまうので、先にwinsock2を呼ぶ
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <assert.h>
#include <functional>

#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment(lib, "ws2_32.lib")

#include <DirectXMath.h>
using namespace DirectX;


#include "externalLibrary/directXTex/DirectXTex.h"

// /IGNORE:4099 を使用してDirectXtex_Debug.pbdが無くても警告が出ないようにしている
#if _DEBUG
#pragma comment(lib,"externalLibrary/directXTex/DirectXTex_Debug.lib")
#else
#pragma comment(lib,"externalLibrary/directXTex/DirectXTex_Release.lib")
#endif

#pragma comment (lib, "winmm.lib")

#if _DEBUG
//#define SCREEN_WIDTH	(1280)
//#define SCREEN_HEIGHT	(720)
#define SCREEN_WIDTH	(1920)
#define SCREEN_HEIGHT	(1080)
#else
#define SCREEN_WIDTH	(1920)
#define SCREEN_HEIGHT	(1080)
#endif

#define SCREEN_SCALE ((SCREEN_WIDTH + SCREEN_HEIGHT) / 1000)

HWND GetWindow();
int GetRandom(); // +と-がどちらも発生する
bool GetWindowActive();
