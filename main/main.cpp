#include "main.h"
#include "manager/sceneManager.h"
#include <d2d1.h>
#include <dwrite.h>
#include <wrl.h>
#include <iostream>
#include <time.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "Stinger";
const char* GAME_VERSION = "0.9.0";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


HWND g_Window;
int g_Seed = 0;

HWND GetWindow()
{
	return g_Window;
}

int XorShiftInt(int& state)
{
	state ^= state << 13;
	state ^= state >> 17;
	state ^= state << 5;
	return state;
}

int GetRandom()
{
	return XorShiftInt(g_Seed);
}



LRESULT CALLBACK FPSWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	GetRandom();
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

// FPSウインドウ用
Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> g_FPSRenderTarget;
Microsoft::WRL::ComPtr<ID2D1Factory> g_D2DFactory;
Microsoft::WRL::ComPtr<IDWriteTextFormat> g_TextFormat;
Microsoft::WRL::ComPtr<IDWriteFactory> g_DWriteFactory;

void InitializeFPSDirectWrite(HWND hwnd)
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, g_D2DFactory.GetAddressOf());
	RECT rc;
	GetClientRect(hwnd, &rc);

	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, g_D2DFactory.GetAddressOf());
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(g_DWriteFactory.GetAddressOf()));

	g_DWriteFactory->CreateTextFormat(
		L"Arial",                // フォント
		nullptr,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		24.0f,                   // サイズ
		L"en-us",
		g_TextFormat.GetAddressOf()
	);

	hr = g_D2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
		g_FPSRenderTarget.GetAddressOf()
	);
}

void RenderFPSWindow(const float& fps)
{
	g_FPSRenderTarget->BeginDraw();
	g_FPSRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	wchar_t fpsText[256];
	swprintf_s(fpsText, 256, L"FPS: %.2f", fps);

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pBrush;
	g_FPSRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), pBrush.GetAddressOf());

	g_FPSRenderTarget->DrawText(
		fpsText,
		static_cast<UINT32>(wcslen(fpsText)),
		g_TextFormat.Get(),
		D2D1::RectF(0, 0, 200, 50),  // 表示領域
		pBrush.Get()
	);

	g_FPSRenderTarget->EndDraw();
}

// ボーダレスウインドウ
void SetBorderlessWindow(HWND hwnd)
{
	// 現在のモニターの解像度を取得
	HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO mi = { sizeof(mi) };
	if (GetMonitorInfo(hMonitor, &mi))
	{
		// ボーダーを無くす
		SetWindowLong(hwnd, GWL_STYLE, WS_POPUP);
		SetWindowPos(hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
			mi.rcMonitor.right - mi.rcMonitor.left,
			mi.rcMonitor.bottom - mi.rcMonitor.top,
			SWP_FRAMECHANGED | SWP_NOOWNERZORDER);
	}
}

int APIENTRY WinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(2329);

	// 乱数のシードを設定
	srand(static_cast<unsigned int>(time(0)));

	g_Seed = static_cast<int>(time(0));
	// マウスカーソルの表示
#if _DEBUG
	ShowCursor(true);
#else
	// TODO :falseに変更予定 / デバッグ用
	ShowCursor(true);
#endif


	//// 乱数エンジンを初期化する (mt19937はよく使われる乱数生成器)
	//std::random_device rd;  // ノイズを使用したシード
	//std::mt19937 gen(rd()); // メルセンヌ・ツイスタ法を使用
	//std::uniform_int_distribution<> distrib(1, 100); // 1から100の間の整数を生成

	WNDCLASSEX wcex = { 0 };
	{
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = 0;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = nullptr;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = CLASS_NAME;
		wcex.hIconSm = nullptr;

		RegisterClassEx(&wcex);


		RECT rc = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		g_Window = CreateWindowEx(0, CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
	}

	#if _DEBUG
	
	#else // releseの時のみ
	 //ボーダレスウインドウ
	SetBorderlessWindow(g_Window);
	#endif

	ShowWindow(g_Window, nCmdShow);
	UpdateWindow(g_Window);


#if _DEBUG


	{
		const char* fpsClassName = "FPSClass";
		const char* fpsWindowName = "FPSWindow";

		// FPSウィンドウ用
		WNDCLASSEX fpsWcex = { 0 };
		fpsWcex.cbSize = sizeof(WNDCLASSEX);
		fpsWcex.style = 0;
		fpsWcex.lpfnWndProc = FPSWndProc; 
		fpsWcex.cbClsExtra = 0;
		fpsWcex.cbWndExtra = 0;
		fpsWcex.hInstance = hInstance;
		fpsWcex.hIcon = nullptr;
		fpsWcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		fpsWcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		fpsWcex.lpszMenuName = nullptr;
		fpsWcex.lpszClassName = fpsClassName;
		fpsWcex.hIconSm = nullptr;

		RegisterClassEx(&fpsWcex);


		// FPS表示用のウィンドウを作成
		HWND hFPSWindow = CreateWindowEx(
			0,
			fpsClassName,
			fpsWindowName,
			WS_OVERLAPPEDWINDOW,
			0, 0,				// 位置
			300, 100,			// サイズ
			nullptr,
			nullptr,
			hInstance,
			nullptr
		);

		// FPSウィンドウを表示
		ShowWindow(hFPSWindow, nCmdShow);
		UpdateWindow(hFPSWindow);
		InitializeFPSDirectWrite(hFPSWindow);

		// フォント
		HRESULT hr = g_DWriteFactory->CreateTextFormat(
			L"Arial",						// フォント
			nullptr,                 
			DWRITE_FONT_WEIGHT_REGULAR,		// 太さ
			DWRITE_FONT_STYLE_NORMAL,		// スタイル
			DWRITE_FONT_STRETCH_NORMAL,		// 幅
			24.0f,							// サイズ
			L"en-us",                   
			g_TextFormat.GetAddressOf()
		);
	}
#endif
	
	SceneManager::Init();

	// DeltaTime用変数
	LARGE_INTEGER frequency = { 0 };
	LARGE_INTEGER prevTime = { 0 }, currTime = { 0 };

	// 周波数の初期化
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&prevTime);

	// フレームの制御
	const double& targetDeltaTime = 1.0 / 60.0;

	MSG msg;
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			// 現在の時間を取得
			QueryPerformanceCounter(&currTime);
			// 経過時間を秒で計算
			const double& deltaTime = static_cast<double>(currTime.QuadPart - prevTime.QuadPart) / frequency.QuadPart;

			if (deltaTime >= targetDeltaTime)
			{
				// FPSを計算
				float deltaTimeFloat = static_cast<float>(deltaTime);
				const float& fps = 1.0f / deltaTimeFloat;
				
				// 上限設定 / ブレイクポイント用
				if (deltaTimeFloat >= 0.2f)
				{
					deltaTimeFloat = 0.0f;
				}

				SceneManager::Update(deltaTimeFloat);

				SceneManager::Draw();

				#if _DEBUG
				// FPSウィンドウの描画
				RenderFPSWindow(fps);
				#endif

				prevTime = currTime;
			}
		}
	}

	UnregisterClass(CLASS_NAME, wcex.hInstance);

	SceneManager::Uninit();

	return (int)msg.wParam;
}




LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


