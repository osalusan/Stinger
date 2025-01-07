#include "behaviorTree.h"

BehaviorTree::~BehaviorTree()
{
    delete m_Root;
    m_Root = nullptr;
}

void BehaviorTree::CreateRoot(BehaviorNode* root)
{
    if (m_Root == nullptr && root != nullptr)
    {
        m_Root = root;
    }
    if (m_Root != nullptr)
    {
        m_Root->Init();
        m_Root->SetTaskName("ルートノード");
    }
}

void BehaviorTree::Update(const float& deltaTime)
{
    if (m_Root == nullptr) return;
    NODE_STATUS status = m_Root->Update(deltaTime);

    // デバッグ用
#if _DEBUG
    if (status == NODE_STATUS::SUCCESS)
    {

    }
    else if (status == NODE_STATUS::FAILURE)
    {

    }
    else
    {

    }
#endif
}
