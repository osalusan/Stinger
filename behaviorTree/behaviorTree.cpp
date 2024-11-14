#include "behaviorTree.h"

BehaviourTree::BehaviourTree(BehaviorNode* rootNode)
{
    if (rootNode != nullptr && m_Root == nullptr)
    {
        m_Root = m_Root;
    }
    if (m_Root != nullptr)
    {
        m_Root->Init();
    }
}

void BehaviourTree::Update()
{
    NODE_STATUS status = m_Root->Update();

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
