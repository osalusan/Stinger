#pragma once
#include <vector>
#include <string>
#include <windows.h>

// 前方宣言
class BehaviorNode;
class Scene;
class BossEnemy;
class Player;
class PlayerStateMachine;
struct ImFont;
// デバッグ画面
class ImguiWindow final
{
private:
    const BehaviorNode* m_RootNodeCache = nullptr;
    const BossEnemy* m_BossEnemyCache = nullptr;
    const Player* m_PlayerCache = nullptr;
    const PlayerStateMachine* m_PlayerStateMachineCache = nullptr;
    std::vector<std::string> m_TaskName = {};

    ImFont* m_Font;

    std::wstring ToWString(const std::string& str, UINT codePage);
    std::string ToUtf8(const std::wstring& wstr);
    std::string GetCurrentPlayerStateName();
    std::string GetCurrentEnemyAttackParts();

    int m_ShowStyleDerivationInfo = 1;
    int m_ShowDerivationInfo = 1;
public:
    ImguiWindow();  // RendererクラスのInitを呼んだ後に作成
    ~ImguiWindow();

    void GetRootNode(Scene* scene);         // Scene内のオブジェクトを全て初期化した後に呼ぶ
    void Update(const float& deltaTime);
    void Draw();
    // 再起呼び出しで、ツリーにある全てのタスクを呼び出すように
    void DrawBehaviorTree(const BehaviorNode* root,const BehaviorNode* parentNode = nullptr,const int& parentChildNum = 0);
    void ClearNode();
};
