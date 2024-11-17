#include "behaviorTree.h"

void BehaviourTree::CreateRoot(BehaviorNode* root)
{
    if (m_Root == nullptr && root != nullptr)
    {
        m_Root = root;
    }
    if (m_Root != nullptr)
    {
        m_Root->Init();
    }
}

void BehaviourTree::Update(const float& deltaTime)
{
    NODE_STATUS status = m_Root->Update(deltaTime);

    // �f�o�b�O�p
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
