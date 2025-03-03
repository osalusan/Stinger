// 一番最初にインクルード
#include "renderer/fbxModelRenderer.h"

#include "imguiWindow.h"
#include "main/main.h"
#include "scene/scene.h"
#include "manager/objectManager.h"
#include "manager/fbxModelManager.h"
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
    ImGuiIO& io = ImGui::GetIO();
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
        ImGui::Text(u8"体力 :%.3f / %.3f", m_PlayerCache->GetHealth(), m_PlayerCache->GetMaxHealth());
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
        ImGui::Text(u8"体力 :%.3f / %.3f", m_BossEnemyCache->GetHealth(), m_BossEnemyCache->GetMaxHealth());
        ImGui::SameLine();
        ImGui::Text(u8"| スタミナ :%.3f / %.3f", m_BossEnemyCache->GetStamina(), m_BossEnemyCache->GetMaxStamina());

        std::wstring atkPartsName = ToWString(GetCurrentEnemyAttackParts(), 932);
        std::string atkPartsUtf8Name = ToUtf8(atkPartsName);
        ImGui::Text(u8"攻撃判定部位 :%s", atkPartsUtf8Name.c_str());
    }
    ImGui::Text(u8"------------- ビヘイビアツリー -------------");

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.7f, 1.0f));         // 通常時の色
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.7f, 0.9f, 1.0f));  // ホバー時の色
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.5f, 0.7f, 1.0f));   // アクティブ時の色

    if (ImGui::Button(u8"派生技情報の表示形式を変更"))
    {
        if (m_ShowStyleDerivationInfo == 0)
        {
            m_ShowStyleDerivationInfo = 1;
        }
        else if (m_ShowStyleDerivationInfo == 1)
        {
            m_ShowStyleDerivationInfo = 0;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button(u8"派生条件の表示情報を変更"))
    {
        if (m_ShowDerivationInfo == 0)
        {
            m_ShowDerivationInfo = 1;
        }
        else if (m_ShowDerivationInfo == 1)
        {
            m_ShowDerivationInfo = 0;
        }
    }

    ImGui::PopStyleColor(3);

    // 青色：現在使用中
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f),u8"青色：現在使用中");
    ImGui::SameLine();
    // 黄色：確率上限エラー
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),u8"黄色：確率エラー");
    ImGui::SameLine();
    // 灰色：発動条件未達成
    ImGui::TextColored(ImVec4(0.4f, 0.5f, 0.6f, 1.0f), u8"灰色：発動条件未達成");
    ImGui::SameLine();
    // 緑色：スタミナ不足
    ImGui::TextColored(ImVec4(0.48f, 0.98f, 0.2f, 1.0f), u8"緑色：スタミナ不足");

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

