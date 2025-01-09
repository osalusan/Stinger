#include "imguiWindow.h"
#include "main/main.h"
#include "scene/scene.h"
#include "renderer/renderer.h"
#include "manager/objectManager.h"
#include "character/bossEnemy.h"
#include "behaviorTree/behaviorTree.h"
#include "behaviorNode/selectorNode.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

// RendererのInitを呼んだ後に作成
ImguiWindow::ImguiWindow()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    m_Font = io.Fonts->AddFontFromFileTTF("imgui/fonts/VL_Gothic_Regular.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    if (m_Font != nullptr)
    {
        io.Fonts->Build();
    }
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

void ImguiWindow::GetRootNode(Scene* scene)
{
    if (scene == nullptr) return;
    ObjectManager* objManager = scene->GetObjectManager();
    if (objManager == nullptr) return;
    BossEnemy* boss = objManager->GetBossEnemy();
    if (boss == nullptr) return;
    const BehaviorTree* tree = boss->GetBehaviourTree();
    if (tree == nullptr) return;

    m_RootNodeCache = tree->GetRootNode();
}

void ImguiWindow::Update(const float& deltaTime)
{
    ImGui_ImplWin32_NewFrame();
    ImGui_ImplDX11_NewFrame();
    ImGui::NewFrame();
    // NewFrameの後
    if (m_Font != nullptr)
    {
        ImGui::PushFont(m_Font);
    }
 

    const float& fps = 1.0f / deltaTime;

    ImGui::Begin("DebugWindow");
    ImGui::Text("FPS :%f", fps);
    ImGui::Text(u8"ビヘイビアツリー");

    DrawBehaviorTree(m_RootNodeCache);

    if (m_Font != nullptr)
    {
        ImGui::PopFont();
    }
    ImGui::End();
}

void ImguiWindow::Draw()
{
    ImGui::Render();  // 描画コマンドの生成
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // 実行
}

void ImguiWindow::DrawBehaviorTree(const BehaviorNode* root)
{
    if (root == nullptr) return;
    std::string name = root->GetTaskName().c_str();
    if (name == "")
    {
        name = "noNameError";
    }

    // utf-8に変換
    std::wstring wstr = ToWString(name, 932);
    std::string utf8Name = ToUtf8(wstr);

    if (root->GetNodeState() != NODE_STATE::FAILURE)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 1.0f, 1.0f)); // 文字色
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // 文字色
    }

    
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode(utf8Name.c_str()))
    {
        for (const BehaviorNode* child : root->GetChildren())
        {
            DrawBehaviorTree(child);
        }
        ImGui::TreePop();
    }
    ImGui::PopStyleColor(1);
}

void ImguiWindow::ClearNode()
{
    m_RootNodeCache = nullptr;
}

// --------------------------------- private ---------------------------------
std::wstring ImguiWindow::ToWString(const std::string& str, UINT codePage)
{
    int sizeNeeded = MultiByteToWideChar(codePage, 0, str.c_str(), -1, nullptr, 0);
    if (sizeNeeded == 0) {
        // エラー処理
        return L"";
    }

    std::wstring wstr(sizeNeeded, L'\0');
    MultiByteToWideChar(codePage, 0, str.c_str(), -1, &wstr[0], sizeNeeded);
    // C++17以降では末尾のヌル文字のケアが必要、消したい場合は
    wstr.resize(sizeNeeded - 1);
    return wstr;
}

std::string ImguiWindow::ToUtf8(const std::wstring& wstr)
{
    // UTF-8へ変換
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (sizeNeeded == 0) {
        // エラー処理
        return "";
    }

    std::string str(sizeNeeded, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], sizeNeeded, nullptr, nullptr);
    // こちらも末尾のヌル文字のケアが必要になる場合があるので必要なら resize
    str.resize(sizeNeeded - 1);
    return str;
}
