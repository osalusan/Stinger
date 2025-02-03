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

// Renderer��Init���Ă񂾌�ɍ쐬
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
    // NewFrame�̌�
    if (m_Font != nullptr)
    {
        ImGui::PushFont(m_Font);
    }
 

    const float& fps = 1.0f / deltaTime;

    ImGui::Begin("DebugWindow");
    ImGui::Text("FPS :%f", fps);

    ImGui::Text(u8"------------- �v���C���[ -------------");
    if (m_PlayerCache != nullptr)
    {
        ImGui::Text(u8"�̗� :%f / %f", m_PlayerCache->GetHealth(), m_PlayerCache->GetMaxHealth());
    }
    if (m_PlayerStateMachineCache != nullptr)
    {
        m_PlayerStateMachineCache->GetCurrentState();
        std::wstring wstr = ToWString(GetCurrentPlayerStateName(), 932);
        std::string utf8Name = ToUtf8(wstr);

        ImGui::Text(u8"���݂̃X�e�[�g :%s", utf8Name.c_str());
    }


    ImGui::Text(u8"------------- �G�l�~�[ -------------");
    if (m_BossEnemyCache != nullptr)
    {
        ImGui::Text(u8"�̗� :%f / %f", m_BossEnemyCache->GetHealth(), m_BossEnemyCache->GetMaxHealth());
        ImGui::SameLine();
        ImGui::Text(u8"| �X�^�~�i :%f / %f", m_BossEnemyCache->GetStamina(), m_BossEnemyCache->GetaMaxStamina());

        std::wstring atkPartsName = ToWString(GetCurrentEnemyAttackParts(), 932);
        std::string atkPartsUtf8Name = ToUtf8(atkPartsName);
        ImGui::Text(u8"�U�����蕔�� :%s", atkPartsUtf8Name.c_str());
    }
    ImGui::Text(u8"�r�w�C�r�A�c���[");
    // �F�F���ݎg�p��
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f),u8"�F�F���ݎg�p��");
    // ���s���Ȃ�
    ImGui::SameLine();
    // ���F�F�m������G���[
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),u8"���F�F�m������G���[");

    DrawBehaviorTree(m_RootNodeCache);

    if (m_Font != nullptr)
    {
        ImGui::PopFont();
    }
    ImGui::End();
}

void ImguiWindow::Draw()
{
    ImGui::Render();  // �`��R�}���h�̐���
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // ���s
}

void ImguiWindow::DrawBehaviorTree(const BehaviorNode* root)
{
    if (root == nullptr) return;
    std::string name = root->GetTaskName().c_str();
    if (name == "")
    {
        name = "noNameError";
    }

    // utf-8�ɕϊ�
    std::wstring wstr = ToWString(name, 932);
    std::string utf8Name = ToUtf8(wstr);

    // �m���G���[
    if (root->GetTotalDerivChance() > 100)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // �����F
    }
    // �g�p��
    else if (root->GetNodeState() != NODE_STATE::FAILURE)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 1.0f, 1.0f)); // �����F
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // �����F
    }

    
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode(utf8Name.c_str()))
    {
        int childNum = 0;
        for (const BehaviorNode* child : root->GetChildren())
        {
            DrawBehaviorTree(child);

            // �h���Z�̃f�[�^��\�L
            if (root->GetDerivationData().size() > 0)
            {
                 ImGui::SameLine();
                 ImGui::Text(u8"�h���\�̗� : %f", root->GetDerivationData(childNum).Health * m_BossEnemyCache->GetMaxHealth());
                 ImGui::SameLine();
                 ImGui::Text(u8"| �h���m�� : %i %%", root->GetDerivationData(childNum).Chance);

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
    // UTF-8�֕ϊ�
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (sizeNeeded == 0) {
        // �G���[����
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
        playerStateName = "�ҋ@";
        break;
    case PLAYER_STATE::HITDAMAGE:
        playerStateName = "��_��";
        break;
    case PLAYER_STATE::ROLLING:
        playerStateName = "���";
        break;
    case PLAYER_STATE::ATTACK_PARRY:
        playerStateName = "�p���B";
        break;
    case PLAYER_STATE::ATTACK_NORMAL:
        playerStateName = "�ʏ�U��";
        break;
    case PLAYER_STATE::RUN:
        playerStateName = "����";
        break;
    case PLAYER_STATE::DEAD:
        playerStateName = "���S";
        break;
    case PLAYER_STATE::MAX:
        playerStateName = "�ő�l";
        break;
    default:
        playerStateName = "���蓖�ĂȂ�";
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
        currentAttackParts = "�Ȃ�";
        break;
    case ATTACK_PARTS::BODY:
        currentAttackParts = "��";
        break;
    case ATTACK_PARTS::ARM:
        currentAttackParts = "���r";
        break;
    case ATTACK_PARTS::RIGHT_ARM:
        currentAttackParts = "�E�r";
        break;
    case ATTACK_PARTS::LEFT_ARM:
        currentAttackParts = "���r";
        break;
    case ATTACK_PARTS::RIGHT_LEG:
        currentAttackParts = "�E��";
        break;
    case ATTACK_PARTS::LEFT_LEG:
        currentAttackParts = "����";
        break;
    case ATTACK_PARTS::ALL:
        currentAttackParts = "���ׂ�";
        break;
    case ATTACK_PARTS::MAX:
        currentAttackParts = "�ő�l";
        break;
    default:
        currentAttackParts = "���蓖�ĂȂ�";
        break;
    }

    return currentAttackParts;
}