void ImguiWindow::DrawBehaviorTree(const BehaviorNode* root, const BehaviorNode* parentNode, const int& parentChildNum)
{
    if (parentNode == nullptr)
    {
        parentNode = root;
    }
    if (parentNode == nullptr || root == nullptr) return;

    std::string name = root->GetTaskName().c_str();
    if (name == "")
    {
        name = "noNameError";
    }

    // utf-8に変換
    std::wstring wstr = ToWString(name, 932);
    std::string utf8Name = ToUtf8(wstr);

    const float& healthValue = parentNode->GetDerivationData(parentChildNum).HealthValue;

    // 確率エラー
    if (root->GetTotalDerivChance() != 100 && root->GetTotalDerivChance() != 0)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // 黄色
    }
    // 使用中
    else if (root->GetNodeState() != NODE_STATE::FAILURE)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 1.0f, 1.0f)); // 青色
    }
    // 発動条件未達成
    else if (healthValue != 0.0f && healthValue * m_BossEnemyCache->GetMaxHealth() < m_BossEnemyCache->GetHealth())
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.5f, 0.6f, 1.0f)); // 灰色
    }
    // スタミナ不足
    else if (m_BossEnemyCache->GetStamina() < root->GetUseStaminaValue() * m_BossEnemyCache->GetMaxStamina())
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.48f, 0.98f, 0.2f, 1.0f)); // 緑色
    }
    // 標準
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // 白色
    }

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    ImVec4 white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    if (ImGui::TreeNode(utf8Name.c_str()))
    {
        int childNum = 0;

        if (parentNode != root)
        {
            float maxParentAnimTime = 0.0f;
            float maxMyAnimTime = 0.0f;
            if (const FbxModelRenderer* model = FbxModelManager::GetAnimationModel(m_BossEnemyCache->GetAnimeModel()))
            {
                maxParentAnimTime = model->GetMaxAnimeTime(parentNode->GetAnimName());
            }
            if (const FbxModelRenderer* model = FbxModelManager::GetAnimationModel(m_BossEnemyCache->GetAnimeModel()))
            {
                maxMyAnimTime = model->GetMaxAnimeTime(root->GetAnimName());
            }

            // 分岐確率
            if (parentNode->GetTotalDerivChance() > 0)
            {
                ImGui::SameLine();
                ImGui::Text(u8"(分岐確率: %i %%)", parentNode->GetDerivChance(parentChildNum));
            }
            if (root->GetDerivationData().size() > 0 && maxMyAnimTime != 0.0f)
            {
                ImGui::SameLine();
                ImGui::Text(u8"(タスクの最大時間: %.2f秒)", maxMyAnimTime);
            }

            if (m_ShowDerivationInfo == 1)
            {
                const float& parentTransTimeValue = parentNode->GetDerivationData(parentChildNum).TransTimeValue;
                // 派生技のデータを表記
                if (parentNode->GetDerivationData().size() > 0 && parentTransTimeValue != 0.0f)
                {
                    if (m_ShowStyleDerivationInfo == 0)
                    {
                        ImGui::Text(u8"{");
                        ImGui::Text(u8"  派生可能体力: %.2f", parentNode->GetDerivationData(parentChildNum).HealthValue * m_BossEnemyCache->GetMaxHealth());
                        ImGui::Text(u8"  派生確率: %i%%", parentNode->GetDerivationData(parentChildNum).Chance);
                        ImGui::Text(u8"  派生移行時間: %.2f秒後", parentTransTimeValue * maxParentAnimTime);
                        ImGui::SameLine();
                        ImGui::Text(u8"(%.2f %%)", parentTransTimeValue * 100.0f);
                        ImGui::Text(u8"}");
                    }
                    else if (m_ShowStyleDerivationInfo == 1)
                    {
                        ImGui::Text(u8"{");
                        ImGui::Text(u8"  派生可能体力: %.2f", parentNode->GetDerivationData(parentChildNum).HealthValue * m_BossEnemyCache->GetMaxHealth());
                        ImGui::SameLine();
                        ImGui::Text(u8"| 派生確率: %i%%", parentNode->GetDerivationData(parentChildNum).Chance);
                        ImGui::SameLine();
                        ImGui::Text(u8"| 派生移行時間: %.2f秒後", parentTransTimeValue * maxParentAnimTime);
                        ImGui::SameLine();
                        ImGui::Text(u8"(%.2f %%)", parentTransTimeValue * 100.0f);
                        ImGui::Text(u8"}");
                    }
                }
            }
        }

        for (const BehaviorNode* child : root->GetChildren())
        {
            DrawBehaviorTree(child, root, childNum);

            childNum++;
        }
        ImGui::TreePop();
    }
    ImGui::PopStyleColor(1);
}

void ImguiWindow::ClearNode()
{
    m_RootNodeCache = nullptr;
    m_PlayerCache = nullptr;
    m_BossEnemyCache = nullptr;
    m_PlayerStateMachineCache = nullptr;
    m_TaskName.clear();
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
    case PLAYER_STATE::ATTACK_EXTR:
        playerStateName = "エクストラ攻撃";
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
