#include "imguiWindow.h"
#include "main/main.h"
#include "scene/scene.h"
#include "renderer/renderer.h"
#include "manager/objectManager.h"
#include "character/bossEnemy.h"
#include "character/player.h"
#include "playerState/playerStateMachine.h"
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
    Player* player = objManager->GetPlayer();
    if (player == nullptr) return;
    const PlayerStateMachine* playerStateMachine = player->GetPlayerStateMachine();
    if (playerStateMachine == nullptr) return;
    BossEnemy* boss = objManager->GetBossEnemy();
    if (boss == nullptr) return;
    const BehaviorTree* tree = boss->GetBehaviourTree();
    if (tree == nullptr) return;

    m_PlayerCache = player;
    m_PlayerStateMachineCache = playerStateMachine;
    m_BossEnemyCache = boss;
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

    ImGui::Text(u8"------------- プレイヤー -------------");
    if (m_PlayerCache != nullptr)
    {
        ImGui::Text(u8"体力 :%f / %f", m_PlayerCache->GetHealth(), m_PlayerCache->GetMaxHealth());
    }
    if (m_PlayerStateMachineCache != nullptr)
    {
        m_PlayerStateMachineCache->GetCurrentState();
        std::wstring wstr = ToWString(GetCurrentPlayerStateName(), 932);
        std::string utf8Name = ToUtf8(wstr);

        ImGui::Text(u8"現在のステート :%s", utf8Name.c_str());
    }


    ImGui::Text(u8"------------- エネミー -------------");
    if (m_BossEnemyCache != nullptr)
    {
        ImGui::Text(u8"体力 :%f / %f", m_BossEnemyCache->GetHealth(), m_BossEnemyCache->GetMaxHealth());
        ImGui::SameLine();
        ImGui::Text(u8"| スタミナ :%f / %f", m_BossEnemyCache->GetStamina(), m_BossEnemyCache->GetaMaxStamina());

        std::wstring atkPartsName = ToWString(GetCurrentEnemyAttackParts(), 932);
        std::string atkPartsUtf8Name = ToUtf8(atkPartsName);
        ImGui::Text(u8"攻撃判定部位 :%s", atkPartsUtf8Name.c_str());
    }
    ImGui::Text(u8"ビヘイビアツリー");
    // 青色：現在使用中
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f),u8"青色：現在使用中");
    // 改行しない
    ImGui::SameLine();
    // 黄色：確率上限エラー
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),u8"黄色：確率上限エラー");

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

    // 確率エラー
    if (root->GetTotalDerivChance() > 100)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // 文字色
    }
    // 使用中
    else if (root->GetNodeState() != NODE_STATE::FAILURE)
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
        int childNum = 0;
        for (const BehaviorNode* child : root->GetChildren())
        {
            DrawBehaviorTree(child);

            // 派生技のデータを表記
            if (root->GetDerivationData().size() > 0)
            {
                 ImGui::SameLine();
                 ImGui::Text(u8"派生可能体力 : %f", root->GetDerivationData(childNum).Health * m_BossEnemyCache->GetMaxHealth());
                 ImGui::SameLine();
                 ImGui::Text(u8"| 派生確率 : %i %%", root->GetDerivationData(childNum).Chance);

                 childNum++;
            }
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
        return L"";
    }

    std::wstring wstr(sizeNeeded, L'\0');
    MultiByteToWideChar(codePage, 0, str.c_str(), -1, &wstr[0], sizeNeeded);
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
    str.resize(sizeNeeded - 1);
    return str;
}

std::string ImguiWindow::GetCurrentPlayerStateName()
{
    std::string playerStateName = "";
    if (m_PlayerStateMachineCache == nullptr) return "";
    switch (m_PlayerStateMachineCache->GetCurrentState())
    {
    case PLAYER_STATE::NONE:
        playerStateName = "None";
        break;
    case PLAYER_STATE::IDLE:
        playerStateName = "待機";
        break;
    case PLAYER_STATE::HITDAMAGE:
        playerStateName = "被ダメ";
        break;
    case PLAYER_STATE::ROLLING:
        playerStateName = "回避";
        break;
    case PLAYER_STATE::ATTACK_PARRY:
        playerStateName = "パリィ";
        break;
    case PLAYER_STATE::ATTACK_NORMAL:
        playerStateName = "通常攻撃";
        break;
    case PLAYER_STATE::RUN:
        playerStateName = "走り";
        break;
    case PLAYER_STATE::DEAD:
        playerStateName = "死亡";
        break;
    case PLAYER_STATE::MAX:
        playerStateName = "最大値";
        break;
    default:
        playerStateName = "割り当てなし";
        break;
    }

    return playerStateName;
}

std::string ImguiWindow::GetCurrentEnemyAttackParts()
{
    std::string currentAttackParts = "";
    if (m_BossEnemyCache == nullptr) return "";
    switch (m_BossEnemyCache->GetCurrentAttackParts())
    {
    case ATTACK_PARTS::NONE:
        currentAttackParts = "なし";
        break;
    case ATTACK_PARTS::BODY:
        currentAttackParts = "体";
        break;
    case ATTACK_PARTS::ARM:
        currentAttackParts = "両腕";
        break;
    case ATTACK_PARTS::RIGHT_ARM:
        currentAttackParts = "右腕";
        break;
    case ATTACK_PARTS::LEFT_ARM:
        currentAttackParts = "左腕";
        break;
    case ATTACK_PARTS::RIGHT_LEG:
        currentAttackParts = "右足";
        break;
    case ATTACK_PARTS::LEFT_LEG:
        currentAttackParts = "左足";
        break;
    case ATTACK_PARTS::ALL:
        currentAttackParts = "すべて";
        break;
    case ATTACK_PARTS::MAX:
        currentAttackParts = "最大値";
        break;
    default:
        currentAttackParts = "割り当てなし";
        break;
    }

    return currentAttackParts;
}
