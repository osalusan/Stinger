#include "imguiWindow.h"
#include "main/main.h"
#include "renderer/renderer.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

// RendererのInitを呼んだ後に作成
ImguiWindow::ImguiWindow()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    
    ID3D11Device* device = Renderer::GetDevice();
    if (device == nullptr) return;
    ID3D11DeviceContext* diviceContext = Renderer::GetDeviceContext();
    if (diviceContext == nullptr) return;

    HWND hwnd = GetWindow();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(device, diviceContext);

    ImGui::StyleColorsDark();
}

ImguiWindow::~ImguiWindow()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImguiWindow::Update(const float& deltaTime)
{
    ImGui_ImplWin32_NewFrame();
    ImGui_ImplDX11_NewFrame();
    ImGui::NewFrame();

    const float& fps = 1.0f / deltaTime;

    ImGui::Begin("DebugWindow");
    ImGui::Text("FPS :%f", fps);
    ImGui::End();
}

void ImguiWindow::Draw()
{
    ImGui::Render();  // 描画コマンドの生成
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // 実行
}
