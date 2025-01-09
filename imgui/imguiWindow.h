#pragma once
#include <vector>
#include <string>
#include <Windows.h>

class BehaviorNode;
class Scene;
struct ImFont;
class ImguiWindow
{
private:
    const BehaviorNode* m_RootNodeCache = nullptr;
    std::vector<std::string> m_TaskName = {};

    ImFont* m_Font;

    std::wstring ToWString(const std::string& str, UINT codePage);
    std::string ToUtf8(const std::wstring& wstr);

public:
    ImguiWindow();
    ~ImguiWindow();

    void GetRootNode(Scene* scene);
    void Update(const float& deltaTime);
    void Draw();
    void DrawBehaviorTree(const BehaviorNode* root);
    void ClearNode();
};
