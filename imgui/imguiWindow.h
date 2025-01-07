#pragma once
#include <vector>
#include <string>

class BehaviorNode;
class Scene;
struct ImFont;
class ImguiWindow
{
private:
    const BehaviorNode* m_RootNodeCache = nullptr;
    std::vector<std::string> m_TaskName = {};

    ImFont* m_Font;
public:
    ImguiWindow();
    ~ImguiWindow();

    void GetRootNode(Scene* scene);
    void Update(const float& deltaTime);
    void Draw();
    void DrawBehaviorTree(const BehaviorNode* root);
};
