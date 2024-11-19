#include "moveMawJLaygoSelector.h"

constexpr float WAIT_TIME = 0.3f;

void MoveMawJLaygoSelector::Init()
{
    m_CurrentTime = WAIT_TIME;
}

NODE_STATUS MoveMawJLaygoSelector::Update(const float& deltaTime)
{
    // àÍî‘ç≈èâÇ…
    m_CurrentTime += deltaTime;

    if (m_CurrentTime < WAIT_TIME) return NODE_STATUS::SUCCESS;

    for (TaskNode* child : m_Children)
    {
        if (child == nullptr) continue;

        NODE_STATUS status = child->Update(deltaTime);
        if (status != NODE_STATUS::FAILURE)
        {
            if (status == NODE_STATUS::SUCCESS)
            {
                m_CurrentTime = 0.0f;
            }
            return status;
        }
    }
    return NODE_STATUS::FAILURE;
}
