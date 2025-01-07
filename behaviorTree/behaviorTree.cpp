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
        m_Root->SetTaskName("���[�g�m�[�h");
    }
}

void BehaviorTree::Update(const float& deltaTime)
{
    if (m_Root == nullptr) return;
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
