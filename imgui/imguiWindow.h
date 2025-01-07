#pragma once

class ImguiWindow
{
public:
    ImguiWindow();
    ~ImguiWindow();

    void Update(const float& deltaTime);
    void Draw();
};
